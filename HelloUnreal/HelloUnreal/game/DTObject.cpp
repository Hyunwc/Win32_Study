#include "DTObject.h"

#include "DTProcPop.h"

DTItem* dtItem;
int dtItemNum;

DTUnit** unit;
int unitNum;
int selectedUnit;
iPoint positionUnit;

// 목표 갯수, 현재 만들고 있는, 완료 갯수
int target, curr, complete, broken;

void startMake(int orderPD, int orderNum)
{
	// 주문 우선 순위 나중에 처리...'
	int path[6][4] = {
		{2,   0, 3}, // 실버볼트
		{2,   0, 3}, // 실버너트
		{3,   0, 1, 3}, // 블랙볼트
		{3,   0, 1, 3}, // 블랙너트
		{3,   0, 2, 3}, // 화이트볼트
		{3,   0, 3, 3}, // 화이트너트
	};
	int num;
	if (orderPD % 2 == 0) num = 3; // bolt
	else				  num = 4; // nut
	num = orderNum / num + (orderNum % num ? 1 : 0);

	for (int i = 0; i < num; i++)
	{
		DTItem* it = &dtItem[dtItemNum + i];
		it->pd = orderPD;
		memcpy(it->path, &path[orderPD][1], sizeof(int) *
			path[orderPD][0]);
		it->pathNum = path[orderPD][0];
		it->pathindex = 0;

		DTUnitMake* u = (DTUnitMake*)unit[0];
		for (int j = 0; j < u->slotInputNum; j++)
		{
			if (u->slotInput[j] == NULL)
			{
				u->slotInput[j] = it;
				break;
			}
		}
	}

	dtItemNum += num;

	target = orderNum; 
	curr = 0;
	complete = 0;
	broken = 0;
}

#include "DTProc.h"

void startMove(int unitIndex)
{
	// 가로나 세로로 움직이는 위치 확인 
	const iPoint& pSrc = unit[unitIndex]->position;
	int location = 1;
	if (pSrc.y < FieldHeight * 0.3f)
		location = 0;
	else if (pSrc.y > FieldHeight * 0.7f)
		location = 2;

	for (int i = 0; i < unitNum; i++)
	{
		if (unit[i]->index / 100 == 1) // 이동하는 로봇
		{
			int loc = 1;
			const iPoint& pDst = unit[i]->position;
			if (pSrc.y < FieldHeight * 0.3f)
				loc = 0;
			else if (pSrc.y > FieldHeight * 0.7f)
				loc = 2;
			if (location == loc)
			{
				DTUnitMove* dm = (DTUnitMove*)unit[i];
				iPoint pDst = iPointMake(pSrc.x, dm->position.y);
				dm->tp[dm->tpNum] = pDst;
				dm->tpNum++;

				dm->havePD = true;
				break;
			}
		}
	}
}

void loadDTObject()
{
	unit = new DTUnit * [6];
	for (int i = 0; i < 5; i++)
	{
		unit[i] = new DTUnitMake(i);
		unit[i]->position = iPointMake(80 + 140 * i, 140);
	}
	
	DTUnitMove* u = new DTUnitMove(100);
	u->sp = iPointMake(80, 170);
	u->ep = iPointMake(660, 170);
	u->speed = 100.0f; // 레일 움직이는 속도
	u->_delta = 1.0f; // 물건을 컨베이어에 옮기는 시간
	u->position = (u->sp + u->ep) / 2; // 현재 위치
	unit[5] = u;

	selectedUnit = -1;
	positionUnit = iPointZero;

	unitNum = 6;

	target = 0;
	curr = 0;
}

void freeDTObject()
{
	for (int i = 0; i < 6; i++)
		delete unit[i];
	delete unit;
}

static iPoint off;
void drawDTObject(float dt, iPoint off)
{
	::off = off;

	for (int i = 0; i < 6; i++)
		unit[i]->paint(dt, off);

	if (selectedUnit == -1)
	{
		showToast(positionUnit, NULL);
	}
	else
	{
		showToast(positionUnit, "unit[%d] select\n", selectedUnit);
	}

	if (curr < target)
	{
		if (unit[0]->start(DTUnitMake::cbWorked0))
			curr++;
	}
}

bool keyDTObject(iKeyStat stat, iPoint point)
{
	int i, j = -1;
	switch (stat)
	{
	case iKeyStatBegan:
		break;
	case iKeyStatMoved:
		for (i = 0; i < 5; i++)
		{
			if (containPoint(point, unit[i]->touchRect(off)))
			{
				j = i;
				break;
			}
		}
		selectedUnit = j;
		positionUnit = point;
		break;
	case iKeyStatEnded:
		break;
	}
	return false;
}

