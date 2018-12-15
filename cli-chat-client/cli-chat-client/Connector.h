#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__

#define CHAT_SERVER_ADDR "127.0.0.1"
#define CHAT_PORT 55
#define BUF_SIZE 1024
#define MSG_SIZE 40

#pragma comment(lib, "ws2_32.lib")


void InitClient();
void CloseClient();
void ErrorHandling(char * message);


typedef struct{
	int roomId;
	int type;
	char msg[MSG_SIZE];
	int msgLen;
} MESSAGE;

typedef struct{
	int id;
	char name[100];
	int nameLen;
	int userCount;
} RoomInfo;

#endif