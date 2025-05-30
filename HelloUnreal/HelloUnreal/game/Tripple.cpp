#include "Tripple.h"
#include "time.h"

#if 1

Bot** bot;
iSort* sort;

int order[] = { 0, 1, 2 };

void loadTripple()
{
	void aaa();
	aaa();
	bot = new Bot*[3];
	bot[0] = new BotRed(iRectMake(10, 200, 50, 50), 
						iColor4fRed, 100.0f);
	bot[1] = new BotGreen(iRectMake(300, 10, 80, 40), 
						  iColor4fGreen, 200.0f);
	bot[2] = new BotBlue(iRectMake(200, 200, 30, 30), 
						 iColor4fBlue, 300.0f, 1, 2);

	sort = new iSort();
}

void freeTripple()
{
	for (int i = 0; i < 3; i++)
		delete bot[i];
	delete bot;
}

//void sort()
//{
//	for (int i = 0; i < 2; i++)
//	{
//		for (int j = i + 1; j < 3; j++)
//		{
//			if (bot[i]->sortY() > bot[j]->sortY())
//			{
//				Bot* temp = bot[i];
//				bot[i] = bot[j];
//				bot[j] = temp;
//			}
//		}
//	}
//}

void Shuffle(Bot* b0, Bot* b1, int idx1, int idx2)
{
	// 인자로 받은 포인터들의 좌표를 비교
	if (b0->rt.origin.y + b0->rt.size.height >
		b1->rt.origin.y + b1->rt.size.height)
	{
		int temp = order[idx1];
		order[idx1] = order[idx2];
		order[idx2] = temp;

		Bot* b = bot[idx1];
		bot[idx1] = bot[idx2];
		bot[idx2] = b;
	}
}

void drawTripple(float dt) 
{
	sort->init();

	for (int i = 0; i < 3; i++)
		sort->add(bot[i]->sortY());
	sort->update();

	for (int i = 0; i < 3; i++)
		bot[sort->get(i)]->paint(dt);

}

void keyTripple(iKeyStat stat, iPoint point)
{
	switch (stat)
	{
	case iKeyStatBegan:
	
		break;
	case iKeyStatMoved:
		
		break;
	case iKeyStatEnded: // 버튼을 뗀 상태
	
	default:
		break;
	}
}

#else

iRect* _rtObj;
iRect** rtObj;
int numObj;

bool selectedObj;
iPoint prevPoint; // 이전 좌표

// new (타입)[개수]
void loadTripple()
{
	_rtObj = new iRect[3];
	rtObj = new iRect* [3];
	for (int i = 0; i < 3; i++)
	{
		_rtObj[i] = iRectMake(50 * i, 50 * i, 50, 50);
		rtObj[i] = &_rtObj[i];
	}
		
	selectedObj = false;
	srand(time(0));
}

void freeTripple()
{
	delete _rtObj;
	delete rtObj;
}

void drawTripple(float dt)
{
	iColor4f color[3] = {
		{1, 0, 0, 1},
		{0, 1, 0, 1},
		{0, 0, 1, 1}
	};

	for (int i = 0; i < 3; i++)
	{
		iColor4f* c = &color[i];
		setRGBA(c->r, c->g, c->b, c->a);
		fillRect(*rtObj[i]);
	}

	if (selectedObj)
	{
		setRGBA(1, 1, 1, 1);
		iRect rt = *rtObj[2];
		rt.origin += iPointMake(5, 5);
		rt.size += iSizeMake(-10, -10);
		fillRect(rt);	
	}
}

