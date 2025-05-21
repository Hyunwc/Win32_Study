#include "Game.h"

#include <math.h>
#include <stdio.h>
iRect me;
iPoint tp;
float takeTime;

#include "Lotto.h"
#include "Tripple.h"
#include "AirShooting.h"

void loadGame()
{
	me = iRectMake(0, 0, 50, 50);
	takeTime = 0.0f;

#if 0
	loadLotto();
#elif 0
	loadTripple();
#else
	loadAirShootng();
#endif
}

void freeGame()
{
#if 0
	//freeLotto();
#elif 0
	freeTripple();
#else
	freeAirShootng();
#endif
}

void drawGame(float dt)
{
	setRGBA(0, 0, 0, 1);
	clear();

#if 0
	drawLotto(dt);
	return;
#elif 0
	drawTripple(dt);
	return;
#else
	drawAirShootng(dt);
	return;
#endif
	drawString(300, 100, L"Hi");

	takeTime += dt;

	//float dx = sin(takeTime) * 50;

	setRGBA(1, 0, 0, 1);
	//drawLine(50 + dx, 0, 150 + dx, 100);
	fillRect(me);

#define move_speed 300
#if 0// ctrl
	extern int keydown;
	iPoint v = iPointZero;
	if (keydown & keydown_a)		v.x = -1;
	else if (keydown & keydown_d)	v.x = 1;

	if (keydown & keydown_w)		v.y = -1;
	else if (keydown & keydown_s)	v.y = 1;

	//if (v != iPointZero)
	//	v /= iPointLength(v);
	v.loadIdentity();

	me.origin += v * move_speed * dt;

#else
	if (me.origin != tp)
	{
		iPoint v = tp - me.origin;
		v.loadIdentity();

		//me.origin += v * move_speed * dt;
		iPoint mp = v * move_speed * dt;
		if (me.origin.x < tp.x)
		{
			me.origin.x += mp.x;
			if (me.origin.x > tp.x)
				me.origin.x = tp.x;
		}
		else if (me.origin.x > tp.x)
		{
			me.origin.x += mp.x;
			if (me.origin.x < tp.x)
				me.origin.x = tp.x;
		}

		if (me.origin.y < tp.y)
		{
			me.origin.y += mp.y;
			if (me.origin.y > tp.y)
				me.origin.y = tp.y;
		}
		else if (me.origin.y > tp.y)
		{
			me.origin.y += mp.y;
			if (me.origin.y < tp.y)
				me.origin.y = tp.y;
		}
	}

#endif

}

void keyGame(iKeyStat stat, iPoint point)
{
#if 0
	keyLotto(stat, point);
	return;
#elif 0
	keyTripple(stat, point);
	return;
#else
	keyAirShootng(stat, point);
	return;
#endif
	
	if (stat == iKeyStatBegan)
	{
		tp = point;
	}
	else if (stat == iKeyStatMoved)
	{

	}
	else if (stat == iKeyStatEnded)
	{

	}
}

