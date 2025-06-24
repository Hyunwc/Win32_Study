#pragma once

#include "iStd.h"

void loadDTLogin();
void freeDTLogin();
void drawDTLogin(float dt);
void keyDTLogin(iKeyStat stat, iPoint point);


struct LoginBar
{
	LoginBar();
	virtual ~LoginBar();

	bool bShow;
	float delta, _delta;
	void show(bool s);
	void paint(float dt, iPoint position);
};

typedef void (*cbDtLogin)(int result);
void dtLogin(cbDtLogin func, const char* name, const char* pw, int n);

#define DISPLAY_THREAD 0