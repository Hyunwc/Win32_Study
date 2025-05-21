#pragma once

#if _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif

//#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#include <Windows.h>

#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#define DEV_WIDTH 640

typedef unsigned char		uint8, byte;
typedef signed char			int8;
typedef unsigned short		uint16;
typedef signed short		int16;
typedef unsigned int		uint32;
typedef signed int			int32;

//typedef float				float32;
//typedef double				float64;

#define DEV_HEIGHT 480

enum iKeyStat
{
	iKeyStatBegan = 0,
	iKeyStatMoved,
	iKeyStatEnded,
	iKeyStatCancel
};

typedef void (*METHOD_VOID)();
typedef void (*METHOD_FLOAT)(float dt);
struct iPoint;
typedef void (*METHOD_KEY)(iKeyStat stat, iPoint point);

#define keydown_none 0// 00000000 00000000 00000000 00000000 
#define keydown_w    1// 00000000 00000000 00000000 00000001 
#define keydown_a    2// 00000000 00000000 00000000 00000010 
#define keydown_s    4// 00000000 00000000 00000000 00000100 
#define keydown_d    8// 00000000 00000000 00000000 00001000 
#define keydown_up   16// 00000000 00000000 00000000 00001000 
#define keydown_left  32// 00000000 00000000 00000000 00001000 
#define keydown_down   64// 00000000 00000000 00000000 00001000 
#define keydown_right   128// 00000000 00000000 00000000 00001000 
#define keydown_space 256
#define keydown_what   1<<31// 10000000 00000000 00000000 00000000