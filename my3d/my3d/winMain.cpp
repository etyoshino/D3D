#include <windows.h>
#include "WindowsMessageMap.h"

LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	static WindowsMessageMap wmm;
	OutputDebugString(wmm(msg, lParam, wParam).c_str());
	switch (msg)
	{
		case WM_CLOSE:
			PostQuitMessage(69);
			break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int mCmdShow)
{
	const auto pClassName = "myd3d";
	// register window class
	WNDCLASSEX wc = {0};
	wc.cbSize = sizeof( wc );
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	wc.hIconSm = nullptr;
	RegisterClassEx( & wc); 
	// create window instance
	HWND hWnd = CreateWindowEx(
		0, pClassName,
		"HelloWord", WS_CAPTION | WS_VSCROLL | WS_BORDER | WS_MINIMIZEBOX| WS_MAXIMIZEBOX | WS_SYSMENU,
		200, 200, 640, 480,
		nullptr, nullptr, hInstance, nullptr
	);
	ShowWindow(hWnd, SW_SHOW);

	MSG msg;
	bool msgRes;
	while ((msgRes = GetMessage(&msg, hWnd, 0, 0)) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (msgRes==-1) {
		return -1;
	}
	else {
		return msg.wParam;
	}
}