#pragma once

#include "iStd.h"

void loadVN();
void freeVN();
void drawVN(float dt);
void keyVN(iKeyStat stat, iPoint point);

struct WhoSay
{
	const char* who; // 누가 말하는가?
	const char* say; // 내용
};

typedef void (*MethodSM)(char** line, int lineNum,
	int pageIndex, int pageNum,
	const char* stringName, float stringSize);

struct ScriptMgt
{
	ScriptMgt(MethodSM method);
	virtual ~ScriptMgt();
	
	static void save(const char* path, WhoSay* ws, int wsNum);
	static WhoSay* call(const char* path, int& wsNum);

	void clean();
	void set(WhoSay* ws, int wsNum,
		const char* stringName, float stringSize,
		int lineWidth, int linesOfPage, float aniDt = 0.2f);
	void load(const char* say);
	void paint(float dt);
	bool nextPage(); // pageIndex++;
private:
	bool nextWs(); // wdIndex++ (말하는 사람바꿈)
public:
	MethodSM method;
	// set
	WhoSay* ws;
	const char* stringName;
	float stringSize;
	int wsNum, wsIndex;
	int lineWidth, linesOfPage;

	// load
	char*** pageStr;
	// 페이지 인덱스(현재 페이지), 총 페이지
	int pageIndex, pageNum;
	// animation
	char** aniStr;
	float aniDt, _aniDt;
	int aniIndex, aniTotal;
};

extern WhoSay ws[10];

struct DicWho
{
	Texture* tex;
	char* key;
};

void loadDicWho();
void freeDicWho();

Texture* getDicWho(const char* key);

// ==================
// PopTest
// ==================
extern iPopup* popTest;

void createPopTest();
void freePopTest();
void showPopTest(bool show);
void drawPopTest(float dt);
bool keyPopTest(iKeyStat stat, iPoint point);