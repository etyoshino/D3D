#include "App.h"


App::App()
    :
    wnd(800,600,"Hello Word")
{}

int App::Go()
{
    MSG msg;
    BOOL gResult;
    while (gResult = GetMessage(&msg, nullptr, 0, 0) > 0)
    {
        // TranslateMessage will post auxilliary WM_CHAR message from key msgs
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        DoFrame();
    }

    //check if GetMessage call itself borked
    if (gResult == -1)
    {
        throw CHWND_LAST_EXCEPT();
    }
    return msg.wParam;
}

void App::DoFrame()
{

}
