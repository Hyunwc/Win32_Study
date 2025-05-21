#pragma once

#include "iDefine.h"

#include "iPoint.h"
#include "iRect.h"
#include "iColor.h"
#include "iSize.h"
#include "iString.h"

#include "iFPS.h"
#include "iSort.h"

extern int keydown;

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

void move(iPoint* cp, const iPoint* tp, const iPoint& mp);
