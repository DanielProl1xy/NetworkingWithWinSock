#pragma once

#pragma comment(lib, "gdi32.lib")

#include <vector>
#include "tcpclient.h"

#define MAX_MESSAGES_COUNT 124
#define MAX_BUFFER 4
#define BTN_SEND 0x42

struct InputBuffer
{
    char c_str[NetSize()];
};

class ChatWidget 
{

private:
    bool mVisible;
    bool mShouldClose;
    
    size_t message_last;
    NetMessage messages[MAX_MESSAGES_COUNT];
    size_t input_size;
    InputBuffer input_list[MAX_BUFFER];

private:
    static ChatWidget *instance;

private: // Win API
    WNDCLASS wc;
    HWND hWindow;
    // Window Components
    HWND hButton;
    HWND hEdit;
    HWND hMessages;

public:
    static ChatWidget *GetInstance();

private:
    ChatWidget();

public:
    ~ChatWidget();

public:
    int Init(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow);
    void Show();
    void Update();
    void Close();
    
    char *GetUserInput();

    void DisplayMessage(NetMessage message);
    void DisplayErrorMessage(const char *text);

public:
    bool ShouldClose() const { return mShouldClose; }
    bool IsVisible() const { return mVisible; }

public:
    static LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};