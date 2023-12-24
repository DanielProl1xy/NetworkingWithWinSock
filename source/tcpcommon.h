#pragma once

#define MAX_MESSAGE_LEN 512
#define MAX_COMMAND_SIZE 32
#define MAX_FLAG_SIZE 32
#define JSON_SIZE 8

#define NET_PORT 7894

#include <memory>

struct NetMessage
{
    char text[MAX_MESSAGE_LEN];
    char nick[MAX_FLAG_SIZE];
};

static constexpr size_t NetSize()
{ 
    return sizeof(NetMessage); 
}

static int SerializeNetMessage(NetMessage *msg, char *out_buff)
{
    if(memcpy(out_buff, msg, NetSize()) == nullptr)
    {
        return -1;
    }
    return 0;
}

static int DeserializeNetMessage(const char *in_buff, NetMessage *out)
{   
    if(memcpy(out, in_buff, NetSize()) == nullptr)
    {
        return -1;
    }
    return 0;
}