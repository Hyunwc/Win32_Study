#pragma once

#include "iStd.h"

void loadAnimating();
void freeAnimating();
void drawAnimating(float dt);
void keyAnimating(iKeyStat stat, iPoint point);

enum Behave
{
	BehaveWait = 0,
	BehaveWalk,
	BehaveRepair,

	BehaveMax
};

extern int tileX, tileY;
extern uint8* tile; // 1:이동가능 9:이동불가

// 백그라운드(길)
void loadAnimatingBG();
void freeAnimatingBG();
void drawAnimatingBG(float dt);

class AIRobot
{
public:
	AIRobot(int index);
	virtual ~AIRobot();

	void paint(float dt);

	static void cbRepair(void* data);

	iRect touchRect();

public:
	int index;
	iImage** imgs; // 0 : wait, 1 : walk, 2 : repair
	iPoint position;
	float speed, rate;

	Behave behave;
};
