
#pragma comment (lib, "ws2_32")

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include "../tcpcommon.h"

#include "stdio.h"
#include <vector>


bool isListening;
std::vector<SOCKET> clients;

void DisconnectClient(int id);
static void SendServerRawMessage(const char *buff) noexcept;
static void SendServerMessage(const char *text) noexcept;

int main()
{
    WSADATA ws;
    WSAStartup(MAKEWORD(2, 2), &ws);
    isListening = false;
    printf("Starting server\n");
    SOCKET serv_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN info = {0};
    info.sin_family = AF_INET;
    info.sin_port = htons(NET_PORT);

    bind(serv_socket, (SOCKADDR *)&info, sizeof(info));
    int rl = listen(serv_socket, SOMAXCONN);
    if(rl != 0)
    {
        printf("failed at listen(): %i\n", WSAGetLastError());
        goto CleanUp;
    }

    u_long mode = 1;
    ioctlsocket(serv_socket, FIONBIO, &mode);

    printf("server is listening at %s\n", inet_ntoa(info.sin_addr));

    isListening = true;

    while(isListening)
    {
        SOCKET client = accept(serv_socket, NULL, NULL);
        if(client != INVALID_SOCKET)
        {
            ioctlsocket(client, FIONBIO, &mode);
            clients.push_back(client);
            SOCKADDR_IN client_info = {0};
            getpeername(client, (SOCKADDR *)&client_info, NULL);
            printf("accepted connection from %s\n", inet_ntoa(client_info.sin_addr));
        }

        size_t size = clients.size();
        for(int i = 0; i < size; ++i)
        {
            char buff[NetSize()] = "";
            const int len = recv(clients[i], buff, NetSize(), 0);
            if(len == SOCKET_ERROR)
            {
                const int err = WSAGetLastError();
                if(err == WSAECONNRESET)
                {
                    DisconnectClient(i);
                    size--;
                    i--;
                }
            }
            else if(len == WSAEWOULDBLOCK)
            {
                continue;
            }
            else if(len > 0)
            {
                printf("%i recieved message: %s\n", len, buff);
                SendServerRawMessage(buff);
            }
        }
        Sleep(16);
    }

CleanUp:
    WSACleanup();
    return 0;
}

void DisconnectClient(int id)
{
    closesocket(clients[id]);
    clients.erase(clients.begin() + id);
    printf("Client disconnected\n");
    SendServerMessage("Client disconnected");
}

static void SendServerRawMessage(const char *buff) noexcept
{
    size_t size = clients.size();
    for(int y = 0; y < size; ++y)
    {
        const int err = send(clients[y], buff, NetSize(), 0);
        if(err == WSAECONNRESET)
        {
            DisconnectClient(y);
            size--;
            y--;
        }
    }
}

static void SendServerMessage(const char *text) noexcept
{
    NetMessage msg;
    char buff[NetSize()];
    strcpy(msg.nick, "SERVER");
    strcpy(msg.text, text);

    SerializeNetMessage(&msg, buff);

    SendServerRawMessage(buff);
}