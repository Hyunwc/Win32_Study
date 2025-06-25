#include "AirShooting.h"
#include "PS.h"
// ============================
// Player
// ============================
ParticleSystem* muzzle;

void loadAirShootng()
{
	loadUs();
	loadYou();
	loadBall();

	loadEffect();
	loadUI();
	muzzle = new ParticleSystem();
}

void freeAirShootng()
{
	freeUs();
	freeYou();
	freeBall();

	freeEffect();
	freeUI();
}

void drawAirShootng(float dt)
{
	drawUs(dt);
	drawYou(dt);
	drawBall(dt);

	checkCollision();

	drawEffect(dt);
	drawUI(dt);
}

void keyAirShootng(iKeyStat stat, iPoint point)
{
	if (keyUI(stat, point))
		return;
	// 인게임 컨트롤
	switch (stat)
	{
	case iKeyStatBegan:
		break;
	case iKeyStatMoved:
		break;
	case iKeyStatEnded:
		break;
	default:
		break;
	}
}

Us* us;

void loadUs()
{
	// 초기화는 하지 않고 크기만 할당
	us = new Us; // new Us()
	us->position = iPointMake(DEV_WIDTH / 2, DEV_HEIGHT * 2 / 3);
	us->hp = 3;
	us->ap = 1;
	us->moveSpeed = 600.0f;
	us->_rate = 0.5f;
	us->rate = 0.0f;
}

void freeUs()
{
	delete us;
}

void drawUs(float dt)
{
	// 임시로 빨간색 사각형으로
	setRGBA(1, 0, 0, 1);
	fillRect(us->rt());

	// ctrl
	iPoint v = iPointZero;
	if (iKeyboardDown(keydown_a))		v.x -= 1;
	else if (iKeyboardDown(keydown_d))	v.x += 1;

	if (iKeyboardDown(keydown_w))		v.y -= 1;
	else if (iKeyboardDown(keydown_s))	v.y += 1;

	v.loadIdentity();
	us->position += v * (us->moveSpeed * dt);

#define keydown_one keydown_space
#define keydown_two keydown_space

	int key = -1;
	if (iKeyboardDown(keydown_space)) key = 0;
	else if (iKeyboardDown(keydown_one)) key = 1;
	else if (iKeyboardDown(keydown_two)) key = 2;

	if (key != -1)
	{
		us->rate += dt;
		if (us->rate >= us->_rate)
		{
			printf("fire!\n");
#if 0
			Ball* b = new Ball;
#else
			// 메모리 풀링
			// 100개중 비활성화된 것을 찾아 등록
			muzzle->paint(dt, iPointMake(us->position.x, us->position.y - 30));
			Ball* b = NULL;
			int index = 0; // 0번째 총알 사용
			for (int i = 0; i < ballMax; i++)
			{
				Ball* t = &_ball[i];
				if (t->own == NULL)
				{
					b = t;
					break;
				}
			}
#endif
			b->own = us;
			b->position = us->position + iPointMake(0, -15);
			b->v = iPointMake(0, -1);
			b->moveSpeed = 400;

			MethodBall m[3] = { paintBallDirect,
				paintBallBomb,paintBallFollow };
			b->method = m[key];

			ball[ballNum] = b;
			ballNum++;
			printf("추가 총알 = %d\n", ballNum);
		}

	}
}

// ============================
// Enemy
// ============================

You* _you;
You** you;
int youNum;

float createYouDt, _createYouDt;

void loadYou()
{
	_you = new You[youMax];
	memset(_you, 0x00, sizeof(You) * youMax);
	you = new You * [youMax];
	youNum = 0;

	_createYouDt = (200 + rand() % 100) / 100.0f;
	createYouDt = 0.0f;
}

void freeYou()
{
	delete _you;
	delete you;
}

void updateYou(float dt);
void drawYou(float dt)
{
	//Enemy
	setRGBA(1, 1, 1, 1);
	for (int i = 0; i < youNum; i++)
	{
		You* y = you[i];
		// paint
		fillRect(y->rt());

		y->position += y->v * (y->moveSpeed * dt);

		if (!containPoint(y->position,
			iRectMake(-10, -90, DEV_WIDTH + 20, DEV_HEIGHT + 90)))
		{
#if 0
			delete b;
#else
			// 삭제가 아닌 hp를 0으로
			y->hp = 0;
#endif
			youNum--;
			// 배열을 앞으로 당김
			for (int j = i; j < youNum; j++)
				you[j] = you[j + 1];
			printf("삭제 후 남은 적 = %d\n", youNum);
		}
	}

	updateYou(dt);
}

