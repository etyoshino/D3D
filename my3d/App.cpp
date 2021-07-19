#include "App.h"
#include<sstream>
#include<iomanip>

App::App()
	:
	wnd(800, 600, "Hello Word")
{}

int App::Go()
{
	while (true)
	{
		// prcess all messages pending, but to not block for new messages
		if (const auto ecode = Window::ProcessMessages()) 
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		DoFrame();
	}
}
 
void App::DoFrame()
{
	wnd.Gfx().EndFrame();
}
