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
// Super Class(최상위 클래스)
// -----------------------------------------
struct Lotto
{
	Lotto(iPoint position);
	virtual ~Lotto();

	float delta;
	iPoint position;
	METHOD_VOID method;

	// Start는 델타타임 초기화하고, 콜백함수 등록하는 역할이므로 그대로 사용
	void start(METHOD_VOID cb);
	// 반드시 자식들이 구현해야하므로 순수 가상함수로
	virtual void paint(float dt) = 0; 
};

// -----------------------------------------
// Button
// -----------------------------------------

struct Button : Lotto
{
	Button(iPoint position);
	virtual ~Button();

	static void cb();

	iSize size; // 내 버튼의 영역
	bool selected;

	virtual void paint(float dt) override; // 돈을 투입 > 사라지기
};

// -----------------------------------------
// Logic
// -----------------------------------------

struct Logic : Lotto
{
	Logic(iPoint position);
	virtual ~Logic();

	static void cb();

	int number[6];
	bool right[6], rightNum;

	virtual void paint(float dt) override; // 추첨 중 -> 숫자를 보여줌 > 당첨여부

	void shuffle();
	void match(); 
};

struct Result : Lotto
{
	Result(iPoint position);
	virtual ~Result();

	static void cb();

	virtual void paint(float dt) override; // 모든 추첨이 완료되었습니다. > 축하메시지
};
