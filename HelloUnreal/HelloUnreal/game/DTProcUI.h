#pragma once

#include "iStd.h"

void loadDTProcUI();
void freeDTProcUI();
void drawDTProcUI(float dt);
bool keyDTProcUI(iKeyStat stat, iPoint point);

// DT의 디스플레이되는 모든 정보
struct ProcData
{
	float playTimeTotal; // 공장이 돌아가는 총 시간
	float playTimeCurr; // 현재 운영되고 있는 시간
	float playTimeLast; // 마지막 접속 시간

	int unitTotal;		// 공장 들어온 개수
	int unitRun;		// 운영중인 
	int unitRepairing;  // 수리중인   
	int unitReqired;	// 수리되었던
	int unitReplace;	// 개조된
	int unitBroken;     // 파괴된

	int target;			// 생산 목표 개수
	int made;			// 현재 진행 개수

	float unitMakeTime[10]; // 10개라고 가정
};


