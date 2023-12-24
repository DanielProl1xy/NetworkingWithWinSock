#include "chat.h"
#include "dbg.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "chatwidget.lib")

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    DEBUG_START();

    ChatApplication *app = ChatApplication::GetInstance();
    int r = app->Exec(hInstance, hPrevInstance, pCmdLine, nCmdShow);

    DEBUG_FINISH();
    
    return r;
}