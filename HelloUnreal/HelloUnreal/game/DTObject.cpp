#include "DTObject.h"

#include "DTProcPop.h"

DTItem* dtItem;
int dtItemNum;

DTUnit** unit;
int unitNum;
int selectedUnit;
iPoint positionUnit;

// 목표 갯수, 현재 만들고 있는, 완료 갯수, 고장
int target, curr, complete, broken;

static iPoint off;

struct OrderInfo
{
	int pd;
	int num;
};

OrderInfo* oi;
int oiNum;
#define oiMax 100

iShortestPath* dtsp;

void startMake(int orderPD, int orderNum)
{
	OrderInfo* o = &oi[oiNum];
	o->pd = orderPD;
	o->num = orderNum;
	oiNum++;

	if (oiNum == 1)
	{
		target = oi[0].num;
		curr = 0;
	}
}

void loadDTObject()
{
	dtItem = new DTItem[dtItemMax];
	for (int i = 0; i < dtItemMax; i++)
		dtItem[i].pd = -1;
	dtItemNum = 0;

	// make : 0 ~ 3
	// move : 4, sida : 5, success : 6, fail : 7, 8
	unitNum = 9;
	unit = new DTUnit * [unitNum];
	// make
	for (int i = 0; i < 4; i++)
	{
		unit[i] = new DTUnitMake(i);
		unit[i]->position = iPointMake(80 + 140 * i, 140 - 30 * i);
	}
	// move
	DTUnitMove* u = new DTUnitMove(100);
	u->sp = iPointMake(60, 200);
	u->ep = iPointMake(660, 200);
	u->speed = 100.0f; // 레일 움직이는 속도
	u->_delta = 1.0f; // 물건을 컨베이어에 옮기는 시간
	u->position = (u->sp + u->ep) / 2; // 현재 위치
	unit[4] = u;
	// sida
	unit[5] = new DTUnitSida(200);
	unit[5]->position = iPointMake(devSize.width / 2, devSize.height / 2);
	// success
	unit[6] = new DTUnitSuccess(300);
	unit[6]->position = iPointMake(80, devSize.height * 0.7f);
	// fail
	for (int i = 0; i < 2; i++)
	{
		DTUnit* u = new DTUnitFail(400 + i);
		u->position = iPointMake(220 + 140 * i, devSize.height * 0.7f);
		unit[7 + i] = u;
	}

	selectedUnit = -1;
	positionUnit = iPointZero;

	target = 0;
	curr = 0;
	complete = 0;
	broken = 0;

	off = iPointZero;

	oi = new OrderInfo[oiMax];
	oiNum = 0;

	dtsp = new iShortestPath();
}

void freeDTObject()
{
	delete dtItem;

	for (int i = 0; i < unitNum; i++)
		delete unit[i];
	delete unit;

	delete oi;

	delete dtsp;
}

