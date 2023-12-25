#include "chatwidget.h"
#include <string>

#pragma comment(lib, "user32.lib")


int window_width = 600;
int window_height = 600;
int pad = 25;
int button_width = 120;
int button_height = 30;

ChatWidget *ChatWidget::instance = nullptr;


ChatWidget *ChatWidget::GetInstance()
{
    if(instance)
        return instance;

    instance = new ChatWidget();
    return instance;
}

ChatWidget::ChatWidget()
{
    mVisible = false;
    mShouldClose = false;
    message_last = 0;
    input_size = 0;
    n_status = 0;
}

ChatWidget::~ChatWidget()
{

}

int ChatWidget::Init(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    wc = { };
    
    wc.lpfnWndProc = windowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("Sample window class");

    RegisterClass(&wc);

    hWindow = CreateWindowEx(
        0,                              // Optional window styles.
        wc.lpszClassName,              // Window class
        NULL,       // Window text
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,            // Window style

        // Size and position
        200, 200, window_width, window_height,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );


    if(hWindow == NULL)
    {
        return -1;
    }

    SetWindowTextA(hWindow, "Client chat");
    
    hButton = CreateWindow(TEXT("BUTTON"),  // Predefined class; Unicode assumed 
                        TEXT("Send message"),      // Button text 
                        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles 
                        window_width - (button_width + pad * 1.5),         // x position 
                        window_height - (button_height + pad * 2),         // x position 
                        button_width,        // Button width
                        button_height,        // Button height
                        hWindow,     // Parent window
                        (HMENU)BTN_SEND,       // No menu.
                        hInstance, 
                        NULL); 

     hEdit = CreateWindow(TEXT("EDIT"),  // Predefined class; Unicode assumed 
                        TEXT("Send message"),      // Button text 
                        WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER,  // Styles 
                        pad,         // x position 
                        window_height - (button_height + pad * 2),         // y position 
                        window_width - button_width - pad * 3,        // Button width
                        button_height,        // Button height
                        hWindow,     // Parent window
                        NULL,       // No menu.
                        hInstance, 
                        NULL); 

    hMessages = CreateWindow(TEXT("STATIC"),
                            TEXT(""),
                            WS_TABSTOP | WS_VISIBLE | WS_CHILD,
                            pad,
                            pad * 2,
                            window_width - pad * 3,
                            window_height - button_height - pad * 5,
                            hWindow,
                            NULL,
                            hInstance,
                            NULL);

    hStatusBar = CreateWindow(TEXT("STATIC"),
                            TEXT(""),
                            WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER,
                            pad,
                            pad,
                            window_width - pad * 3,
                            pad,
                            hWindow,
                            NULL,
                            hInstance,
                            NULL);

    return 0;

}

void ChatWidget::Show()
{
    mVisible = true;
    mShouldClose = false;

    ShowWindow(hWindow, 1);
}

void ChatWidget::Close()
{
    DestroyWindow(hWindow);
}

char *ChatWidget::GetUserInput()
{
    if(input_size > 0)
    {
        input_size -= 1;
        return input_list[input_size].c_str;
    }
    else return NULL;
}

void ChatWidget::DisplayNetMessage(NetMessage msg)
{
    // [1] [2] [3] [ ] null
    //              ^last ^lest++
    // [2] [3] [4] [ ] null
    //              ^last ^last++

    messages[message_last] = msg;
    message_last++;

    char all_msg[MAX_MESSAGE_LEN * MAX_MESSAGES_COUNT + MAX_FLAG_SIZE * MAX_MESSAGES_COUNT] = "";

    for(int i = 0; i < message_last; ++i)
    {
        const char *msg = messages[i].text;
        const char *nick = messages[i].nick;
        strcat(all_msg, nick);
        strcat(all_msg, ": ");
        strcat(all_msg, msg);
        strcat(all_msg, "\r\n");
    }

    SetWindowTextA(hMessages, all_msg);

    if(message_last > MAX_MESSAGES_COUNT - 1)
    {
        message_last--;

        memmove(messages, &messages[1], sizeof(messages[0]) * MAX_MESSAGES_COUNT - 1);
    }

}

void ChatWidget::DisplayErrorMessage(const char *message)
{
    MessageBoxA(hWindow, message, "Error", MB_OK | MB_ICONERROR);
}

void ChatWidget::Update()
{
    MSG msg = { };
    if(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    else
    {
        mShouldClose = true;
    }
    SendMessage(hWindow, WM_PAINT, NULL, NULL);
    Sleep(16);
}

void ChatWidget::SetStatus(Status st) 
{ 
    if(st < StatusKind::STATUS_COUNT)
    {
        n_status = st; 
        char c_status[MAX_FLAG_SIZE];
        sprintf(c_status, "Status: %s", status_messages.at(n_status));
        SetWindowTextA(hStatusBar, c_status);
    }
}

LRESULT CALLBACK ChatWidget::windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ChatWidget *wnd = instance; // TEMP

    switch (uMsg)
    {
    case WM_COMMAND:
        switch(wParam)
        {
            case BTN_SEND:

                int len = GetWindowTextLengthA(wnd->hEdit);

                if(len <= 1) return 0;

                if(len > MAX_MESSAGE_LEN)
                {
                    wnd->DisplayErrorMessage("Message is to long!");
                    return 0;
                }

                char usr_message[MAX_MESSAGE_LEN];

                GetWindowTextA(wnd->hEdit, usr_message, len + 1);
                SetWindowTextA(wnd->hEdit, "");

                strcpy(wnd->input_list[wnd->input_size].c_str, usr_message);                
                wnd->input_size = (wnd->input_size + 1) % MAX_BUFFER;

                return 0;
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        {
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        }
        EndPaint(hwnd, &ps);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
