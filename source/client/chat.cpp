#include "chat.h"
#include "dbg.h"
#include "unordered_map"
#include "ctype.h"

ChatApplication *ChatApplication::instance = nullptr;

ChatApplication* ChatApplication::GetInstance()
{
    if(instance)
        return instance;

    instance = new ChatApplication();

    return instance;
}

size_t ChatApplication::GetNextToken(const char *str, size_t begin, char *out_buff)
{
    for(size_t i = begin; i < strlen(str); ++i)
    {
        if(isgraph(str[i]))
        {
            strncat(out_buff, &str[i], 1);
        }
        else if(isspace(str[i]))
        {
            strcat(out_buff, "\0");
            return i - begin;
            break;
        }
        else break;
    }
    return -1;
}

int ChatApplication::ProcessCommand(const char *command)
{
    char buf[MAX_COMMAND_SIZE] = "";
    size_t buf_len = GetNextToken(command, 1, buf);
    char flag[MAX_FLAG_SIZE] = "";

    if(strcmp(buf, commands.at(CONNECT)) == 0)
    {
        size_t flag_len = GetNextToken(command, buf_len + 2, flag);
        DEBUG_ARGS("COMMAND: Connect to %s\n", flag);
        int err = client.ConnectTo(flag);
        if(err != 0)
        {
            widget->DisplayErrorMessage("Couldn't connect for some reason :<");
            return 0;
        }
        widget->SetStatus(StatusKind::CONNECTED);
        return 0;
    }
    else if(strcmp(buf, commands.at(DISCONNECT)) == 0)
    {
        DEBUG("COMMAND: Disconnect\n");
        client.Disconnect();
        widget->SetStatus(StatusKind::DISCONNECTED);
        return 0;
    }
    else if(strcmp(buf, commands.at(SET_NICK)) == 0)
    {
        size_t flag_len = GetNextToken(command, buf_len + 2, flag);
        if(flag_len > 0)
        {
            client.SetClientNick(flag);
            DEBUG_ARGS("COMMAND: Set nickname to %s\n", flag);
        }
        return 0;
    }

    return -1;
}

int ChatApplication::Exec(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    widget = ChatWidget::GetInstance();
    if(widget->Init(hInstance, hPrevInstance, pCmdLine, nCmdShow) < 0)
    {
        return -1;
    }
    widget->Show();

    widget->SetStatus(StatusKind::DISCONNECTED);
    client = TCPClient();

    while(!widget->ShouldClose())
    {
        widget->Update();

        NetMessage recv = {0};
        if(client.GetNetMessage(&recv) > 0)
        {
            widget->DisplayNetMessage(recv);
        }

        char *usr_input = widget->GetUserInput();
        if(usr_input)
        {
            if(usr_input[0] == '/')
            {
                if(ProcessCommand(usr_input) != 0)
                {
                    widget->DisplayErrorMessage("Unkown command :<");
                }
            }
            else
            {
                const int err = client.SendNetMessage(usr_input);
                if(err == SOCKET_ERROR)
                {
                    client.Disconnect();
                    widget->SetStatus(StatusKind::DISCONNECTED);
                    widget->DisplayErrorMessage("Couldn't send a message, probably you are not connected ;<");
                }
            }
        }
    }

    client.Disconnect();
    widget->Close();
    
    free(widget);

    return 0;
}