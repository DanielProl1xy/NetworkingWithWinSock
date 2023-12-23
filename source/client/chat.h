#pragma once

#include "chatwidget.h"
#include <unordered_map>

enum ChatCommandKind
{
    CONNECT = 0,
    DISCONNECT,
    SET_NICK,
    COMMANDS_COUNT
};

const std::unordered_map<int, const char*> commands = {
    {ChatCommandKind::CONNECT, "connect"},
    {ChatCommandKind::DISCONNECT, "disconnect"},
    {ChatCommandKind::SET_NICK, "nickname"}
};

class ChatApplication
{

private:
    static ChatApplication *instance;
    ChatWidget *widget;
    TCPClient client;

public:
    static ChatApplication *GetInstance();

public:
    int Exec(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow);

private:
    int ProcessCommand(const char *command);
    size_t GetNextToken(const char *str, size_t begin, char *out_buff);
};