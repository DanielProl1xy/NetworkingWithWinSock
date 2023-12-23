
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
            char buff[MAX_MESSAGE_LEN] = "";
            const int len = recv(clients[i], buff, MAX_MESSAGE_LEN, 0);
            if(len == WSAEWOULDBLOCK)
            {
                continue;
            }
            else if(len > 0)
            {
                printf("%i recieved message: %s\n", len, buff);

                for(int y = 0; y < size; ++y)
                {
                    const int r = send(clients[y], buff, MAX_MESSAGE_LEN, 0);
                    if(r == SOCKET_ERROR)
                    {
                        printf("couldn't send a message %i\n", WSAGetLastError());
                    }
                }
            }
        }
        Sleep(16);
    }

CleanUp:
    WSACleanup();
    return 0;
}