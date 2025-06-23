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
