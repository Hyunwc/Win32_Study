#pragma once

#include "iDefine.h"

extern HWND hwnd;
extern HDC hdc;
extern HGLRC hrc;

extern uint32 vao, vbo, vbe;

void loadOpenGL(HWND hwnd);
void freeOpenGL();
void resizeOpenGL(int width, int height);

void swapBuffer();
void setMakeCurrent(bool enable);

struct Vertex
{
	float position[4];
	float color[4];
	float texCoord[2];
	//float normal[4];

};

extern Vertex* vertex;

// 프레임버퍼 
class iFBO
{
public:
	iFBO(int width, int height);
	virtual ~iFBO();

	static Texture* createImage(int width, int height);

	void bind();
private:
	void _bind(Texture* tex);
public:
	void bind(Texture* tex);
	void unbind();

	uint32 fbo;
	uint32 depthBuffer;
	Texture* tex;

	Texture** texPrev;
	int numPrev;
};

extern iFBO* fbo;