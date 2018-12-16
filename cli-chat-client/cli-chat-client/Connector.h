#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__

#define CHAT_SERVER_ADDR "127.0.0.1"
#define CHAT_PORT 55
#define BUF_SIZE 1024
#define MSG_SIZE 40

#define ERROR_MESSAGE -1
#define INIT_MESSAGE 0
#define STRING_MESSAGE 1
#define EMOTICON_MESSAGE 2
#define JOIN_ROOM 3

#pragma comment(lib, "ws2_32.lib")


void InitClient();
void CloseClient();
void ErrorHandling(char * message);


typedef struct{
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

void MakeMessge(MESSAGE* message, int type, char* buf, int str_len);

#endif