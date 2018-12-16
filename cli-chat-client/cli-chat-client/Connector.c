#include "Connector.h"
#include <WinSock2.h>
#include <stdio.h>
#include <process.h>
#include <io.h>
#include <Windows.h>


unsigned WINAPI recvHandle(void* arg);

WSADATA wsa;
SOCKET s;
SOCKET recvS;
SOCKADDR_IN servAddr;

MESSAGE msg;
MESSAGE* msgbuf;

COORD output_pos = { 0, 24 };

MESSAGE recvmsg;

void InitClient(){
	char msg_buf[100];
	char buf[BUF_SIZE];
	char name[30];
	int strLen = 0;
	int count;
	int i = 0;
	int result;
	int isInit = 0;
	int roomId;
	FILE* recvFp;
	HANDLE thread;
	int error_code;

	if (WSAStartup(MAKEWORD(2, 2), &wsa))
		ErrorHandling("WSA Error");

	s = socket(PF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET)
		ErrorHandling("Invalid Sock Error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(CHAT_SERVER_ADDR);
	servAddr.sin_port = htons(CHAT_PORT);

	if (connect(s, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");


	while (isInit == 0){
		printf("1. 1번방\n2. 2번방\n3. 3번방\n4. 4번방\n5. 5번방\n");
		scanf("%d", &roomId);
		getchar();

		if (roomId < 1 || roomId > 5){
			printf("잘못된 선택입니다.\n");
			continue;
		}

		printf("닉네임 입력 : ");
		gets(name);

		MakeMessge(&msg, JOIN_ROOM, &roomId, 4);
		send(s, &msg, sizeof(MESSAGE), 0);
		isInit = 1;
	}
	
	thread = _beginthreadex(NULL, 0, recvHandle, NULL, 0, NULL);
	if (thread == 0){
		CloseClient();
		ErrorHandling("thread error");
	}

	fflush(stdin);
	system("cls");

	while (1){
		gets(msg_buf);
		sprintf(buf, "%s : %s", name, msg_buf);
		MakeMessge(&msg, STRING_MESSAGE, buf, strlen(buf));
		send(s, &msg, sizeof(MESSAGE), 0);
	}

	closesocket(s);
	WSACleanup();
}

void CloseClient(){
	closesocket(s);
	WSACleanup();
}

void ErrorHandling(char * message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void MakeMessge(MESSAGE* message, int type, char* buf, int str_len){
	message->type = type;
	memcpy(message->msg, buf, str_len);
	message->msgLen = str_len;
}

unsigned WINAPI recvHandle(void* arg){
	int strLen = 0;
	while (1){
		strLen = recv(s, &recvmsg, sizeof(MESSAGE), 0);
		if (strLen != 0){
			switch (recvmsg.type){
			case STRING_MESSAGE:
				recvmsg.msg[recvmsg.msgLen] = '\0';
				puts(recvmsg.msg);
				break;
			case EMOTICON_MESSAGE:
				break;
			case ERROR_MESSAGE:
				break;
				// 룸에 조인 요청
			}
		}
		else if (strLen == -1){
			ErrorHandling("Connect Error");
			CloseClient();
		}
	}
	
}

