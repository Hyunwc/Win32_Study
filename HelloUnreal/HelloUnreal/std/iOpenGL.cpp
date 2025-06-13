#include "iOpenGL.h"

#include "iStd.h"

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

	// glew Setting Begin
	glewExperimental = TRUE;
	GLenum error = glewInit();
	if (error != GLEW_OK)
		return;
	// glew Setting End

	glEnable(GL_BLEND); // 알파 블렌드를 섞겠다.

	
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Input Color.rgb = DST.rgb * (1.0 - SRC.alpha) + SRC.rgb* SRC.alpha
	// Input Color.a = DST.a * (1.0 - SRC.alpha) + SRC.alpha * SRC.alpha
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	// DST(background) SRC(image)
	// Input Color.rgb = DST.rgb * (1.0 - SRC.alpha) + SRC.rgb* SRC.alpha
	// Input Color.a = DST.a * (1.0 - SRC.alpha) + SRC.alpha
	
	// frame buffer object binding texture...
	// Input Color.rgb = DST.rgb * (1.0 - SRC.alpha) + SRC.rgb // (SRC.rgb * SRC.alpha)
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_LINE_SMOOTH);

	//fbo = new iFBO(devSize.width, devSize.height);
	fbo = new iFBO(1920, 1080);
	fbo->tex->width = devSize.width;
	fbo->tex->height = devSize.height;

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


	//glPushMatrix(); // backup
	////float m[4][4]; // == m[16]
	////glGetFloatv(GL_MODELVIEW_MATRIX, &m[0][0]); // backup
	//glTranslatef(200, 0, 0);

	//float n[4][4]; // == m[16]
	//glGetFloatv(GL_MODELVIEW_MATRIX, &n[0][0]);

	//glPopMatrix(); // restore
	////glLoadMatrixf(&m[0][0]); // restore
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

iFBO* fbo;

Texture* iFBO::createImage(int width, int height)
{
	uint32 texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	// 안티 알리아싱
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// 색상 그라데이션
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NEAREST

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	Texture* tex = new Texture;
	tex->texID = texID;
	tex->width = width;
	tex->height = height;
	tex->potWidth = width;
	tex->potHeight = height;
	tex->retainCount = 1;

	return tex;
}

iFBO::iFBO(int width, int height)
{
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	tex = createImage(width, height);

	glGenFramebuffers(1, &fbo); // 프레임버퍼 생성
	glBindFramebuffer(GL_FRAMEBUFFER, fbo); // 바인딩

	GLenum fboBuffs[1] = { GL_COLOR_ATTACHMENT0 };
	//glDrawBuffer(fboBuffs[0]);
	glDrawBuffers(1, fboBuffs);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0
		, GL_TEXTURE_2D, tex->texID, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	// 해제
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	texPrev = new Texture * [10];
	numPrev = 0;
}

iFBO::~iFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &fbo);

	glDeleteRenderbuffers(1, &depthBuffer);
	freeImage(tex);

	delete texPrev;
}

void iFBO::bind()
{
	bind(tex);
}

void iFBO::_bind(Texture* tex)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo); // fbo screen

	GLenum fboBuffs[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, fboBuffs);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0
		, GL_TEXTURE_2D, tex->texID, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	glViewport(0, 0, tex->width, tex->height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, tex->width, tex->height, 0, -1024, 1024);
}

void iFBO::bind(Texture* tex)
{
	_bind(tex);

	texPrev[numPrev] = tex;
	numPrev++;
}

// 원래대로 다시 설정
void iFBO::unbind()
{
	numPrev--;
	if (numPrev)
	{
		_bind(texPrev[numPrev - 1]);
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // read screen

		glViewport(viewport.origin.x, viewport.origin.y,
			viewport.size.width, viewport.size.height);

		// ~ 해상도 지정
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, devSize.width, devSize.height, 0, -1024, 1024);
	}
}
