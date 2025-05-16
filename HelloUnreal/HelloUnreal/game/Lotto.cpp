#include "Lotto.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

Button* btn;
Logic** logic;
Result* result;

int* answer; // 정답 번호

int gachaIndex, gachaNum;

void loadLotto()
{
	btn = new Button(iPointMake(10, 50));
	logic = new Logic * [5];
	for(int i = 0; i < 5; i++)
		logic[i] = new Logic(iPointMake(10, 100 + 40 * i));
	result = new Result(iPointMake(10, 300));

	answer = new int[6];
	for (int i = 0; i < 6; i++)
		answer[i] = 1 + i;

	srand(time(NULL));
}

void freeLotto()
{
	delete btn;
	for (int i = 0; i < 5; i++)
		delete logic[i];
	//btn = nullptr;
	delete logic;
	delete result;

	delete answer;
}

void drawLotto(float dt)
{
	btn->paint(dt);
	for (int i = 0; i < 5; i++)
		logic[i]->paint(dt);
	result->paint(dt);
}

void cbComplete()
{
	printf("all complete...\n");
	// 초기화
	loadLotto();
}

void cbLogic()
{
	printf("cbLogic\n");
	gachaIndex++;
	if (gachaIndex < gachaNum)
	{
		logic[gachaIndex]->start(cbLogic);
	}
	else// if (gachaIndex == gachaNum)
	{
		// Result
		result->start(cbComplete);
	}
}

void cbBtn()
{
	printf("cbBtn\n");
	// 버튼을 눌렀으니 start
	logic[0]->start(cbLogic);
}

void keyLotto(iKeyStat stat, iPoint point)
{
	if (stat == iKeyStatBegan)
	{
		gachaNum = 1 + rand() % 5; // 1 ~ 5
		printf("gachaNum = %d", gachaNum);
		gachaIndex = 0;
		//tp = point;
		btn->start(0, cbBtn);
	}
	else if (stat == iKeyStatMoved)
	{
		iRect rt;
		rt.origin = btn->position;
		rt.size = btn->size;
		btn->selected = containPoint(point, rt);
	}
	else if (stat == iKeyStatEnded)
	{

	}
}

// -----------------------------------------
// Button
// -----------------------------------------

Button::Button(iPoint position)
{
	delta = 0.0f;
	this->position = position;
	method = NULL;

	size = iSizeMake(140, 35);
	selected = false;
}

Button::~Button()
{
	// TODO someting..
}

#define buttonAni 2.0f

void Button::paint(float dt)
{
	float scale = 1.0f;
	// 아무것도 안 일어난 상태
	if (delta == 0.0f)
		;
	else if (delta < buttonAni)
	{
		// animation
		scale = delta / buttonAni; // 0 ~ 1
		scale = 1.0f - scale; // 1 ~ 0
		printf("scale = %f\n", scale);

		delta += dt;
		if (delta >= buttonAni)
		{
			// 함수가 등록 되었을때만
			if (method)
				method();
		}
	}
	else
		return;

	if (selected)
		setRGBA(1, 0, 0, 1);
	else
		setRGBA(0, 0, 1, 1);

	fillRect(position.x, position.y, size.width, size.height);

	setStringSize(30);
	setStringRGBA(1, 1, 1, scale);
	drawString(position.x, position.y, L"로또시작");
}

void Button::start(int money, METHOD_VOID cbGacha)
{
	// 개발환경에서 0보다 크면서 가장 작은 값. 소숫점 6자리
	delta = 0.000001f;
	method = cbGacha;
}

// -----------------------------------------
// Logic
// -----------------------------------------

Logic::Logic(iPoint position)
{
	delta = 0.0f;
	this->position = position;
	method = NULL;
	for (int i = 0; i < 6; i++)
		number[i] = 0;

	//srand(time(NULL));
}

Logic::~Logic()
{
	// TODO Someting...
}

