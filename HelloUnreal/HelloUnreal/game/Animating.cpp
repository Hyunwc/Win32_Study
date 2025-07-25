#include "Animating.h"

static iSort* sort;
AIRobot** ai;
int selectedAI;

iShortestPath* sp;

void loadAnimating()
{
	loadAnimatingBG(); // 길

	sort = new iSort(5);

	ai = new AIRobot * [5];
	for (int i = 0; i < 5; i++)
	{
		ai[i] = new AIRobot(i);
		ai[i]->position = iPointMake(100 + 50 * i, 100 + 50 * i);
	}
	
	selectedAI = -1;

	sp = new iShortestPath();
}

void freeAnimating()
{
	freeAnimatingBG();

	delete sort;

	for (int i = 0; i < 5; i++)
		delete ai[i];
	delete ai;

	delete sp;
}

void drawAnimating(float dt)
{
	/*int tx = 5;
	for (int i = 0; i < 20; i++)
	{
		int x = i % tx;
		int y = i / tx;
		printf("%d => (%d, %d)\n", i, x, y);
	}*/

	// draw

	/*for (int j = 0; j < tileY; j++)
	{
		for (int i = 0; i < tileX; i++)
			tile[tileX * j + i] = 1;
	}*/
	// 위와 같음
	memset(tile, 0x01, sizeof(uint8) * tileX * tileY);

	sort->init();
	for (int i = 0; i < 5; i++)
	{
		AIRobot* a = ai[i];
		sort->add(a->position.y);

		int x = ((int)a->position.x) / 64;
		int y = ((int)a->position.y) / 64;
		int index = tileX * y + x;
		tile[index] = CX;
	}

	sort->update();

	drawAnimatingBG(dt);

	for (int i = 0; i < 5; i++)
	{
		int j = sort->get(i);
		AIRobot* a = ai[j];

		a->rate = linear(1, 2, a->position.y / devSize.height);
		a->paint(dt);
	}
	
	// ctrl
	if (selectedAI == -1)
		return;
	AIRobot* ai = ::ai[selectedAI];

	setRGBA(1, 1, 1, 1);
	drawRect(ai->touchRect());

	if (ai->behave != BehaveRepair)
	{
		iPoint v = iPointZero;
		if (iKeyboardStat(keydown_a))		v.x -= 1;
		else if (iKeyboardStat(keydown_d))	v.x += 1;

		if (iKeyboardStat(keydown_w))		v.y -= 1;
		else if (iKeyboardStat(keydown_s))	v.y += 1;

		Behave be;
		if (v == iPointZero)
			be = BehaveWait;
		else
		{
			be = BehaveWalk;
			v.loadIdentity();
		}

		// 수리
		if (iKeyboardDown(keydown_space))
		{
			be = BehaveRepair;
		}

		//행동 및 포지션 갱신
		ai->behave = be;
		ai->position += v * (ai->speed * dt);

		if (be == BehaveRepair)
		{
			ai->imgs[be]->startAnimation(AIRobot::cbRepair, ai);
		}
	}
}

void keyAnimating(iKeyStat stat, iPoint point)
{
	int i, j = -1;
	switch (stat) {
	case iKeyStatBegan:
		if (selectedAI == -1)
		{
			for (int i = 0; i < 5; i++)
			{
				if (containPoint(point, ai[i]->touchRect()))
				{
					j = i;
					break;
				}
			}
			selectedAI = j;
		}
		else // 선택이됨
		{
			void testArrive(AIRobot * ai);
			ai[selectedAI]->gogo(point, testArrive);
			selectedAI = -1;
		}
		break;

	case iKeyStatMoved:
		break;
	case iKeyStatEnded:
		break;
	default:
		break;
	}
}

int tileX, tileY;
uint8* tile; // 1:이동가능 9:이동불가

void loadAnimatingBG()
{
	// 640 480
	// 10 x 8(7.5)
	
	tileX = 10, tileY = 8;
	tile = new uint8[tileX * tileY];
}

void freeAnimatingBG()
{
	delete tile;
}

