#include "Game.h"

#include <math.h>
#include <stdio.h>
iRect me;
iPoint tp;
float takeTime;

#include "Lotto.h"
#include "Tripple.h"
#include "AirShooting.h"
#include "Memory.h"
#include "PS.h"

ParticleSystem** _ps;
ParticleSystem* ps;
iPoint psPos;
int psIndex;

void loadGame()
{
#if 1
	/*struct Score
	{
		char name[32];
		int kor, eng, math;
	};*/

	/*Score score[3] = {
		{"김태현", 100, 100, 100},
		{"김재학", 50, 50, 50},
		{"고민철", 50, 50, 50},
	};*/
	//saveFile((char*)score, sizeof(score), "test.sav");

	// 외부에서 파일을 해석하지 못하게 하는 방법1
	/*int len = sizeof(score);
	char* t = new char[len];
	for (int i = 0; i < len; i++)
		t[i] = ((char*)score)[i] + 128;
	saveFile(t, len, "test.sav");*/


	//int len;
	//char* t = loadFile(len, "test.sav");
	///*for (int i = 0; i < len; i++)
	//	t[i] -= 128;*/
	//Score* s = (Score*)t;
	////Score* s = (Score*)loadFile("test.sav");
	//for (int i = 0; i < 3; i++)
	//{
	//	printf("[%s] %d %d %d\n",
	//		s[i].name, s[i].kor, s[i].eng, s[i].math);
	//}

	/*char* str = loadFile("HelloUnreal.h");
	printf("HelloUnreal.h\n%s\n", str);
	delete str;*/
#else

#pragma pack(push, 1)
	struct Score
	{
		char kor;
		short math;
		int eng;
	};
#pragma pack(pop)

	printf("%d\n", sizeof(Score));
#endif
	me = iRectMake(0, 0, 50, 50);
	takeTime = 0.0f;

#if 0
	loadLotto();
#elif 0
	loadTripple();
#elif 1
	loadAirShootng();
#else
	loadMemory();
#endif

#if 0
	_ps = new ParticleSystem * [7];
	for (int i = 0; i < 7; i++)
	{
		char path[32];
		sprintf(path, "test%d.ptc", i);
		_ps[i] = new ParticleSystem(path);
	}
	ps = _ps[psIndex = 0];
	//ps = new ParticleSystem();
#else
	/*ps = new ParticleSystem();
	ps->save("test6.ptc");*/
#endif

	//
	//// 파일 이름으로 Load
	//ps = new ParticleSystem("test.ptc");
}

void freeGame()
{
#if 0
	freeLotto();
#elif 0
	freeTripple();
#elif 1
	freeAirShootng();
#else
	freeMemory();
#endif

	delete ps;
	delete _ps;
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
#elif 1
	drawAirShootng(dt);
	return;
#elif 0
	drawMemory(dt);
	return;
#else
	//ps->paint(dt, iPointMake(DEV_WIDTH/2, DEV_HEIGHT /2));
	ps->paint(dt, iPointMake(psPos.x, psPos.y));

	if (keydown & keydown_space)
	{
		psIndex = (psIndex + 1) % 7;
		ps = _ps[psIndex];
	}
	return
#endif

#if 1
	drawString(300, 100, L"Hi");

	takeTime += dt;

	//float dx = sin(takeTime) * 50;

	setRGBA(1, 0, 0, 1);
	//drawLine(50 + dx, 0, 150 + dx, 100);
	fillRect(me);
#endif

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
#elif 1
	keyAirShootng(stat, point);
	return;
#elif 0
	keyMemory(stat, point);
	return;
#else
	//ps->move(stat, point);
#endif
	
	if (stat == iKeyStatBegan)
	{
		//psPos = point;
		tp = point;
	}
	else if (stat == iKeyStatMoved)
	{
		psPos = point;
	}
	else if (stat == iKeyStatEnded)
	{

	}
}


