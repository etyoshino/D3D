#include "window.h"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int mCmdShow)
{
	Window wnd(800, 600, "Hello Word");

	MSG msg;
	bool msgRes;
	while ((msgRes = GetMessage(&msg, nullptr, 0, 0)) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (msgRes==-1) {
		return -1;
	}
	return msg.wParam;
}