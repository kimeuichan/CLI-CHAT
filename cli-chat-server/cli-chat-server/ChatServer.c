#include "ChatServer.h"
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <process.h>

RoomInfo roomInfoArr[MAX_ROOM_COUNT];
Room roomArr[MAX_ROOM_COUNT];
int userCount, roomCount;
int tmpCltS[MAX_CON_USER];
User user[MAX_CON_USER];
SOCKET serverS, cltS;

void start(){
	WSADATA wsaData;
	SOCKADDR_IN serAdr, cltAdr;
	FD_SET fd;
	TIMEVAL timeout;
	int fdnum, i, strLen, roomIndex;
	char * max_error_msg = "Sorry, full access";
	MESSAGE msg;
	MESSAGE msgbuf;
	char buf[BUF_SIZE];
	int recvLen;
	int msgSize = sizeof(msg);

	int cltAdrSize;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup Error");

	serverS = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serAdr, 0, sizeof(serAdr));
	serAdr.sin_family = AF_INET;
	serAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	serAdr.sin_port = htons(atoi(CHAT_PORT));

	if (bind(serverS, (SOCKADDR*)&serAdr, sizeof(serAdr)) == SOCKET_ERROR)
		ErrorHandling("bind Error");

	if (listen(serverS, 5) == SOCKET_ERROR)
		ErrorHandling("listen Error");

	userCount = 0;
	roomCount = 0;

	timeout.tv_sec = 5;
	timeout.tv_usec = 5000;

	FD_ZERO(&fd);
	FD_SET(serverS, &fd);

	while (1){
		if ((fdnum = select(0, &fd, 0, 0, &timeout)) == SOCKET_ERROR)
			break;

		if (fdnum == 0)
			continue;

		// ������ ��û�� �ִ��� Ȯ��
		for (i = 0; i < fd.fd_count; i++){
			if (FD_ISSET(fd.fd_array[i], &fd)){
				// ���� ������ ���ο� ��ſ����� ����
				if (fd.fd_array[i] == serverS){
					cltAdrSize = sizeof(cltAdr);
					cltS = accept(serverS, &cltAdr, &cltAdrSize);
					// �ִ� 10���� �Ѿ�� ���� �޽����� ������ ������ ����
					if (userCount >= 9){
						MakeMessge(&msg, ERROR_MESSAGE, "full connected", 14);
						send(cltS, &msg, sizeof(msg), 0);
						close(cltS);
						printf("full connect");
						break;
					}
					printf("new client connected...\n");
					FD_SET(cltS, &fd);
					userCount++;
				}
				// �ƴϸ� �ٸ� ����� request�� ���� ��
				else{
					recvLen = 0;
					// MESSAGE ����� ���� ������ recv
					while (recvLen != msgSize){
						strLen = recv(fd.fd_array[i], buf[recvLen], sizeof(msg), 0);
						// Ŭ���� ��û
						if (strLen == 0){
							closesocket(fd.fd_array[i]);
							FD_CLR(fd.fd_array[i], &fd);
							printf("close socket..\n");
							userCount--;
							break;
						}
						recvLen += strLen;
					}
					//�޼��� ����
					memcpy(&msgbuf, buf, sizeof(msg));

					switch (msgbuf.type){
						// �ʱ� �޼���(�� ���� ����)
						case INIT_MESSAGE:
							break;
						// ��ε� ĳ����
						case STRING_MESSAGE:
						case EMOTICON_MESSAGE:
							// �������� ã�� �濡 �޽��� �Ѹ���
							roomIndex = FindRoomIndexByCltS(fd.fd_array[i]);
							if (roomIndex != -1)
								BroadCasetByRoomIndex(roomIndex, &msg);
							else
								puts("Not founded Room index");
							break;
						// �뿡 ���� ��û
						case JOIN_ROOM:
							break;
					}
				}
			}
		}
	}

	closesocket(serverS);
	WSACleanup();
}

void ErrorHandling(char * message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void MakeMessge(MESSAGE* message, int type, char* buf, int str_len){
	message->type = type;
	strcpy(message->msg, buf, str_len);
	message->msgLen = str_len;
}

int FindUserIndexByCltS(int cltS){

}

int FindRoomIndexByCltS(int cltS){
	int i, j;
	for (i = 0; i < MAX_ROOM_COUNT; i++){
		for (j = 0; j < roomArr[i].userCount; j++){
			if (roomArr[i].cltSock[j] == cltS)
				return i;
		}
	}

	return -1;
}

void CompressUser(int roomindex, int userindex){

}

void BroadCasetByRoomIndex(int index, MESSAGE* msg){
	int i;
	for (i = 0; i < roomArr[i].cltSock; i++){
		send(serverS, &msg, sizeof(MESSAGE), 0);
	}
}


