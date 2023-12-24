#pragma once
#pragma comment (lib, "ws2_32")

#include "../tcpcommon.h"

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

class TCPClient
{

private:
    char client_nick[MAX_FLAG_SIZE] = "DefUser";

    SOCKET server_socket;
    SOCKET client_socket;
    SOCKADDR_IN info;

    bool isConnected;

public:
    TCPClient();
    ~TCPClient();

public:

    void SetClientNick(const char *nick);

    int ConnectTo(const char *server_addr);

    void Disconnect();

    int SendNetMessage(const char *text);

    int GetNetMessage(NetMessage *out);
};