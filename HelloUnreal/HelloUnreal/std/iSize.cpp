#include "iSize.h"

iSize& iSize::operator *= (float f)
{
	width *= f;
	height *= f;

	return *this;
}

iSize iSizeMake(float width, float height)
{
	iSize s;
	s.width = width;
	s.height = height;
	return s;
}