void updateYou(float dt)
{
	// render 2~3초에 한번씩 you 생성
	createYouDt += dt;
	if (createYouDt > _createYouDt)
	{
		createYouDt -= _createYouDt;
		_createYouDt = (100 + rand() % 100) / 100.0f;

		// 비활성화된 Enemy를 찾아 초기화 후 다시 사용
		You* y = NULL;
		for (int i = 0; i < youMax; i++)
		{
			You* t = &_you[i];
			if (t->hp == 0)
			{
				y = t;
				break;
			}
		}

		y->hp = 3;
		y->position = iPointMake(50 + rand() % (DEV_WIDTH - 100), -50);
		y->v = iPointMake(0, 1);
		y->moveSpeed = 100 + rand() % 100;

		you[youNum] = y;
		youNum++;
		printf("추가 적 = %d\n", youNum);
	}
}


// ============================
// Ball
// ============================

Ball* _ball; // 총알 종류
Ball** ball;
int ballNum; // 총알 갯수

void loadBall()
{
	/*Ball* t = new Ball;
	((BallFollow*)t)->tp;*/

	_ball = new Ball[ballMax];
	memset(_ball, 0x00, sizeof(Ball) * ballMax);

	// 주소값이지 실체가 아님, 총알을 담을 주소 100개
	ball = new Ball * [ballMax];
	ballNum = 0;
}

void freeBall()
{
	delete _ball;
	delete ball;
}

void drawBall(float dt)
{
	iRect rt;
	// Bullet
	setRGBA(0, 0, 1, 1);
	for (int i = 0; i < ballNum; i++)
	{
		Ball* b = ball[i];
		// paint
		rt.origin = b->position + iPointMake(-5, -5);
		rt.size = iSizeMake(10, 10);
		fillRect(rt);

		b->position += b->v * (b->moveSpeed * dt);

		// 총알이 화면을 벗어날 경우
		// or 적이랑 충돌이 일어났다면
		if (!containPoint(b->position,
			iRectMake(-10, -10, DEV_WIDTH + 20, DEV_HEIGHT + 20)))
		{
#if 0
			delete b;
#else
			// 삭제가 아닌 NULL로 비활성화
			b->own = NULL;
#endif
			ballNum--;
			// 배열을 앞으로 당김
			for (int j = i; j < ballNum; j++)
				ball[j] = ball[j + 1];
			printf("삭제 후 남은 총알 = %d\n", ballNum);
		}
	}
}

bool paintBallDirect(Ball* b, float dt)
{
	b->position += b->v * (b->moveSpeed * dt);
	return false;
}


// 충돌 후 
bool paintBallFollow(Ball* ball, float dt)
{
	BallFollow* b = (BallFollow*)ball;

	if (b->findDt < b->_findDt)
	{
		iPoint v = b->tp - b->position;
		v.loadIdentity();

		b->position += v * (b->moveSpeed * dt);
		b->v = v; // 백업

		b->findDt += dt;

		if (iPointLength(b->position - b->tp) < 5)
			b->findDt = b->_findDt;
	}
	else
	{
		// 등장한 you중에서 가장 가까운 you
		You* y = NULL;
		float dis = 0xffffff;
		for (int i = 0; i < youNum; i++)
		{
			float d = iPointLength(b->position - you[i]->position);
			if (d < dis)
			{
				dis = d;
				y = you[i];
			}
		}
		if (y)
		{
			b->tp = y->position;
			b->findDt = 0.0f;
		}
		else
		{
			b->tp = b->position + b->tp * 1000;
			b->findDt = -20.0f;
		}
	}

	return false;
}

// Direct와 동일, 충돌 후 데미지 주변 영향
bool paintBallBomb(Ball* b, float dt)
{
	b->position += b->v * (b->moveSpeed * dt);
	return false;
}

// ============================
// Collision
// ============================

void checkCollision()
{
	// collision
	// us + you
	iRect rtDst = us->rt();
	for (int i = 0; i < youNum; i++)
	{
		You* y = you[i];
		if (containRect(y->rt(), rtDst))
		{
			y->hp--;
			// 요소를 전부 이동시키지 않고
			// 맨 마지막 요소를 빈곳에 채운다
			if (y->hp == 0)
			{
				youNum--;
				you[i] = you[youNum];
				i--;
			}

			us->hp--;

			if (us->hp == 0)
			{

			}
		}
	}
	// us + you + ball
}

// ============================
// Effect
// ============================

void loadEffect()
{
}

void freeEffect()
{
}

void drawEffect(float dt)
{
}

void addEffect(Effect ect, iPoint position, void* own)
{
}

// ============================
// UI
// ============================


void loadUI()
{
}

void freeUI()
{
}

void drawUI(float dt)
{
}

bool keyUI(iKeyStat stat, iPoint point)
{
	//if (// TODO : UI영역이 아닐경우의 함수를 만들어야함)
	//	return false;
	
	// 인게임 컨트롤
	switch (stat)
	{
	case iKeyStatBegan:
		break;
	case iKeyStatMoved:
		break;
	case iKeyStatEnded:
		break;
	default:
		break;
	}

	return true;
}


