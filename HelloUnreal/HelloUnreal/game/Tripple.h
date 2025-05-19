#pragma once

#include "iStd.h"

void loadTripple();
void freeTripple();
void drawTripple(float dt);
void keyTripple(iKeyStat stat, iPoint point);

#define DEV_WIDTH 640
#define DEV_HEIGHT 480

struct Bot
{
	Bot(iRect rt, iColor4f color);
	virtual ~Bot();

	iRect rt;
	iColor4f color;
	int energy;

	virtual void paint(float dt);
	virtual void sort(Bot* b0, Bot* b1, int idx1, int idx2) = 0;
};

struct BotRed : Bot
{
	float speed;
	bool left;

	BotRed(iRect rt, iColor4f color, float speed);
	virtual ~BotRed();

	virtual void paint(float dt) override; // 좌우
	virtual void sort(Bot* b0, Bot* b1, int idx1, int idx2) override;
};

struct BotGreen : Bot
{
	float speed;
	bool up;

	BotGreen(iRect rt, iColor4f color, float speed);
	virtual ~BotGreen();

	virtual void paint(float dt) override; // 세로
	virtual void sort(Bot* b0, Bot* b1, int idx1, int idx2) override;
};

struct BotBlue : Bot
{
	float speed, ai, _ai, aiFrom, aiTo;
	iPoint tp; // 목표 지점

	static void cb(BotBlue* bb);

	BotBlue(iRect rt, iColor4f color, float speed, float aiFrom, float aiTo);
	virtual ~BotBlue();

	virtual void paint(float dt) override; // 자율
	virtual void sort(Bot* b0, Bot* b1, int idx1, int idx2) override;
};

