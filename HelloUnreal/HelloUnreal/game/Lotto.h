#pragma once

#include "iStd.h"

void loadLotto();
void freeLotto();
void drawLotto(float dt);
void keyLotto(iKeyStat stat, iPoint point);

// TODO : 돈을 투입(1000~5000) 키 : 1~5

// 추첨 시작
// 추첨 중 : 2~3초 
// 숫자가 등장. 간격 : 0.2초
// 이번주 당첨 번호와 비교 표시

// 다음 추첨

// 버튼 : 버튼을 누르면 애니메이션 시작 > 콜백 등록함수 실행

// -----------------------------------------
// Button
// -----------------------------------------

struct Button
{
	Button(iPoint position);
	virtual ~Button();

	iPoint position;
	float delta;
	METHOD_VOID method;

	iSize size; // 내 버튼의 영역
	bool selected;

	void paint(float dt); // 돈을 투입 > 사라지기
	void start(int money, METHOD_VOID cbGacha);
};

// -----------------------------------------
// Logic
// -----------------------------------------

struct Logic
{
	Logic(iPoint position);
	virtual ~Logic();

	float delta;
	iPoint position;
	METHOD_VOID method;

	int number[6];
	bool right[6], rightNum;

	void paint(float dt); // 추첨 중 -> 숫자를 보여줌 > 당첨여부
	void start(METHOD_VOID cbResult);
	void shuffle();
	void match(); 
};

struct Result
{
	Result(iPoint position);
	virtual ~Result();

	float delta;
	iPoint position;
	METHOD_VOID method;

	void paint(float dt); // 모든 추첨이 완료되었습니다. > 축하메시지
	void start(METHOD_VOID cbComplete); 
};