// =======================================
// DTUnit
// =======================================
DTUnit::DTUnit(int index)
{
	this->index = index;
	img = NULL;
	position = iPointZero;
	_delta = 0.0f;
	delta = 0.0f;
}

DTUnit::~DTUnit()
{
	if(img)
		delete img;
}

bool DTUnit::start(MethodWorked m)
{
	// 값이 있을땐 명령불가
	if (delta)// && delta < _delta)
		return false;

	delta = 0.000001f;
	methodWorked = m;
	return true;
}

float DTUnit::run(float dt)
{
	// 0초일땐 대기상태. 아래 조건에서 시간이 작다고
	// 바로 생산이되면 안되기때문에 예외처리
	if (delta == 0.0f)
		return 0.0f;
	// ctrl
	if (delta < _delta) // 물건을 싣는중...
	{
		delta += dt;
		float r = delta / _delta; // 진행중 비율
		// r에 따라 싣는중인 애니메이션 디스플레이
		if (delta >= _delta)
		{
			methodWorked(this); // 일이 끝났을때 콜백호출
		}
		if (r > 1.0f)
			r = 1.0f;
		return r;
	}
	return 1.0f;
}

iRect DTUnit::touchRect(iPoint position)
{
	return img->touchRect(this->position + position);
}

// =======================================
// DTUnitMake
// =======================================
// w : 80 ~ 120, h : 40 ~ 160
DTUnitMake::DTUnitMake(int index) : DTUnit(index)
{
	MakeInfo* m = &mi[index];

	const char* str[4] = {"대기", "생산", "고장", "완료"};
	int num[4] = { 3, 10, 2, 5 };
	imgs = new iImage * [StateMakeMax];

	iColor4f* c = &m->color;
	setRGBA(c->r, c->g, c->b, c->a);

	setStringSize(18);
	setStringRGBA(0, 0, 0, 1);

	iGraphics* g = iGraphics::getInstance();
	iSize* s = &m->size;
	for (int i = 0; i < 4; i++)
	{
		img = new iImage();
		for (int j = 0; j < num[i]; j++)
		{
			g->init(s->width, s->height);

			g->fillRect(0, 0, s->width, s->height);
			g->drawString(s->width / 2, s->height / 2 - 10, VCENTER | HCENTER,
				"index%d", index);
			g->drawString(s->width / 2, s->height / 2 + 10, VCENTER | HCENTER,
				"%s%d", str[i], j);

			Texture* tex = g->getTexture();
			g->clean();
			img->add(tex);
			freeImage(tex);
		}
		img->position = iPointMake(-s->width / 2, -s->height);
		img->_aniDt = 0.17f;
		img->startAnimation();
		imgs[i] = img;
	}
	
	sm = StateMakeReady;

	int slotNum[4][2] = {
		{3, 20}, // DTItem의 재료저장 슬롯개수, 생산저장 슬롯개수
		{10, 10},
		{10, 10},
		{10, 2},
	};

	slotInputNum = slotNum[index][0];
	slotOutputNum = slotNum[index][1];

	slotInput = new DTItem*[10];
	memset(slotInput, NULL, sizeof(DTItem) * 10);
	slotOutput = new DTItem * [10];
	memset(slotOutput, NULL, sizeof(DTItem) * 10);

	_delta = m->delta;
}

DTUnitMake::~DTUnitMake()
{
	for (int i = 0; i < StateMakeMax; i++)
		delete imgs[i];
	delete imgs;
	img = NULL; // 부모 멤버는 해제하면안됨

	delete slotInput;
	delete slotOutput;
}

bool DTUnitMake::start(MethodWorked m)
{
	// 최상위 클래스의 start 호출(Super::)
	// result -> 값이 달라질 수 있으니 변수에 넣어서 반환
	bool result = DTUnit::start(m);

	// TODO : 코드를 추가
	sm = StateMakeMake;

	return result;
}

