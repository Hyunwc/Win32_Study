#include "iOpenGL.h"

#include "iStd.h"

HWND hwnd;
HDC hdc;
HGLRC hrc; // 그래픽 라이브러리 리소스 // opengl 사용하기 위한 도구

// vao : vertex array object
uint32 vao, vbo, vbe;
Vertex* vertex;

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

	setMakeCurrent(true);

	glewExperimental = TRUE;
	GLenum error = glewInit();
	if (error != GLEW_OK)
		return;

	// 유효성 체크
	if (wglewIsSupported("WGL_ARB_create_context"))
	{
		setMakeCurrent(false);
		wglDeleteContext(hrc);

		int attr[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 2,
			WGL_CONTEXT_FLAGS_ARB, 0,
			0,
		};

		hrc = wglCreateContextAttribsARB(hdc, NULL, attr);
	}
	
	setMakeCurrent(true);

#if 1
	const char* strGL = (const char*)glGetString(GL_VERSION);
	const char* strGLEW = (const char*)glewGetString(GLEW_VERSION);
	const char* strGLSL = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	printf("OpenGL Version : %s\nGLEW Version : %s\nGLSL Version : %s\n",
		strGL, strGLEW, strGLSL);
#endif
	// ~ OpenGL Setting End

	// glew Setting Begin
	
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

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, NULL, GL_STATIC_DRAW);
	vertex = new Vertex[4];

	glGenBuffers(1, &vbe);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbe);
	uint8 indices[] = { 0, 1, 2,  2, 1, 3 };
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint8) * 6, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//fbo = new iFBO(devSize.width, devSize.height);
	fbo = new iFBO(1920, 1080);
	fbo->tex->width = devSize.width;
	fbo->tex->height = devSize.height;

	setMakeCurrent(false);
}

void freeOpenGL()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &vbe);

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
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrtho(0, devSize.width, devSize.height, 0, -1024, 1024);
	//
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();


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

uint32 iShader::compile(const char* strCode, int option)
{
	uint32 id = glCreateShader(option);
	glShaderSource(id, 1, &strCode, NULL);
	glCompileShader(id);
	checkShader(id);

	return id;
}

uint32 iShader::compileVert(const char* strCode)
{
	return compile(strCode, GL_VERTEX_SHADER);
}

uint32 iShader::compileFrag(const char* strCode)
{
	return compile(strCode, GL_FRAGMENT_SHADER);
}

void iShader::checkShader(uint32 id)
{
	GLint result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_TRUE)
		return;

	int length;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
	char* s = new char[1 + length];
	glGetShaderInfoLog(id, length, NULL, s);
	s[length] = 0;
	printf("checkShaderID Error!!\n(%s)\n", s);
	delete s;
}

void iShader::deleteShader(uint32 id)
{
	glDeleteShader(id);
}

void iShader::deleteProgram(uint32 id)
{
	glDeleteProgram(id);
}

uint32 iShader::link(uint32 vertID, uint32 fragID)
{
	uint32 id = glCreateProgram();
	glAttachShader(id, vertID);
	glAttachShader(id, fragID);
	glLinkProgram(id);
	glDetachShader(id, vertID);
	glDetachShader(id, fragID);
	checkProgram(id);

	return id;
}

void iShader::checkProgram(uint32 id)
{
	GLint result;
	glGetProgramiv(id, GL_LINK_STATUS, &result);
	if (result == GL_TRUE)
		return;

	int length;
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
	char* s = new char[1 + length];
	glGetProgramInfoLog(id, length, NULL, s);
	s[length] = 0;
	printf("checkProgramID Error!!\n(%s)\n", s);
	delete s;
}

uint32 iShader::build(const char* strVertex, const char* strFrag)
{
	uint32 vertID = compileVert(strVertex);
	uint32 fragID = compileFrag(strFrag);

	uint32 programID = link(vertID, fragID);

	deleteShader(vertID);
	deleteShader(fragID);

	return programID;
}

uint32 iShader::buildFromPath(const char* pathVertex, const char* pathFrag)
{
	int len;
	char* strVert = loadFile(len, pathVertex);
	char* strFrag = loadFile(len, pathFrag);
	uint32 programID = build(strVert, strFrag);
	delete strVert;
	delete strFrag;

	return programID;
}

uint32 iShader::buildShaderToy(const char* pathVertex, const char* pathFrag)
{
	int len;
	char* strVert = loadFile(len, pathVertex);
	char* strFrag = loadFile(len, pathFrag);

	const char* strBefore = "                   \n\
        #version 150                            \n\
                                                \n\
        #ifdef GL_ES                            \n\
        precision mediump float;                \n\
        #endif                                  \n\
                                                \n\
        uniform vec3      iResolution;          \n\
        uniform float     iTime;                \n\
        uniform float     iTimeDelta;           \n\
        uniform float     iFrameRate;           \n\
        uniform int       iFrame;               \n\
        uniform float     iChannelTime[4];      \n\
        uniform vec3      iChannelResolution[4];\n\
        uniform vec4      iMouse;               \n\
        uniform sampler2D iChannel0;            \n\
        uniform sampler2D iChannel1;            \n\
        uniform sampler2D iChannel2;            \n\
        uniform sampler2D iChannel3;            \n\
        uniform vec4      iDate;                \n\
        uniform float     iSampleRate;          \n\
                                                \n\
        out vec4 fragColor;                     \n";

#if 0 // strFrag 쉐이더토이 받은 파일
	void mainImage(out vec4 fragColor, in vec2 fragCoord)
	{

	}
#endif

	const char* strAfter = "						\
		void main()									\
		{											\
			mainImage(fragColor, gl_FragCoord.xy);  \
		}";

	int lenBefore = strlen(strBefore);
	int lenAfter = strlen(strAfter);
	int lenTotal = lenBefore + len + lenAfter;
	char* str = new char[lenTotal + 1];
	strcpy(str, strBefore);
	strcpy(&str[lenBefore], strFrag);
	strcpy(&str[lenBefore + len], strAfter);

	uint32 programID = build(strVert, str);
	delete strVert;
	delete strFrag;
	delete str;

	return programID;
}