void drawDTObject(float dt, iPoint off)
{
	::off = off;

	for (int i = 0; i < unitNum; i++)
		unit[i]->paint(dt, off);

	if (selectedUnit == -1)
	{
		showToast(positionUnit, NULL);
	}
	else
	{
		showToast(positionUnit, "unit[%d] select\n", selectedUnit);
	}

	if (oiNum)
	{
		OrderInfo* o = &oi[0];
		if (o->num == 0)
		{
			oiNum--;
			memcpy(oi, &oi[1], sizeof(OrderInfo) * oiNum);
			target = oi[0].num;
			curr = 0;
			return;
		}
		//else //if (o->num)

		DTUnitMake* u = (DTUnitMake*)unit[0];
		int num = 0; // 주문을 받을 수 있는 개수
		for (int i = 0; i < u->slotInNum; i++)
		{
			if (u->slotIn[i] == NULL)
				num++;
		}
		// num 주문을 할 수 있는 개수
		if (oi->num <= num)
		{
			// oi->num 주문
			num = oi->num;
			oi->num = 0;
		}
		else if (oi->num > num)
		{
			// num 주문
			oi->num -= num;
		}
		//num = orderNum / num + (orderNum % num ? 1 : 0);

		for (int i = 0; i < num; i++)
		{
			//DTItem* it = &dtItem[dtItemNum + i];
			DTItem* it = NULL;
			for (int j = 0; j < dtItemMax; j++)
			{
				if (dtItem[j].pd == -1)
				{
					it = &dtItem[j];
					break;
				}
			}
			it->pd = oi->pd;
			int* p = itemPath[oi->pd];
			memcpy(it->path, &p[1], sizeof(int) * p[0]);
			it->pathNum = p[0];
			it->pathindex = 0;

			for (int j = 0; j < u->slotInNum; j++)
			{
				// 비었다면 방금 생산한 제품 넣어주기
				if (u->slotIn[j] == NULL)
				{
					u->slotIn[j] = it;
					break;
				}
			}
		}
		dtItemNum += num;
		curr += num;
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
		for (i = 0; i < unitNum; i++)
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
			delta = 0.0f;
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
	RobotInfo* m = &ri[index];

	const char* str[4] = {"대기", "생산", "고장", "완료"};
	int num[4] = { 3, 10, 2, 5 };
	iColor4f sc = iColor4fMake(0, 0, 0, 1);
	imgs = createRobot(
		index, 4,
		str, num,
		&m->size, &m->color,
		18, &sc);

	sm = StateMakeReady;

	int* n = &slotInOut[index][0];
	slotInNum = n[0];
	slotOutNum = n[1];

	slotIn = new DTItem*[slotInNum];
	memset(slotIn, 0x00, sizeof(DTItem*) * slotInNum);
	slotOut = new DTItem * [slotOutNum];
	memset(slotOut, 0x00, sizeof(DTItem*) * slotOutNum);

	n = &slotInOut[index][2];
	makeInNum = n[0];
	makeOutNum = n[1];
	makeSlotIn = new int[slotInNum];

	_delta = m->delta;
}

DTUnitMake::~DTUnitMake()
{
	for (int i = 0; i < StateMakeMax; i++)
		delete imgs[i];
	delete imgs;
	img = NULL; // 부모 멤버는 해제하면안됨

	delete slotIn;
	delete slotOut;
	delete makeSlotIn;
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
	iPoint p = this->position + position;
	img->paint(dt, p);

	p.x += img->position.x;
	// 생산재료
	for (int i = 0; i < slotInNum; i++)
	{
		setRGBA(0, 0, 0, 1);
		fillRect(p.x + 12 * i, p.y + 5, 10, 10);
		if (slotIn[i])
		{
			setRGBA(1, 1, 0, 1);
			fillRect(p.x + 1 + 12 * i, p.y + 1 + 5, 8, 8);
		}
	}
	// 생산하고 완료된
	for (int i = 0; i < slotOutNum; i++)
	{
		setRGBA(0, 0, 0, 1);
		fillRect(p.x + 12 * i, p.y + 20, 10, 10);
		if (slotOut[i])
		{
			setRGBA(0, 1, 1, 1);
			fillRect(p.x + 1 + 12 * i, p.y + 1 + 20, 8, 8);
		}
	}
	setRGBA(1, 1, 1, 1);
	
	// ctrl

	// 물건 싣는중...
	float r = run(dt);

	// 진행 상태를 나타내는 프로그레스바
	if (r) // 생산이 완료가 되더라도 디스플레이
	{
		// 생산 프로그래스 바
		p = iPointMake(img->position.x, 0) +
			this->position + position + iPointMake(0, 35);

		setRGBA(0, 0, 0, 1);
		fillRect(p.x, p.y, img->tex->width, 10);
		setRGBA(1, 0, 0, 1);
		fillRect(p.x + 2, p.y + 2, r * (img->tex->width - 4), 10 - 4);
		setRGBA(1, 1, 1, 1);
	}
	else //if (r == 0.0f) // 쉬고있으니 생산시작 해야함
	{
		int inNum = 0;
		for (int i = 0; i < slotInNum; i++)
		{
			if (slotIn[i])
				inNum++;
		}
		if (inNum < makeInNum)
			return;

		int outNum = 0;
		for (int i = 0; i < slotOutNum; i++)
		{
			if (slotOut[i] == NULL)
				outNum++;
		}

		if (outNum < makeOutNum)
			return;

		inNum = 0;
		for (int i = 0; i < slotInNum; i++)
		{
			if (slotIn[i])
			{
				makeSlotIn[inNum] = i;
				inNum++;
				if (inNum == makeInNum)
					break;
			}
		}

		MethodWorked m[4] = {
				cbWorked0, cbWorked1,
				cbWorked2, cbWorked3
		};
		start(m[index]);
	}
}

void DTUnitMake::cbWorked0(DTUnit* obj)
{
	DTUnitMake* dm = (DTUnitMake*)obj;

	// 실 데이터를 반영
	printf("DTUnitMake[0] 생산완료 => UnitMove\n");
	dm->sm = StateMakeReady;

	// 추가적으로 만들어야 하는 DTItem
	DTItem** input;
	if (dm->makeInNum == dm->makeOutNum)
	{
		input = new DTItem * [dm->makeInNum];
		for (int i = 0, j = 0; i < dm->slotInNum; i++)
		{
			if (dm->slotIn[i])
			{
				input[j] = dm->slotIn[i];
				j++;
			}
		}
	}
	else if (dm->makeInNum < dm->makeOutNum)
	{
		// 증가
		DTItem* dti = NULL;
		input = new DTItem * [dm->makeOutNum];
		for (int i = 0, j = 0; i < dm->slotInNum; i++)
		{
			if (dm->slotIn[i])
			{
				dti = 
				input[j] = dm->slotIn[i];
				j++;
			}
		}
		int add = dm->makeOutNum - dm->makeInNum;
		for (int i = 0; i < add; i++)
		{
			for (int j = 0; j < dtItemMax; j++)
			{
				if (dtItem[j].pd == -1)
				{
					memcpy(&dtItem[j], dti, sizeof(DTItem));
					input[dm->makeInNum + i] = &dtItem[j];
					break;
				}
			}
		}
	}
	else
	{
		// 감소
		input = new DTItem * [dm->makeInNum];
		for (int i = 0, j = 0; i < dm->slotInNum; i++)
		{
			if (dm->slotIn[i] == NULL)
				continue;

			if (j < dm->slotOutNum)
			{
				input[j] = dm->slotIn[i];
				j++;
			}
			else
			{
				dm->slotIn[i]->pd = -1;
			}
		}
	}

	for (int i = 0, j = 0; i < dm->slotOutNum; i++)
	{
		if (dm->slotOut[i] == NULL)
		{
			dm->slotOut[i] = input[j];
			j++;
			if (j == dm->makeOutNum)
				break;
		}
	}

	delete input;
	//for (int i = 0, j = 0; i < dm->slotInNum; i++)
	//{
	//	if (dm->slotIn[i])
	//	{
	//		dm->slotIn[i] = NULL;
	//		j++;
	//		if (j == dm->makeInNum)
	//			break;
	//	}
	//}

	for (int i = 0; i < dm->makeInNum; i++)
	{
		int j = dm->makeSlotIn[i];
		dm->slotIn[j] = NULL;
	}
}

void DTUnitMake::cbWorked1(DTUnit* obj)
{
	cbWorked0(obj);
	return;

	DTUnitMake* dm = (DTUnitMake*)obj;

	// 실 데이터를 반영
}

void DTUnitMake::cbWorked2(DTUnit* obj)
{
	cbWorked0(obj);

	DTUnitMake* dm = (DTUnitMake*)obj;

	// 실 데이터를 반영
}

void DTUnitMake::cbWorked3(DTUnit* obj)
{
	cbWorked0(obj);
	DTUnitMake* dm = (DTUnitMake*)obj;

	// 실 데이터를 반영
	complete++;
	
}

// =======================================
// DTUnitMove
// =======================================

DTUnitMove::DTUnitMove(int index) : DTUnit(index)
{
	index -= 100; // 0 ~ 99
	// index == 0 : 가로로 움직이는 로봇
	// index == 1 : 세로로 움직이는 로봇
	RobotInfo* m = &ri[4];

	const char* str[3] = { "대기", "이동", "싣기" };
	int num[3] = { 3, 1, 10 };
	iColor4f sc = iColor4fMake(1, 1, 1, 1);
	imgs = createRobot(
		index, 3,
		str, num,
		&m->size, &m->color,
		15, &sc);

	sm = StateMoveReady;

	sp = iPointZero;
	ep = iPointZero;

	have = NULL;
}

DTUnitMove::~DTUnitMove()
{
	for (int i = 0; i < StateMoveMax; i++)
		delete imgs[i];
	delete imgs;
	img = NULL;
}

void DTUnitMove::setAreaRange(iPoint sp, iPoint ep, float speed)
{
	this->sp = sp;
	this->ep = ep;
	this->speed = speed;
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

	if (sm == StateMoveReady)
	{
		// 옮겨야할 물건을 찾아간다
		DWORD min = 0xffffffff;
		int unitIndex = -1;
		for (int i = 0; i < unitNum; i++)
		{
			//if (unit[i]->index < 100)
			if (unit[i]->index < 3) // 제품 완료가 된, 마지막 공정 제외
			{
				DTUnitMake* u = (DTUnitMake*)unit[i];
				for (int j = 0; j < u->slotOutNum; j++)
				{
					DTItem* it = u->slotOut[j];
					if (it == NULL) continue;
					if (min > it->makeEnd)
					{
						min = it->makeEnd;
						unitIndex = i;
					}
				}
			}
		}

		if (unitIndex != -1)
		{
			// 이 위치로 가라!
			tPosition.x = unit[unitIndex]->position.x;
			tPosition.y = this->position.y;
			this->unitIndex = unitIndex;

			sm = StateMoveMove;
		}
	}
	else if (sm == StateMoveMove)
	{
		// 도착하고 물건을 싣는다
		iPoint mp = tPosition - this->position;
		mp.loadIdentity();
		mp = mp * (speed * dt);
		if (move(&this->position, &tPosition, mp))
		{
			sm = StateMovePick;
			start(cbWorked);
		}
	}
	else if (sm == StateMovePick)
	{
		// 물건 싣는중...
		float r = run(dt);
		// r에 따라 싣는중인 애니메이션 디스플레이
		
		// 콜백 : cbWorked
		// 진행 상태를 나타내는 프로그레스바
		if (r) // 생산이 완료가 되더라도 디스플레이
		{
			// 생산 프로그래스 바
			iPoint p = iPointMake(img->position.x, 0) +
				this->position + position + iPointMake(0, 35);

			setRGBA(0, 0, 0, 1);
			fillRect(p.x, p.y, img->tex->width, 10);
			setRGBA(1, 0, 0, 1);
			fillRect(p.x + 2, p.y + 2, r * (img->tex->width - 4), 10 - 4);
			setRGBA(1, 1, 1, 1);
		}
	}
}

void DTUnitMove::cbWorked(DTUnit* obj)
{
	DTUnitMove* dm = (DTUnitMove*)obj;

	// 물건 내리기
	if (dm->have)
	{
		DTItem* it = dm->have;
		int index = it->path[it->pathindex];
		DTUnitMake* u = (DTUnitMake*)unit[index];
		for (int i = 0; i < u->slotInNum; i++)
		{
			if (u->slotIn[i] == NULL)
			{
				u->slotIn[i] = dm->have;
				dm->have = NULL;

				dm->sm = StateMoveReady;
				break;
			}
		}
	}
	// 물건 싣기
	else //if (have == NULL) // 빈손일 경우
	{
		DTUnitMake* u = (DTUnitMake*)unit[dm->unitIndex];
		DWORD min = 0xFFFFFFFF;
		int index = -1;
		for (int i = 0; i < u->slotOutNum; i++)
		{
			DTItem* it = u->slotOut[i];
			if (it == NULL) continue;
			if (min > it->makeEnd)
			{
				min = it->makeEnd;
				index = i;
			}
		}
		dm->have = u->slotOut[index];
		u->slotOut[index] = NULL;

		// go
		dm->have->pathindex++;
		int targetIndex = dm->have->path[dm->have->pathindex];
		dm->tPosition.x = unit[targetIndex]->position.x;
		dm->tPosition.y = dm->tPosition.y;

		dm->sm = StateMoveMove;
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

// ===========================
// DTSida
// ===========================
DTUnitSida::DTUnitSida(int index) : DTUnit(index)
{
	index -= 200; // 0 ~ 99
	RobotInfo* m = &ri[5];

	// 기본적으로 왼쪽으로 쳐다보는 로봇(상/하)
	const char* str[4] = { "대기", "이동(상)", "이동(하)", "싣기"};
	int num[4] = {3, 4, 4, 5};
	iColor4f sc = iColor4fMake(0, 0, 0, 1);
	imgs = createRobot(
		index, 4,
		str, num,
		&m->size, &m->color,
		8, &sc);

	sm = StateMoveReady;

	path = new iPoint[32 * 24];
	pathNum = 0;
}

DTUnitSida::~DTUnitSida()
{
	for (int i = 0; i < StateMoveMax; i++)
		delete imgs[i];
	delete imgs;
	img = NULL;

	delete path;
}

bool DTUnitSida::start(MethodWorked m)
{

	return false;
}

void DTUnitSida::paint(float dt, iPoint position)
{
	img = imgs[sm];
	img->paint(dt, this->position + position);

	setLineWidth(2);
	setRGBA(1, 1, 1, 1);
	int j = pathNum - 1;
	for (int i = 0; i < j; i++)
	{
		drawLine(path[i] + position, path[i + 1] + position);
	}
}

void DTUnitSida::cbWorked(DTUnit* obj)
{

}

// ===========================
// DTUnitSuccess
// ===========================
DTUnitSuccess::DTUnitSuccess(int index) : DTUnit(index)
{
	index -= 300;

	RobotInfo* m = &ri[6];
	const char* str[1] = { "대기" };
	int num[1] = { 3, };
	iColor4f sc = iColor4fMake(0, 0, 0, 1);
	iImage** imgs = createRobot(
		index, 1,
		str, num,
		&m->size, &m->color,
		15, &sc);

	img = imgs[0];
	delete imgs;
}

DTUnitSuccess::~DTUnitSuccess()
{
	// 슈퍼클래스에서 파괴할것임
}

void DTUnitSuccess::paint(float dt, iPoint position)
{
	img->paint(dt, this->position + position);
}

// ===========================
// DTUnitFail
// ===========================
DTUnitFail::DTUnitFail(int index) : DTUnit(index)
{
	index -= 400;

	RobotInfo* m = &ri[6];
	const char* str[1] = { "대기" };
	int num[1] = { 3, };
	iColor4f sc = iColor4fMake(0, 0, 0, 1);
	iImage** imgs = createRobot(
		index, 1,
		str, num,
		&m->size, &m->color,
		15, &sc);

	img = imgs[0];
	delete imgs;
}

DTUnitFail::~DTUnitFail()
{

}

void DTUnitFail::paint(float dt, iPoint position)
{
	img->paint(dt, this->position + position);
}

iImage** createRobot(
	int index, int beNum,
	const char** strBe, int* beAniNum, 
	iSize* s, iColor4f* c, 
	float ss, iColor4f* sc)
{
	iImage** imgs = new iImage * [beNum];
	setRGBA(c->r, c->g, c->b, c->a);

	setStringSize(ss);
	setStringRGBA(sc->r, sc->g, sc->b, sc->a);

	iGraphics* g = iGraphics::getInstance();
	for (int i = 0; i < beNum; i++)
	{
		iImage* img = new iImage();
		for (int j = 0; j < beAniNum[i]; j++)
		{
			g->init(s->width, s->height);

			g->fillRect(0, 0, s->width, s->height);
			g->drawString(s->width / 2, s->height / 2 - 10, VCENTER | HCENTER,
				"index%d", index);
			g->drawString(s->width / 2, s->height / 2 + 10, VCENTER | HCENTER,
				"%s%d", strBe[i], j);

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

	return imgs;
}

RobotInfo ri[8] = {
	// 0 ~ 3 : Make
	{{80, 40},		{1, 0, 0, 1}, 3},
	{{120, 40},		{0, 1, 0, 1}, 2},
	{{80, 160},		{0, 0, 1, 1}, 2 },//1000},
	{{120, 120},	{1, 1, 0, 1}, 10 },//500},
	// 4 : Move
	{{50, 50},		{0, 0, 0, 1}, 200 },//200},
	// 5 : Sida
	{{30, 30},		{0.5, 0.5, 0.5, 1}, 1},
	// 6 : Success
	{{120, 60},		{0.25, 0.25, 0.25, 1}, 0},
	// 7 : Fail
	{{120, 60},		{0.25, 0.0, 0.0, 1}, 100},

};

// 0 : 쇠 -> 볼트, 너트
// 1 : 도색 -> 은볼트 -> 검은볼트
// 2 : 도색 -> 은볼트 -> 흰볼트
// 3 : 포장 -> 
int itemPath[6][4] = {
			{2,   0, 3,	 }, // 실버볼트
			{2,   0, 3,	 }, // 실버너트
			{3,   0, 1, 3}, // 블랙볼트
			{3,   0, 1, 3}, // 블랙너트
			{3,   0, 2, 3}, // 화이트볼트
			{3,   0, 2, 3}, // 화이트너트
};

// 저장소 + 재료
int slotInOut[4][4] = {
	{3, 20	,1 ,5}, // DTItem의 재료저장 슬롯개수, 생산저장 슬롯개수
	{10, 10	,1, 1},
	{10, 10	,1, 1},
	{10, 2	,3, 1},
};