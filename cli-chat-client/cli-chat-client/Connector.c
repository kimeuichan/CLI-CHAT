#include "Connector.h"
#include <WinSock2.h>
#include <stdio.h>

#define BUF_SIZE 1024

WSADATA wsa;
SOCKET s;
SOCKADDR_IN servAddr;

void InitClient(){
	char buf[BUF_SIZE] = "Hello";
	int strLen = 0;
	int count;
	int i = 0;
	int result;

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

	while (1){
		send(s, buf, strlen(buf), 0);
		Sleep(2000);
	}
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