void drawAnimatingBG(float dt)
{
	iColor4f color[10] = {
		{},
		{1, 1, 1, 0.5f},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{1, 0, 1, 0.5f},
	};

	for (int j = 0; j < tileY; j++)
	{
		for (int i = 0; i < tileX; i++)
		{
			int t = tile[tileX * j + i];
			iColor4f& c = color[t];
			setRGBA(c.r, c.g, c.b, c.a);
			fillRect(64 * i, 64 * j, 64, 64);

			setRGBA(1, 1, 1, 1);
			drawRect(64 * i + 2, 64 * j + 2, 60, 60);
		}
	}
}

AIRobot::AIRobot(int index)
{
	// wait : 30 x 80, blue, 3 frame
	// walk : 20 x 100, red, 4 frame
	// repair : 50 x 50, green, 20 frame
	this->index = index;

	struct RobotInfo
	{
		int width, height;
		iColor4f color;
		int frame;
	};

	RobotInfo robotInfo[3] = {
		{30, 80, iColor4fBlue, 3},
		{20, 100, iColor4fRed, 4},
		{50, 50, iColor4fGreen, 20},
	};
	iGraphics* g = iGraphics::getInstance();

	imgs = new iImage * [3];
	for (int i = 0; i < 3; i++)
	{
		iImage* img = new iImage();

		RobotInfo* ri = &robotInfo[i];
		for (int j = 0; j < ri->frame; j++)
		{
			g->init(ri->width, ri->height);

			iColor4f* c = &ri->color;
			setRGBA(c->r, c->g, c->b, c->a);
			g->fillRect(0, 0, ri->width, ri->height);

			setStringSize(10);
			setStringRGBA(0, 0, 0, 1);
			//g->drawString(0, 0, "%d", j);

			Texture* tex = g->getTexture();
			img->add(tex);
			freeImage(tex);
		}

		img->position = iPointMake(-ri->width / 2, -ri->height);
		if (i < 2)
			img->startAnimation();
		imgs[i] = img;
	}

	position = iPointMake(200, 300);
	speed = 300;

	behave = BehaveWait;

	path = new iPoint[tileX * tileY];
	pathNum = 0;
}

AIRobot::~AIRobot()
{
	for (int i = 0; i < 3; i++)
		delete imgs[i];
	delete imgs;
}

void AIRobot::paint(float dt)
{
	// draw
#if 1
	setRGBA(0, 0, 0, 1);
	for (int i = 0; i < pathNum; i++)
	{
		iPoint& p = path[i];
		fillRect(p.x, p.y, 10, 10);
	}
	setRGBA(1, 1, 1, 1);
#endif

	imgs[behave]->rate = rate;
	imgs[behave]->paint(dt, position);

	setRGBA(1, 1, 1, 1);
	fillRect(position.x - 30, position.y - 1, 60, 2);
	fillRect(position.x - 1, position.y - 30, 2, 60);

	// ctrl
	if (pathNum)
	{
		iPoint targetPos = path[pathIndex]; // 내가 가야할 곳
		iPoint v = targetPos - position;
		v.loadIdentity();
		if (move(&position, &targetPos, v * 300 * dt))
		{
			pathIndex++;
			// 모든 경로를 다갔다면?
			if (pathIndex == pathNum)
			{
				pathNum = 0;
				if (method)
					method(this);
			}
		}
	}
	
	pathNum;
	pathIndex; // path[0] ~ path[pathNum - 1]
}

void AIRobot::cbRepair(void* data)
{
	/*int i = -1;
	for (; i < 5; i++)
	{
		if (ai[i] == data)
		{
			break;
		}
	}*/

	AIRobot* ai = (AIRobot*)data;
	printf("누구? : [%d]의 수리가 끝\n", ai->index);
	ai->behave = BehaveWait;
}

iRect AIRobot::touchRect()
{
	return imgs[behave]->touchRect(position);
}

void AIRobot::gogo(iPoint point, MethodAIRobotArrive m)
{
	sp->set(tile, tileX, tileY, 64, 64);
	sp->run(position, point, path, pathNum);
	// 구했으니 0부터 출발
	pathIndex = 0;
	method = m;
}

void testArrive(AIRobot* ai)
{
	int i;
	for (i = 0; i < 5; i++)
	{
		if (::ai[i] == ai)
			break;
	}
	printf("ai[%d] 수리해\n", i);
	ai->imgs[ai->behave = BehaveRepair]->startAnimation(AIRobot::cbRepair, ai);
}
