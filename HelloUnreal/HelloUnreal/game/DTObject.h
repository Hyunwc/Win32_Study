#pragma once

#include "iStd.h"

void loadDTObject();
void freeDTObject();
void drawDTObject(float dt, iPoint off);
bool keyDTObject(iKeyStat stat, iPoint point);

struct DTUnit;

typedef void (*MethodWorked)(DTUnit* obj);

struct DTUnit
{
	DTUnit(int index);
	virtual ~DTUnit();

	virtual void start(MethodWorked m) = 0; // 생산시작을 알려줘
	virtual void paint(float dt, iPoint position) = 0;

	// 파라미터 : offset(스크롤된 값)
	iRect touchRect(iPoint position);

	int index;
	iImage* img;
	iPoint position; // 로봇의 위치
	float delta, _delta; // 생산시간
};

struct DTUnitMake : DTUnit
{
	DTUnitMake(int index);
	virtual ~DTUnitMake();

	virtual void start(MethodWorked m); // 생산시작을 알려줘
	virtual void paint(float dt, iPoint position);
};

struct DTUnitMove : DTUnit
{
	DTUnitMove();
	virtual ~DTUnitMove();

	virtual void start(MethodWorked m); // 생산시작을 알려줘
	virtual void paint(float dt, iPoint position);
};

struct DTUnitReqair : DTUnit
{
	DTUnitReqair();
	virtual ~DTUnitReqair();

	virtual void start(MethodWorked m); // 생산시작을 알려줘
	virtual void paint(float dt, iPoint position);
};

