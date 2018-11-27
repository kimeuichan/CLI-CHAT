#include <stdio.h>
#include <WinSock2.h>
#include <stdlib.h>

#define CHAT_SERVER_ADDR "127.0.0.1"
#define CHAT_PORT 55

#pragma comment(lib, "ws2_32.lib")

void ErrorHandling(char * message);


int main(){
	WSADATA wsa;
	SOCKET s;
	SOCKADDR_IN servAddr;

	char message[30];
	int strLen = 0;
	int idx = 0;
	int read_len;

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

	while (read_len = recv(s, &message[idx++], 1, 0)){
		if (read_len == -1)
			ErrorHandling("read() Error!");
		strLen += read_len;
	}

	printf("Message from server : %s  \n", message);
	printf("read call count : %d\n", strLen);

	closesocket(s);
	WSACleanup();
	return 0;
}

void ErrorHandling(char * message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}