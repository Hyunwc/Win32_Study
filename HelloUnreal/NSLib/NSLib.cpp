#include "NSLib.h"

// display mode
//static nsInt dm = 0;
//static nsInt x, y, w, h;
//
//void nsInit(int* argc, char** argv)
//{
//}
//
//void nsInitDisplayMode(nsInt dm)
//{
//	::dm = dm;
//}
//
//void nsInitWindowSize(nsInt w, nsInt h)
//{
//	::w = w;
//	::h = h;
//}
//
//void nsInitWindowPosition(nsInt x, nsInt y)
//{
//	::x = x;
//	::y = y;
//}
//
//#include <Windows.h>
//DWORD dwExStyle, dwStyle;
//static void setWindowStyle(bool fullscreen, DWORD& dwExStyle, DWORD& dwStyle)
//{
//	if (fullscreen)
//	{
//		dwExStyle = WS_EX_APPWINDOW;
//		dwStyle = WS_POPUP;
//	}
//	else
//	{
//		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
//		dwStyle = WS_OVERLAPPEDWINDOW;
//	}
//}
//
//void nsCreateWindow(const char* szFormat, ...)
//{
//	wchar_t className[] = L"Hello";
//
//	HWND hWnd = CreateWindowEx(dwExStyle, className, className,
//		dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
//		x, y, w, h,
//		nullptr, nullptr, NULL, nullptr);
//}

int nsAdd(int a, int b)
{
	return a + b;
}