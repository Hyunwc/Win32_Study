#pragma once

#include "iStd.h"

void loadAirShootng();
void freeAirShootng();
void drawAirShootng(float dt);
void keyAirShootng(iKeyStat stat, iPoint point);

// ============================
// Us
// ============================
struct Us
{
	iPoint position;
	int hp, ap; // 체력과 공격력
	// _rate -> 어디선가 가지고 있어야 rate 반영
	float moveSpeed, rate, _rate;

	iRect rt()
	{
		return iRectMake(position.x - 15, position.y - 15, 30, 30);
	}
};

extern Us* us;

void loadUs();
void freeUs();
void drawUs(float dt);

// ============================
// you
// ============================
// ai
struct You
{
	int hp;
	iPoint position, v;
	float moveSpeed;

	iRect rt()
	{
		return iRectMake(position.x - 10, position.y - 10, 20, 20);
	}
};

extern You* _you;
extern You** you;
extern int youNum;
#define youMax 50

extern float createYouDt, _createYouDt;

void loadYou();
void freeYou();
void drawYou(float dt);

// ============================
// Ball
// ============================
struct _Ball;
typedef bool (*MethodBall)(_Ball* ball, float dt);

typedef struct _Ball
{
	void* own; // 소유자
	iPoint position, v;
	float moveSpeed;

	MethodBall method;
	// diff
	float dummy[8];
} Ball, BallDirect;

// 유도탄
struct BallFollow
{
	void* own; // 소유자
	iPoint position, v;
	float moveSpeed;

	MethodBall method;
	// diff
	iPoint tp; // 타겟
	float findDt, _findDt;
};

// 광범위 폭탄
struct BallBomb
{
	void* own; // 소유자
	iPoint position, v;
	float moveSpeed;

	MethodBall method;
	// diff
	float range; // 범위
};

extern Ball* _ball; // 총알 종류
extern Ball** ball;
extern int ballNum; // 총알 갯수
#define ballMax 120

void loadBall();
void freeBall();
void drawBall(float dt);

bool paintBallDirect(Ball* b, float dt);
bool paintBallFollow(Ball* ball, float dt);
bool paintBallBomb(Ball* b, float dt);

// ============================
// Collision
// ============================
void checkCollision();

// ============================
// Effect
// ============================
// 피격, 잔상 경고
enum Effect
{
	EffectDmg = 0,
	EffectLine,
	EffectWarning,
};

void loadEffect();
void freeEffect();
void drawEffect(float dt);
void addEffect(Effect ect, iPoint position, void* own);

// ============================
// UI
// ============================
void loadUI();
void freeUI();
void drawUI(float dt);
bool keyUI(iKeyStat stat, iPoint point);