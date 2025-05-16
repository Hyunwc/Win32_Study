#pragma once

#if _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif

#include <Windows.h>

#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")

#include "iPoint.h"
#include "iRect.h"

enum iKeyStat
{
	iKeyStatBegan = 0,
	iKeyStatMoved,
	iKeyStatEnded,
	iKeyStatCancel
};

typedef void (*METHOD_VOID)();
typedef void (*METHOD_FLOAT)(float dt);
typedef void (*METHOD_KEY)(iKeyStat stat, iPoint point);

extern int keydown;
#define keydown_none 0// 00000000 00000000 00000000 00000000 
#define keydown_w    1// 00000000 00000000 00000000 00000001 
#define keydown_a    2// 00000000 00000000 00000000 00000010 
#define keydown_s    4// 00000000 00000000 00000000 00000100 
#define keydown_d    8// 00000000 00000000 00000000 00001000 
#define keydown_up   16// 00000000 00000000 00000000 00001000 
#define keydown_left  32// 00000000 00000000 00000000 00001000 
#define keydown_down   64// 00000000 00000000 00000000 00001000 
#define keydown_right   128// 00000000 00000000 00000000 00001000 
#define keydown_what   1<<31// 10000000 00000000 00000000 00000000


void loadApp(HWND hWnd, METHOD_VOID load, METHOD_VOID free,
	METHOD_FLOAT draw, METHOD_KEY key);
void freeApp();
void drawApp(float dt);
void keyApp(iKeyStat stat, iPoint point);

void setRGBA(float r, float g, float b, float a);

void clear();

void drawLine(float x0, float y0, float x1, float y1);
void drawLine(iPoint p0, iPoint p1);

void drawRect(float x, float y, float width, float height);
void drawRect(iRect rt);

void fillRect(float x, float y, float width, float height);
void fillRect(iRect rt);

void setStringSize(float size);
void setStringRGBA(float r, float g, float b, float a);
void drawString(float x, float y, const WCHAR* str);

// 시작과 끝값
float linear(float s, float e, float rate);
iPoint linear(iPoint s, iPoint e, float rate);

float easeln(float s, float e, float rate);
iPoint easeln(iPoint s, iPoint e, float rate);

float easeOut(float s, float e, float rate);
iPoint easeOut(iPoint s, iPoint e, float rate);

