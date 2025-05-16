#pragma once

typedef unsigned char byte;

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

