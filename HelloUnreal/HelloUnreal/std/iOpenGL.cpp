#include "iOpenGL.h"

HWND hwnd;
HDC hdc;
HGLRC hrc; // 그래픽 라이브러리 리소스 // opengl 사용하기 위한 도구

void loadOpenGL(HWND hwnd)
{
	// 전역변수로 사용되는 hwnd에 파라미터 대입
	::hwnd = hwnd;
	// ~ OpenGL Setting Begin
	hdc = GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0x00, sizeof(PIXELFORMATDESCRIPTOR)); // 0으로 초기화
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int pixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, pixelFormat, &pfd);
	hrc = wglCreateContext(hdc);
	// ~ OpenGL Setting End

	setMakeCurrent(true);

	glEnable(GL_BLEND); // 알파 블렌드를 섞겠다.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_LINE_SMOOTH);

	setMakeCurrent(false);
}

void freeOpenGL()
{
	wglMakeCurrent(NULL, NULL); // 사용x
	wglDeleteContext(hrc);
	ReleaseDC(hwnd, hdc);
	DestroyWindow(hwnd); // 윈도우 삭제
}

#include "iRect.h"
extern iRect viewport;
extern iSize devSize;
void resizeOpenGL(int width, int height)
{
	glViewport(viewport.origin.x, viewport.origin.y,
		viewport.size.width, viewport.size.height);

	// ~ 해상도 지정
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, devSize.width, devSize.height, 0, -1024, 1024);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// ~ 해상도 지정
}

void swapBuffer()
{
	// 버퍼 교체 (더블 버퍼링)
	SwapBuffers(hdc);
}

void setMakeCurrent(bool enable)
{
	if (enable)
		wglMakeCurrent(hdc, hrc); // OpenGL을 사용하겠다
	else
		wglMakeCurrent(NULL, NULL); // 사용해제
}
