#include "Window.h"
#include "WindowsThrowMacros.h"
#include "resource.h"
#include <sstream>

// Window Class Stuff
Window::WindowClass Window::WindowClass::wndClass;

const char* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}

Window::WindowClass::WindowClass()noexcept
	:
	hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetUp;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(
		LoadImage(hInst, MAKEINTRESOURCE(IDI_APPICON),
			IMAGE_ICON, 16, 16, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>(
		LoadImage(hInst, MAKEINTRESOURCE(IDI_APPICON),
			IMAGE_ICON, 16, 16, 0));;
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, GetInstance());
}

Window::Window(int width, int height, const char* name) noexcept
	:
	width(width),
	height(height)
{
	// 设置窗口位置大小
	RECT wr;
	wr.left = 200;
	wr.right = width + wr.left;
	wr.top = 200;
	wr.bottom = wr.top + height;
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
	{
		throw CHWND_LAST_EXCEPT();
	}
	// 创建窗口拿到窗口实例
	hWnd = CreateWindow(
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);
	// 检查是否拿到实例
	if (hWnd == nullptr)
	{
		throw CHWND_LAST_EXCEPT();
	}

	// 设置窗口默认显示隐藏
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	// 创建 Graphics 实体
	pGfx = std::make_unique<Graphics>(hWnd);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

void Window::SetTitle(const std::string& title)
{
	if (SetWindowText(hWnd, title.c_str()) == 0)
	{
		throw CHWND_LAST_EXCEPT();
	}
}

std::optional<int> Window::ProcessMessages() noexcept
{
	MSG msg;
	// 当队列中有消息时派发执行事件
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		//检查是否时退出消息，这个消息无法被定义的HandleMsgSetUp捕获。所以需要提前拦截
		// 详解https://blog.csdn.net/yuanhubilie/article/details/8555677
		if (msg.message == WM_QUIT) {
			//  返回退出码
			return msg.wParam;
		}

		//TranslateMessage 将虚拟键消息转换为字符消息
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return {};
}

Graphics& Window::Gfx()
{
	return *pGfx;
}

LRESULT Window::HandleMsgSetUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//　用户在创建窗口时传入的一个处理消息的方法，存储在winAPI的窗口类指针
	if (msg == WM_NCCREATE) {	//NC--->>non-client非客户区包括标题栏、窗口边框、最大、最小按钮、滚动条等
		//　从创建的数据中获得窗口的指针
		const CREATESTRUCTW* pCreateW = reinterpret_cast<CREATESTRUCTW*>(lParam);
		//lpParam指向通过CREATESTRUCT结构(lpCreateParams成员)传递给窗口的值的指针
		//通过WM_CREATE消息的lParam参数。此消息在返回之前由该函数发送到创建的窗口
		Window* const pWnd = static_cast<Window*>(pCreateW->lpCreateParams);
		// 设置WinAPI-managed的用户数据窗口实例
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// 设置消息进程的处理方式
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	//if we get a message before the WM_NCCREATE message ,handle with default handle
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// retrieve ptr to window instance
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window instance handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;

		/*************** KEYBOARD MESSAGE ******************/
	case WM_KEYDOWN:
		// syskey commands need to be handled to track ALT key (VK_MENU) and F10
	case WM_SYSKEYDOWN:
	{
		if (!(lParam & 0x40000000) || kbd.AutoRepeatIsEnabled()) {
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	}
	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	}
	case WM_CHAR:
	{
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;
	}
	/*************** KEYBOARD MESSAGE ******************/

	/***************** Mouse MESSAGE ********************/
	case WM_MOUSEMOVE:
	{
		POINTS pt = MAKEPOINTS(lParam);
		// 鼠标在客户端区域内
		if (pt.x >= 0 && pt.x < width && pt.y>0 && pt.y < height) {
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow()) {
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		// 鼠标不在客户端区域内
		else
		{
			// 捕获是否有鼠标事件，更新鼠标位置
			if (wParam & (MK_LBUTTON | MK_RBUTTON)) {
				mouse.OnMouseMove(pt.x, pt.y);
			}
			// button up ->release capture / log event for leaving
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}
	case WM_MBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnMidPressed(pt.x, pt.y);
		break;
	}
	case WM_MBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnMidReleased(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	/***************** Mouse MESSAGE ********************/
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// Window Exception Stuff
std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	// windows 将为错误消息分配内存，并使指针指向它
	const DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);
	// 获取错误结果失败
	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}
	// 拿出windos-allocated buffer中的错误信息
	std::string errorString = pMsgBuf;
	// 释放 buffer
	LocalFree(pMsgBuf);
	return errorString;
}

Window::HrException::HrException(int line, const char* file, HRESULT hr) noexcept
	:
	Exception(line, file),
	hr(hr)
{}

const char* Window::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::HrException::GetType() const noexcept
{
	return "Window Exception";
}

HRESULT Window::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::HrException::GetErrorDescription() const noexcept
{
	return Exception::TranslateErrorCode(hr);
}

const char* Window::NoGfxException::GetType() const noexcept
{
	return "Window Exception [No Graphics]";
}