void DTUnitMake::paint(float dt, iPoint position)
{
	img = imgs[sm]; 
	//setRGBA(1, 0, 1, 1);
	//iPoint p = this->position + position;
	//fillRect(p.x, p.y, 50, 50);
	//setRGBA(1, 1, 1, 1);
	// 현재위치 + 스크롤값
	img->paint(dt, this->position + position);

	// ctrl

	// 물건 싣는중...
	float r = run(dt);

	// 진행 상태를 나타내는 프로그레스바
	if (r) // 생산이 완료가 되더라도 디스플레이
	{
		iPoint p = iPointMake(img->position.x, 0) +
			this->position + position + iPointMake(0, 10);
		setRGBA(0, 0, 0, 1);
		fillRect(p.x, p.y, img->tex->width, 10);
		setRGBA(1, 0, 0, 1);
		fillRect(p.x + 2, p.y + 2, r * (img->tex->width - 4), 10 - 4);
		setRGBA(1, 1, 1, 1);
	}

	// r에 따라 프로그레스바 애니메이션 디스플레이
	if (r < 1.0f)
		return;

	// 다음 생산을 진행
	// 대기 슬롯이 가득찼을때(생산 준비가 되었을때)
	MethodWorked m[6] = {
		cbWorked0, cbWorked1, cbWorked2,
		cbWorked3, cbWorked4, cbWorked5
	};
	start(m[index]);
}

void DTUnitMake::cbWorked0(DTUnit* obj)
{
	DTUnitMake* dm = (DTUnitMake*)obj;

	// 실 데이터를 반영
	printf("DTUnitMake[0] 생산완료 => UnitMove\n");
	startMove(dm->index);
}

void DTUnitMake::cbWorked1(DTUnit* obj)
{
	DTUnitMake* dm = (DTUnitMake*)obj;

	// 실 데이터를 반영
}

void DTUnitMake::cbWorked2(DTUnit* obj)
{
	DTUnitMake* dm = (DTUnitMake*)obj;

	// 실 데이터를 반영
}

void DTUnitMake::cbWorked3(DTUnit* obj)
{
	DTUnitMake* dm = (DTUnitMake*)obj;

	// 실 데이터를 반영
}

void DTUnitMake::cbWorked4(DTUnit* obj)
{
	DTUnitMake* dm = (DTUnitMake*)obj;

	// 실 데이터를 반영
}

void DTUnitMake::cbWorked5(DTUnit* obj)
{
	DTUnitMake* dm = (DTUnitMake*)obj;

	// 실 데이터를 반영
}

void DTUnitMake::cbWorked9(DTUnit* obj)
{
	DTUnitMake* dm = (DTUnitMake*)obj;

	// 실 데이터를 반영
	complete++;
}

MakeInfo mi[5] = {
	{{80, 40}, {1, 0, 0, 1}, 3},
	{{120, 40}, {0, 1, 0, 1}, 200},
	{{80, 160}, {0, 0, 1, 1}, 1000},
	{{120, 120}, {1, 1, 0, 1}, 500},
	{{80, 80}, {1, 0, 1, 1}, 200},
};

// =======================================
// DTUnitMove
// =======================================

DTUnitMove::DTUnitMove(int index) : DTUnit(index)
{
	index -= 100; // 0 ~ 99
	// index == 0 : 가로로 움직이는 로봇
	// index == 1 : 세로로 움직이는 로봇

	const char* str[3] = { "대기", "이동", "싣기" };
	int num[3] = { 3, 1, 10 };

	setStringSize(15);
	setStringRGBA(1, 1, 1, 1);
	iGraphics* g = iGraphics::getInstance();
	imgs = new iImage * [StateMoveMax];
	iSize s = iSizeMake(50, 50);

	for (int i = 0; i < StateMoveMax; i++)
	{
		img = new iImage();

		for (int j = 0; j < num[i]; j++)
		{
			g->init(s.width, s.height);

			setRGBA(0, 0, 0, 1);
			g->fillRect(0, 0, s.width, s.height);
			setRGBA(1, 1, 0, 1);
			setLineWidth(2);
			g->drawRect(5, 5, s.width - 10, s.height - 10);

			g->drawString(s.width / 2, s.height/2, VCENTER | HCENTER, "%s%d", str[i], j);

			Texture* tex = g->getTexture();
			g->clean();
			img->add(tex);
			freeImage(tex);
		}
		img->position = iPointMake(-s.width / 2, -s.height / 2);
		img->_aniDt = 0.17f;
		img->startAnimation();
		imgs[i] = img;
	}

	sm = StateMoveReady;

	sp = iPointZero;
	ep = iPointZero;

	tp = new iPoint[100];
	tpNum = 0;

	havePD = false;
}

