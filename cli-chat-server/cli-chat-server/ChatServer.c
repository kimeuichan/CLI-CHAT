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
	fd_set reads, cpyReads;
	TIMEVAL timeout;
	int fdNum, i, strLen, roomIndex;
	int * intbuf;
	char * max_error_msg = "Sorry, full access";
	MESSAGE msg;
	MESSAGE * msgbuf;
	char buf[BUF_SIZE];
	int recvLen;
	int msgSize = sizeof(msg);
	int error_code;

	int cltAdrSize;

	// 초기화
	memset(&roomInfoArr, 0, sizeof(roomInfoArr));
	memset(&roomArr, 0, sizeof(roomArr));
	memset(&user, 0, sizeof(user));

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
	timeout.tv_usec = 0;

	FD_ZERO(&reads);
	FD_SET(serverS, &reads);

	while (1)
	{
		cpyReads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		if ((fdNum = select(0, &cpyReads, 0, 0, &timeout)) == SOCKET_ERROR)
			break;

		if (fdNum == 0)
			continue;

		for (i = 0; i<reads.fd_count; i++)
		{
			if (FD_ISSET(reads.fd_array[i], &cpyReads))
			{
				if (reads.fd_array[i] == serverS)     // connection request!
				{
					cltAdrSize = sizeof(cltAdr);
					cltS = accept(serverS, &cltAdr, &cltAdrSize);
					// 최대 10명이 넘어가면 에러 메시지를 보내고 연결을 끊음
					if (userCount >= 9){
					MakeMessge(&msg, ERROR_MESSAGE, "full connected", 14);
					send(cltS, &msg, sizeof(msg), 0);
					close(cltS);
					printf("full connect");
					break;
					}
					printf("new client connected... : %d\n", cltS);
					FD_SET(cltS, &reads);
					userCount++;
				}
				else    // read message!
				{
					recvLen = recv(reads.fd_array[i], buf, sizeof(MESSAGE), 0);
					if (recvLen == 0)    // close request!
					{
						FD_CLR(reads.fd_array[i], &reads);
						closesocket(cpyReads.fd_array[i]);
						printf("closed client: %d \n", cpyReads.fd_array[i]);
					}
					else if(recvLen > 0){
						msgbuf = buf;
						switch (msgbuf->type){
							// 초기 메세지(룸 정보 전달)
						case INIT_MESSAGE:
							break;
							// 브로드 캐스팅
						case STRING_MESSAGE:
						case EMOTICON_MESSAGE:
							// 유저들을 찾고 방에 메시지 뿌리기
							roomIndex = FindRoomIndexByCltS(reads.fd_array[i]);
							if (roomIndex != -1)
								BroadCasetByRoomIndex(roomIndex, msgbuf, reads.fd_array[i]);
							else
								puts("Not founded Room index");
							break;
							// 룸에 조인 요청
						case JOIN_ROOM:
							intbuf = msgbuf->msg;
							JoinRoom((*intbuf) - 1, reads.fd_array[i]);
							break;
						}
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
	int i;
	for(i = userindex; i < roomArr[roomindex].userCount; i++){
		roomArr[roomindex].cltSock[i] = roomArr[roomindex].cltSock[i + 1];
	}
	roomArr[roomindex].userCount--;
}

void BroadCasetByRoomIndex(int index, MESSAGE* msg, int cltS){
	int i;
	for (i = 0; i < roomArr[index].userCount; i++){
		if (roomArr[index].cltSock[i] != cltS){
			printf("send %dth room : %d->%d from : %d\n", index, roomArr[index].cltSock[i], msg->msgLen, cltS);
			send(roomArr[index].cltSock[i], msg, sizeof(MESSAGE), 0);
		}
	}
}

void JoinRoom(int roomindex, int clts){
	int currentPos = roomArr[roomindex].userCount;
	if (currentPos >= MAX_ROOM_USER)
		return;
	roomArr[roomindex].cltSock[currentPos++] = clts;
	roomArr[roomindex].userCount = currentPos;
	printf("%d room's counts : %d\n", roomindex, roomArr[roomindex].userCount);
}