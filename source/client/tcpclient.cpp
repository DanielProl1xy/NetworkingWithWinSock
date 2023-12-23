
#include "tcpclient.h"
#include "string"
#include "dbg.h"

TCPClient::TCPClient()
{

}

TCPClient::~TCPClient()
{
    closesocket(client_socket);
}

void TCPClient::SetClientNick(const char *nick)
{
    strcpy(client_nick, nick);
}

int TCPClient::ConnectTo(const char *server_addr)
{
    info.sin_addr.s_addr = inet_addr(server_addr);

    server_socket = socket(info.sin_family, SOCK_STREAM, IPPROTO_TCP);
    int lr = connect(server_socket, (SOCKADDR*)&info, sizeof(info));

    if(lr != 0)
        return -1;

    isConnected = true;

    u_long mode = 1;
    ioctlsocket(server_socket, FIONBIO, &mode);
    ioctlsocket(client_socket, FIONBIO, &mode);

    return 0;
}

int TCPClient::InitClient()
{
    WSADATA ws;
    WSAStartup(MAKEWORD(2, 2), &ws);
    client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    info = {0};
    info.sin_family = AF_INET;
    info.sin_port = htons(NET_PORT);


    isConnected = false;
    return 0;
}

int TCPClient::SendNetMessage(const char *text)
{
    if(isConnected == false) return -1;

    return send(server_socket, text, strlen(text), 0);
}

int TCPClient::GetNetMessage(char *out_buff)
{
    if(isConnected == false) return -1;

    const int len = recv(server_socket, out_buff, MAX_MESSAGE_LEN, 0);

    if(len == WSAEWOULDBLOCK)
    {
        return -1;
    }

    return len;
}