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

typedef void (*MethodSM)(char** line, int lineNum, int pageIndex, int pageNum);

struct ScriptMgt
{
	ScriptMgt(MethodSM method);
	virtual ~ScriptMgt();
	
	void set(WhoSay* ws, int wsNum, int lw, int lop);
	void clean();
	void load(const char* say);
	void paint(float dt);
	bool nextPage(); // pageIndex++;
private:
	bool nextWs(); // wdIndex++ (말하는 사람바꿈)
public:
	MethodSM method;
	// set
	WhoSay* ws;
	int wsNum, wsIndex;
	int lineWidth, linesOfPage;

	// load
	char*** pageStr;
	// 페이지 인덱스(현재 페이지), 총 페이지
	int pageIndex, pageNum;
};

extern WhoSay ws[10];