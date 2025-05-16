#include "iRect.h"

iRect& iRect::operator += (const iPoint& p)
{
	//origin += p;
	origin.x += p.x;
	origin.y += p.y;
	return *this;
}

iRect& iRect::operator *= (float f)
{
	// size *= f;
	size.width *= f;
	size.height *= f;
	return *this;
}

iRect iRectMake(float x, float y, float width, float height)
{
	iRect rt;
	rt.origin.x = x;
	rt.origin.y = y;
	rt.size.width = width;
	rt.size.height = height;

	return rt;
}

bool containPoint(iPoint p, iRect rt)
{
#if 0
	if (p.x >= rt.origin.x &&
		p.x <= rt.origin.x + rt.size.width &&
		p.y >= rt.origin.y &&
		p.y <= rt.origin.y + rt.size.height)
		return true;
	return false;
#else
	if (p.x < rt.origin.x ||
		p.x > rt.origin.x + rt.size.width ||
		p.y < rt.origin.y ||
		p.y > rt.origin.y + rt.size.height)
		return false;
	return true;
#endif
}

bool containRect(iRect rt0, iRect rt1)
{
	if (rt0.origin.x + rt0.size.width < rt1.origin.x ||
		rt0.origin.x > rt1.origin.x + rt1.size.width ||
		rt0.origin.y + rt0.size.height < rt1.origin.y ||
		rt0.origin.y > rt1.origin.y + rt1.size.height)
		return false;
	return true;
}

float getDegree(float start, float end, float rate)
{
	return 0.0f;
}