DTUnitMove::~DTUnitMove()
{
	for (int i = 0; i < StateMoveMax; i++)
		delete imgs[i];
	delete imgs;
	img = NULL;
	// 자식이 만든것은 자식이 삭제
	delete tp;
}

void DTUnitMove::setAreaRange(iPoint sp, iPoint ep, float speed)
{
	this->sp = sp;
	this->ep = ep;
	this->speed = speed;
}

void DTUnitMove::cbWorked(DTUnit* obj)
{
	DTUnitMove* dm = (DTUnitMove*)obj;

	dm->delta = 0.0f;

	// 만든 물건을 컨베이어 싣기 => 그 다음 공정으로 이동
	if (dm->havePD)
	{
		dm->havePD = false;

		int index = 0; // 무조건 첫번째가 아님
		DTUnit* u = unit[index]; 
		u->delta = 0.0f; // 상태변경을 위해서

		MethodWorked m[] = {
			DTUnitMake::cbWorked0, DTUnitMake::cbWorked1, DTUnitMake::cbWorked2,
			DTUnitMake::cbWorked3, DTUnitMake::cbWorked4, DTUnitMake::cbWorked5,
			DTUnitMake::cbWorked9 
		};

		u->start(m[index]);

		index = 1; // 무조건 두번째가 아님
		u = unit[index];

		iPoint pDst = iPointMake(u->position.x, dm->position.y); // 다음 공정 위치
		dm->tp[dm->tpNum] = pDst;
		dm->tpNum++;
	}
	// 컨베이어 있는 물건을 생산로봇에 전달 => 대기
	else
	{
		// 컨베이어 대기상태
		dm->sm = StateMoveReady;

		int index = 1; // 무조건 두번째가 아님
		//DTUnit* u = unit[index];

		// 다음 공정 생산 시작
		//u->start(DTUnitMake::cbWorked1);
		DTUnitMake* u = (DTUnitMake*)unit[index];

		//for (int i = 0; i < 10; i++)
		//{
		//	if (u->slot[i] == false)
		//	{
		//		u->slot[i] = true;
		//		break;
		//	}
		//}
	}

}

bool DTUnitMove::start(MethodWorked m)
{
	bool result = DTUnit::start(m);

	sm = StateMovePick;

	return result;
}

void DTUnitMove::paint(float dt, iPoint position)
{
	// path-line (이동 경로)
	iPoint s = sp + position;
	iPoint e = ep + position;

	setRGBA(0, 0, 0, 1);
	setLineWidth(10);
	drawLine(s, e);
	setRGBA(1, 1, 0, 1);
	setLineWidth(4);
	drawLine(s + iPointMake(2, 2), e - iPointMake(2, 2));
	setRGBA(1, 1, 1, 1);

	img = imgs[sm];
	img->paint(dt, this->position + position);

	// ctrl

	// 물건 싣는중...
	float r = run(dt);
	// r에 따라 싣는중인 애니메이션 디스플레이
	if (r && r < 1.0f)
		return;

	if (tpNum) // 값이있으니 이동해야함
	{
		sm = StateMoveMove;

		iPoint& curr = this->position;
		iPoint& tPosition = tp[0];
		iPoint mp = tPosition - curr;
		mp.loadIdentity();
		mp = mp * (speed * dt);
		if (move(&curr, &tPosition, mp))
		{
			tpNum--;
			memcpy(tp, &tp[1], sizeof(iPoint) * tpNum);
			// methodArrive
			// 도착한 곳에서 컨베이어에 물건을 싣는다.
			start(cbWorked);
		}
	}
	else
	{

	}
}

Texture* createTDObject(const iSize& s, 
	const iColor4f& bs, const iColor4f& cs, const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szFormat, szText);

	int lineNum;
	char** line = iString::split(lineNum, szText);

	float stringSize = getStringSize();
	float y = s.height - stringSize / 2;
	if (lineNum > 1)
		y -= (stringSize + 5) * (lineNum - 1);

	iGraphics* g = iGraphics::getInstance();

	g->init(s.width, s.height);

	setRGBA(bs.r, bs.g, bs.b, bs.a);
	g->fillRect(0, 0, s.width, s.height);
	setStringRGBA(cs.r, cs.g, cs.b, cs.a);
	for (int i = 0; i < lineNum; i++)
		g->drawString(s.width / 2, y + (stringSize + 5) * i, VCENTER | HCENTER, line[i]);

	iString::free(line, lineNum);

	return g->getTexture();
}

