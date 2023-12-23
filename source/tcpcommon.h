#pragma once



#define MAX_MESSAGE_LEN 512
#define MAX_COMMAND_SIZE 32
#define MAX_FLAG_SIZE 32
#define JSON_SIZE 8

#define NET_PORT 7894

struct NetMessage
{
    char text[MAX_MESSAGE_LEN];
    char nick[MAX_FLAG_SIZE] = "User";
};

// int SerializeNetMessage(const NetMessage msg, char *out_buff);

// int DeserializeNetMessage(const char *in_buff, NetMessage *out);