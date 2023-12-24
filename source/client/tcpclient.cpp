
#include "tcpclient.h"
#include "string"
#include "dbg.h"

TCPClient::TCPClient()
{

}

TCPClient::~TCPClient()
{
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();
}

void TCPClient::SetClientNick(const char *nick)
{
    strcpy(client_nick, nick);
}

int TCPClient::ConnectTo(const char *server_addr)
{
    WSADATA ws;
    WSAStartup(MAKEWORD(2, 2), &ws);

    info = {0};
    info.sin_family = AF_INET;
    info.sin_port = htons(NET_PORT);

    isConnected = false;

    info.sin_addr.s_addr = inet_addr(server_addr);

    client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    server_socket = socket(info.sin_family, SOCK_STREAM, IPPROTO_TCP);
    
    int lr = connect(server_socket, (SOCKADDR*)&info, sizeof(info));

    if(lr != 0)
        return lr;

    isConnected = true;

    u_long mode = 1;
    ioctlsocket(server_socket, FIONBIO, &mode);
    ioctlsocket(client_socket, FIONBIO, &mode);

    return 0;
}

void TCPClient::Disconnect()
{
    closesocket(server_socket);
    closesocket(client_socket);
    shutdown(client_socket, SD_BOTH);
    WSACleanup();
    isConnected = false;
}

int TCPClient::SendNetMessage(const char *text)
{
    if(isConnected == false) return -1;

    NetMessage nsg = {0};
    strcpy(nsg.nick, client_nick);
    strcpy(nsg.text, text);

    char buff[NetSize()];

    if(SerializeNetMessage(&nsg, buff) != 0)
        return -1;

    return send(server_socket, buff, NetSize(), 0);
}

int TCPClient::GetNetMessage(NetMessage *out)
{
    if(isConnected == false) return -1;
    char buff[NetSize()];

    const int len = recv(server_socket, buff, NetSize(), 0);

    if(len == WSAEWOULDBLOCK)
    {
        return -1;
    }

    if(DeserializeNetMessage(buff, out) != 0)
    {
        return -2;
    }

    return len;
}