void keyTripple(iKeyStat stat, iPoint point)
{
	switch (stat)
	{
	case iKeyStatBegan:
		for (int i = 2; i > -1; i--)
		{
			if (containPoint(point, *rtObj[i]))
			{
				iRect* t = rtObj[i];
				for (int j = i + 1; j < 3; j++)
				{
					rtObj[j - 1] = rtObj[j];
				}

				rtObj[2] = t;

				selectedObj = true;
				prevPoint = point;
				break;
			}
		}
		break;
	case iKeyStatMoved:
		if (selectedObj)
		{
			// 움직일 좌표 = 현재 - 이전
			iPoint mp = point - prevPoint;
			// 마지막 사각형을 움직어야 하므로 2
			rtObj[2]->origin += mp;

			prevPoint = point;
		}
		break;
	case iKeyStatEnded: // 버튼을 뗀 상태
		selectedObj = false;
		break;

	default:
		break;
	}
}

#endif

Bot::Bot(iRect rt, iColor4f color)
{
	this->rt = rt;
	this->color = color;
	energy = 0;
}

Bot::~Bot()
{
	// todo something
}

void Bot::paint(float dt)
{
	iColor4f* c = &color;
	setRGBA(c->r, c->g, c->b, c->a);
	fillRect(rt);
}

BotRed::BotRed(iRect rt, iColor4f color, float speed) : Bot(rt, color)
{
	this->speed = speed;
	left = true;
}

BotRed::~BotRed()
{
	// todo something
}

void BotRed::paint(float dt)
{
	if (left)
	{
		rt.origin.x -= speed * dt;
		if (rt.origin.x < 0)
		{
			rt.origin.x = 0;
			left = false;
		}
	}
	else //if (left == false)
	{
		rt.origin.x += speed * dt;
		// 현재 좌표 + 사이즈를 더하는 이유
		// 현재 사이즈가 더해지지 않으면 오브젝트가
		// 화면 밖으로 넘어가게됨
		if (rt.origin.x + rt.size.width > DEV_WIDTH)
		{
			rt.origin.x = DEV_WIDTH - rt.size.width;
			left = true;
		}
	}

	Bot::paint(dt);
}

void BotRed::sort(Bot* b0, Bot* b1, int idx1, int idx2)
{
}

BotGreen::BotGreen(iRect rt, iColor4f color, float speed) : Bot(rt, color)
{
	this->speed = speed;
	up = true;
}

BotGreen::~BotGreen()
{
	// todo something
}

void BotGreen::paint(float dt)
{
	if (up)
	{
		rt.origin.y -= speed * dt;
		if (rt.origin.y < 0)
		{
			rt.origin.y = 0;
			up = false;
		}
	}
	else //if (up == false)
	{
		rt.origin.y += speed * dt;
		if (rt.origin.y + rt.size.height > DEV_HEIGHT)
		{
			rt.origin.y = DEV_HEIGHT - rt.size.height;
			up = true;
		}
	}

	Bot::paint(dt);
}

void BotGreen::sort(Bot* b0, Bot* b1, int idx1, int idx2)
{
}

BotBlue::BotBlue(iRect rt, iColor4f color, float speed, float aiFrom, float aiTo) : Bot(rt, color)
{
	this->speed = speed;
	this->aiFrom = aiFrom;
	_ai = 0.0f;
	ai = 0.0f;
	tp = rt.origin;
}

BotBlue::~BotBlue()
{
	// todo something
}

void BotBlue::cb(BotBlue* bb)
{
	// 타겟 포지션 x, y 랜덤으로 
	bb->tp = iPointMake(rand() % DEV_WIDTH, rand() % DEV_HEIGHT);
	bb->_ai = linear(bb->aiFrom, bb->aiTo, (rand() % 100) / 100.f);
	bb->ai = 0.0f;
}

void BotBlue::paint(float dt)
{
	if (rt.origin != tp)
	{
		iPoint mp = tp - rt.origin;
		mp.loadIdentity();
		move(&rt.origin, &tp, mp * (speed *dt));
	}
	// 현재와 타겟 위치가 같을 경우
	else if (rt.origin == tp)
	{
		ai += dt;
		if (ai >= _ai)
		{
			cb(this); // 임시 콜백
		}
	}

	Bot::paint(dt);
}

void BotBlue::sort(Bot* b0, Bot* b1, int idx1, int idx2)
{
}



