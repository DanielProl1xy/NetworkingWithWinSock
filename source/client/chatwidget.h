#pragma once

#pragma comment(lib, "gdi32.lib")

#include <unordered_map>
#include <vector>
#include "tcpclient.h"

#define MAX_MESSAGES_COUNT 5
#define MAX_BUFFER 4
#define BTN_SEND 0x42

enum StatusKind
{
    DISCONNECTED,
    CONNECTED,
    STATUS_COUNT
};

typedef size_t Status;

const std::unordered_map<int, const char*> status_messages = {
    {StatusKind::DISCONNECTED, "Disconnected"},
    {StatusKind::CONNECTED, "Connected"}
};

struct InputBuffer
{
    char c_str[MAX_MESSAGE_LEN];
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

    Status n_status;

private:
    static ChatWidget *instance;

private: // Win API
    WNDCLASS wc;
    HWND hWindow;
    // Window Components
    HWND hButton;
    HWND hEdit;
    HWND hMessages;
    HWND hStatusBar;

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
    
    // Returns one line of input buffer and ereases it
    char *GetUserInput();

    void DisplayNetMessage(NetMessage message);
    void DisplayErrorMessage(const char *text);

public:
    bool ShouldClose() const { return mShouldClose; }
    bool IsVisible() const { return mVisible; }
    void SetStatus(Status st);
    void SetWindowTitle(const char *title) { SetWindowTextA(hWindow, title); }

public:
    static LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};