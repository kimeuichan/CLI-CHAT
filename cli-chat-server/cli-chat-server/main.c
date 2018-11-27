#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#define CHAT_SERVER_ADDR "127.0.0.1"
#define CHAT_PORT 55

void ErrorHandling(char * message);


#pragma comment(lib, "Ws2_32.lib")

int main(){
	WSADATA wsa;
	SOCKET cltSocket, serverSocket;
	SOCKADDR_IN serverAddr, cltAddr;

	int szCltIntAddr;
	char message[] = "Hello World!";

	if (WSAStartup(0x0202, &wsa))
		ErrorHandling("WSA Error");

	serverSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET)
		ErrorHandling("Invalid Sock Error");

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(55);

	if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	if (listen(serverSocket, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	szCltIntAddr = sizeof(cltAddr);
	cltSocket = accept(serverSocket, (SOCKADDR*)&cltAddr, &szCltIntAddr);

	if (cltSocket == INVALID_SOCKET)
		ErrorHandling("accpet() error");

	send(cltSocket, message, sizeof(message), 0);
	closesocket(cltSocket);
	closesocket(serverSocket);


	WSACleanup();

	return 0;
}

void ErrorHandling(char * message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}