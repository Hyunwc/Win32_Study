#pragma once

#include "iStd.h"

// target : 생산 목표 개수(수주 개수)
// curr : (생산중 + 생산완료)
// complete : 생산완료
// broken : 불량품 개수

// 디스플레이 
// 생산률 complete / target
// 진행률 curr / target
extern int curr, complete, broken;

enum Material
{
	Stain = 0,

	Bolt,
	BoltSilver = Bolt,
	BoltBlack, BoltWhite,
	BoltkSilverPack, BoltBlackPack, BoltWhitePack,

	Nut,
	NutSilver = Nut,
	NutBlack, NutWhite,
	NutkSilverPack, NutBlackPack, NutWhitePack,
};

// DTUnitMake

struct DTItem
{
	int pd;
	int path[3];
	int pathindex, pathNum;
	// 생산시간
	DWORD makeStart, makeEnd; 
};

extern DTItem* dtItem;
extern int dtItemNum;
#define dtItemMax 1000

struct DTUnit;
extern DTUnit** unit;
extern int unitNum;

void loadDTObject();
void freeDTObject();
void drawDTObject(float dt, iPoint off);
bool keyDTObject(iKeyStat stat, iPoint point);

struct OrderInfo
{
	int pd;
	int num, _num; // _num : 주문받은 개수
};

extern OrderInfo* oi;
extern int oiNum;
#define oiMax 100

extern iShortestPath* dtsp;

extern OrderInfo* oiBk;
extern int oiNumBk;

// DTProcUI에서 쓰기위함
void startMake(int orderPD, int orderNum);

typedef void (*MethodWorked)(DTUnit* obj);

struct DTUnit
{
	DTUnit(int index);
	virtual ~DTUnit();

	virtual bool start(MethodWorked m); // 생산시작을 알려줘
	virtual void paint(float dt, iPoint position) = 0;
	void displaySida(float dt, iPoint position);

	float run(float dt);
	// 파라미터 : offset(스크롤된 값)
	iRect touchRect(iPoint position);

	// 0~99 : 생산 로봇
	// 100~199 : 운반 로봇
	// 200~299 : 수리 로봇
	int index;
	// 현재 사용하고 있는 이미지
	iImage* img;
	// 로봇의 기준(월드) 좌표(img->pos:축좌표)
	// offMap : 카메라좌표
	iPoint position; 
	// 수행시간
	float delta, _delta; 
	// 수리로봇이 도착지
	iPoint positionSida;

	// 몇번 실행됐는지
	int exe;

	MethodWorked methodWorked;
};

// index == 0
// 수주가 있을경우 생산시작

// 그외
// index - 1 프로세서에서 생산된 물건 있으면 생산시작

// index == max - 1
// 생산완료되면, 수주 채워짐
enum StateMake
{
	StateMakeReady = 0,
	StateMakeMake,
	StateMakeBroken,
	StateMakeComplete,

	StateMakeMax
};

// index : 0 ~ 99
// delta == 0.0f : 명령 받을 준비
// delta < _delta : 생산중 & 명령 받을 수 없는
// delta >= _delta : 생산완료 & 명령 받을 수 없는
struct DTUnitMake : DTUnit
{
	iImage** imgs; // img = imgs[sm];
	StateMake sm;

	// 재료 담는 곳, 완료품 담는곳
	DTItem** slotIn, **slotOut; 
	int slotInNum, slotOutNum;
	// 재료
	int makeInNum, makeOutNum; // 들어가는 재료, 만들어진 상품
	int* makeSlotIn; // 제작에 들어간 슬롯?

	DTUnitMake(int index);
	virtual ~DTUnitMake();

	virtual bool start(MethodWorked m) override; // 생산시작을 알려줘
	virtual void paint(float dt, iPoint position);
	
	static void cbWorked0(DTUnit* obj); // 볼트,너트
	static void cbWorked1(DTUnit* obj); // 블랙 도색
	static void cbWorked2(DTUnit* obj); // 화이트 도색
	static void cbWorked3(DTUnit* obj); // 포장
};

enum StateMove
{
	StateMoveReady = 0,
	StateMoveMove,
	StateMovePick,

	StateMoveMax
};

// index : 100 ~ 199
struct DTUnitMove : DTUnit
{
	iImage** imgs;
	StateMove sm;
	DTUnitMove(int index);
	virtual ~DTUnitMove();

	void setAreaRange(iPoint sp, iPoint ep, float speed);

	virtual bool start(MethodWorked m) override; // 생산시작을 알려줘
	virtual void paint(float dt, iPoint position);

	static void cbWorked(DTUnit* obj);

	iPoint sp, ep;
	iPoint tPosition;
	int unitIndex;
	float speed;

	// 생산된지 오래된 기준으로 사용할 것
	DTItem* have;
};

// index : 200 ~ 299
struct DTUnitSida : DTUnit
{
	iImage** imgs;
	StateMove sm;

	iPoint* path;
	int pathIndex, pathNum;
	float speed;

	int targetUnit;
	DTItem* have;

	DTUnitSida(int index);
	virtual ~DTUnitSida();

	virtual bool start(MethodWorked m) override; // 생산시작을 알려줘
	virtual void paint(float dt, iPoint position);
	static void cbWorked(DTUnit* obj);
};

extern iShortestPath* dtsp;

// index : 300 ~ 399
struct DTUnitSuccess : DTUnit
{
	DTItem** have;
	int haveNum, _haveNum;

	DTUnitSuccess(int index);
	virtual ~DTUnitSuccess();

	virtual void paint(float dt, iPoint position);
};

// index : 400 ~ 499
struct DTUnitFail : DTUnit
{
	DTUnitFail(int index);
	virtual ~DTUnitFail();

	virtual void paint(float dt, iPoint position);
};

Texture* createTDObject(const iSize& s, const iColor4f& bs, const iColor4f& cs, const char* szFormat, ...);

// ==================================
// 로봇 이미지 생성
// ==================================
iImage** createRobot(
	int index, int beNum,
	const char** strBe, int* beAniNum,
	iSize* s, iColor4f* c,
	float ss, iColor4f* sc);

// ==================================
// 로봇 데이터
// ==================================
struct RobotInfo
{
	iSize size;
	iColor4f color;
	float delta; // 생산시간
};

extern RobotInfo ri[8];

extern int itemPath[6][4];
extern int slotInOut[4][4];