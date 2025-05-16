#include "iColor.h"

iColor4b& iColor4b::operator =(const iColor4f& c)
{
	r = (byte)(c.r * 0xFF);
	g = (byte)(c.g * 0xFF);
	b = (byte)(c.b * 0xFF);
	a = (byte)(c.a * 0xFF);
	return *this;
}

iColor4f& iColor4f::operator =(const iColor4b& c)
{
	r = c.r / 255.f;
	g = c.g / 255.f;
	b = c.b / 255.f;
	a = c.a / 255.f;
	return *this;
}

iColor4b iColor4bMake(byte r, byte g, byte b, byte a)
{
	iColor4b c;
	c.r = r;
	c.g = g;
	c.b = b;
	c.a = a;
	return c;
}

iColor4f iColor4fMake(float r, float g, float b, float a)
{
	iColor4f c;
	c.r = r;
	c.g = g;
	c.b = b;
	c.a = a;
	return c;
}
