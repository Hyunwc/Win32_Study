#pragma once

#include "iDefine.h"

struct iColor4f;

struct iColor4b
{
	byte r, g, b, a;

	iColor4b& operator =(const iColor4f& c);
};

struct iColor4f
{
	float r, g, b, a;

	iColor4f& operator =(const iColor4b& c);
};

iColor4b iColor4bMake(byte r, byte g, byte b, byte a);
iColor4f iColor4fMake(float r, float g, float b, float a);

#define iColor4fRed iColor4fMake(1, 0, 0, 1)
#define iColor4fGreen iColor4fMake(0, 1, 0, 1)
#define iColor4fBlue iColor4fMake(0, 0, 1, 1)
#define iColor4fWhite iColor4fMake(1, 1, 1, 1)
#define iColor4fBlack iColor4fMake(0, 0, 0, 1)