void Logic::paint(float dt)
{
	// 안보임(스타트) > 추첨중 > 숫자를 보여줌 > 당첨여부
	if (delta == 0.0f)
	{
		// 안보임
	}
	else if (delta < 2.0f)
	{
		// 추첨 중
		const WCHAR* s = NULL;
		if (delta < 0.3f)		s = L"추";
		else if (delta < 0.6f)  s = L"추첨";
		else if (delta < 0.9f)  s = L"추첨중";
		else if (delta < 1.2f)  s = L"추첨";
		else if (delta < 1.5f)  s = L"추";
		else if (delta < 1.8f)  s = L"추첨";
		else					s = L"추첨중";
			
		setStringSize(30);
		setStringRGBA(1, 1, 1, 1);
		drawString(position.x, position.y, s);
		
		delta += dt;
		if (delta >= 2.0f)
		{
			// 중복되지 않도록 1 ~ 45 중 6개의 숫자를 랜덤하게 뽑기
			shuffle();
		}
	}
	// 숫자 한개씩 노출
	else if (delta < 3.2f)
	{
		for (int i = 0; i < 6; i++)
		{
			// a는 알파값
			// t는 0.2초씩 늘리기 위함
			float a = 1.0f;
			float t = 2.0f + 0.2f * i;
			if (delta < t)			   a = 0.0f;
			else if (delta < t + 0.2f) a = (delta - t) / 0.2f;

			setStringRGBA(1, 1, 1, a);
			WCHAR s[10];
			wsprintf(s, L"%d", number[i]);
			drawString(position.x + 40 * i, position.y, s);
		}

		delta += dt;
	}
	else// if(delta < 3.7f)
	{
		// 모든 숫자 노출(당첨숫자 표시) + 당첨여부
	
		for (int i = 0; i < 6; i++)
		{
			if (right[i])
				setStringRGBA(0, 0, 1, 1);
			else
				setStringRGBA(1, 1, 1, 1);

			WCHAR s[10];
			wsprintf(s, L"%d", number[i]);
			drawString(position.x + 40 * i, position.y, s);
		}
		setStringRGBA(1, 1, 1, 1);
		WCHAR s[20];
		wsprintf(s, L"당첨여부 %d", rightNum);
		drawString(position.x + 240, position.y, s);

		delta += dt;
		if (delta >= 3.7f)
		{
			// 콜백함수 
			if (method)
			{
				method();
				method = NULL;
			}
		}
	}
	// 추첨 중.... 숫자 노출x 당첨 여부X
	// 숫자 노출O + 당첨 여부O or X
}

void Logic::start(METHOD_VOID cbResult)
{
	delta = 0.000001f;
	method = cbResult;
}

void Swap(int& a, int& b)
{
	int temp = a;
	a = b;
	b = a;
}

void Sort(int arr[], int size)
{
	for (int i = 0; i < size - 1; i++)
	{
		for (int j = 0; j < (size - 1 - i); j++)
		{
			if (arr[j] > arr[j + 1])
				Swap(arr[j], arr[j + 1]);
		}
	}
}

void Logic::shuffle()
{
#if 0
	int cnt = 0;
	// cnt가 6이 될때까지 반복
	while (cnt < 6)
	{
		// 랜덤 숫자 생성
		int randNum = rand() % 45 + 1;
		// 중복 여부
		bool duplication = false;
		// 중복 체크
		for (int i = 0; i < 6; i++) 
		{
			// 같다면 불리언값 true
			if (randNum == number[i])
			{
				duplication = true;
			}
		}
		// false일때만 삽입.true일 경우 다시 반복
		if (!duplication)
		{
			number[cnt] = randNum;
			cnt++;
		}
	}
#else
	// 중복체크가 필요 없음
	bool n[45] = {false, };
	for (int i = 0; i < 6; i++)
	{
		for (;;)
		{
			int r = rand() % 45;
			if (n[r] == false)
			{
				n[r] = true;
				break;
			}
		}
	}
	for (int i = 0, j = 0; i < 45; i++)
	{
		if (n[i])
		{
			number[j] = i + 1;
			j++;
		}
	}

	Sort(number, 6);

#endif
	match(); // 답이 맞는지?
}

void Logic::match()
{
	int n = 0;
	for (int i = 0; i < 6; i++)
	{
		bool exist = false;
		right[i] = false;
		for (int j = 0; j < 6; j++)
		{
			if (number[i] == answer[j])
			{
				right[i] = true;
				n++;
				break;
			}
		}
	}

	rightNum = n;
	printf("right number = %d\n", n);
}

Result::Result(iPoint position)
{
	delta = 0.0f;
	this->position = position;
	method = NULL;
}

Result::~Result()
{
	// TODO something..
}

void Result::paint(float dt)
{
	float a = 0.0f;
	if (delta == 0.0f);
	else
	{
		delta += dt;
		if (delta >= 1.5f)
		{
			if (method)
			{
				method();
				method = NULL;
			}
		}
	}
	a = delta / 1.5f;

	setStringSize(30);
	setStringRGBA(1, 1, 1, a);
	drawString(position.x, position.y, L"뽑기완료");
	setStringRGBA(1, 1, 1, 1);
}

void Result::start(METHOD_VOID cbComplete)
{
	delta = 0.000001f;
	method = cbComplete;
}


