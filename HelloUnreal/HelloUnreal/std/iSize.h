#pragma once

struct iSize
{
	float width, height;

	iSize& operator *=(float f);
};

iSize iSizeMake(float width, float height);
#define iSizeZero iSizeMake(0, 0)
