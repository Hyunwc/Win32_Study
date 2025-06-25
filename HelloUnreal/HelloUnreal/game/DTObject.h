#pragma once

#include "iStd.h"

// target : 생산 목표 개수(수주 개수)
// curr : (생산중 + 생산완료)
// complete : 생산완료
// broken : 불량품 개수

// 디스플레이 
// 생산률 complete / target
// 진행률 curr / target
extern int target, curr, complete, broken;

void loadDTObject();
void freeDTObject();
void drawDTObject(float dt, iPoint off);
bool keyDTObject(iKeyStat stat, iPoint point);

void startMake(int target);

struct DTUnit;
typedef void (*MethodWorked)(DTUnit* obj);

struct DTUnit
{
	DTUnit(int index);
	virtual ~DTUnit();

	virtual bool start(MethodWorked m); // 생산시작을 알려줘
	virtual void paint(float dt, iPoint position) = 0;

	float run(float dt);
	// 파라미터 : offset(스크롤된 값)
	iRect touchRect(iPoint position);

	int index;
	iImage* img;
	iPoint position; // 로봇의 위치
	float delta, _delta; // 생산시간

	MethodWorked methodWorked;
};

// index == 0
// 수주가 있을경우 생산시작

// 그외
// index - 1 프로세서에서 생산된 물건 있으면 생산시작

// index == max - 1
// 생산완료되면, 수주 채워짐
enum StateMake
{
	StateMakeReady = 0,
	StateMakeMake,
	StateMakeBroken,
	StateMakeComplete,

	StateMakeMax
};

// index : 0 ~ 99
struct DTUnitMake : DTUnit
{
	iImage** imgs;
	StateMake sm;

	DTUnitMake(int index);
	virtual ~DTUnitMake();

	virtual bool start(MethodWorked m) override; // 생산시작을 알려줘
	virtual void paint(float dt, iPoint position);
	
	static void cbWorked0(DTUnit* obj);
	static void cbWorked1(DTUnit* obj);
	static void cbWorked2(DTUnit* obj);
	static void cbWorked3(DTUnit* obj);
	static void cbWorked4(DTUnit* obj);
	static void cbWorked5(DTUnit* obj);
	static void cbWorked9(DTUnit* obj);
};

// index : 100 ~ 199
struct DTUnitMove : DTUnit
{
	DTUnitMove(int index);
	virtual ~DTUnitMove();

	void setAreaRange(iPoint sp, iPoint ep, float speed);
	static void cbWorked(DTUnit* obj);

	virtual bool start(MethodWorked m) override; // 생산시작을 알려줘
	virtual void paint(float dt, iPoint position);

	iPoint sp, ep;
	float speed;

	iPoint* tp;
	int tpNum;
};

// index : 200 ~ 299
struct DTUnitReqair : DTUnit
{
	DTUnitReqair();
	virtual ~DTUnitReqair();

	virtual bool start(MethodWorked m) override; // 생산시작을 알려줘
	virtual void paint(float dt, iPoint position);
};

