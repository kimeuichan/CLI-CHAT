#ifndef __CHAT_SERVER_H

#define __CHAT_SERVER_H

#pragma comment(lib, "Ws2_32.lib")

#define CHAT_PORT "55"
#define BUF_SIZE 1024
#define MSG_SIZE 40
#define ROOM_NAME_SIZE 100
#define MAX_ROOM_USER 5
#define MAX_CON_USER 10
#define MAX_ROOM_COUNT 5

#define ERROR_MESSAGE -1
#define INIT_MESSAGE 0
#define STRING_MESSAGE 1
#define EMOTICON_MESSAGE 2
#define JOIN_ROOM 3

typedef struct{
	int id;
	char name[100];
	int nameLen;
	int userCount;
} RoomInfo;

typedef struct{
	int id;
	int userCount;
	int cltSock[MAX_ROOM_USER];
} Room;

typedef struct{
	int sock;
	int roomId;
} User;

typedef struct{
	int type;
	char msg[MSG_SIZE];
	int msgLen;
} MESSAGE;

void ErrorHandling(char * message);
void Init();
void BroadCasetByRoomIndex(int index, MESSAGE* msg, int cltS);
void CompressUser(int index);
void MakeMessge(MESSAGE* message, int type, char* buf, int str_len);
int FindRoomIndexByCltS(int cltS);
void JoinRoom(int roomindex, int clts);

#endif