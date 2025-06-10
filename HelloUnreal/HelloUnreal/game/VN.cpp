#include "VN.h"

ScriptMgt* sm;

void methodSM(char** line, int lineNum, int pageIndex, int pageNum);

// whosay s[whoIndex] -> pageIndex / pageNum
// index total

// click
// index < num -> index = total
// index == num -> (page++ pageNum -> whoIndex)


void loadVN()
{
	sm = new ScriptMgt(methodSM);
	sm->set(ws, 10, 150, 3);
}

void freeVN()
{
	delete sm;
}

iStrTex** stVN = NULL;

void methodSM(char** line, int lineNum, int pageIndex, int pageNum)
{
	setRGBA(1, 0, 0, 1);
	float x = 0, y = 0;
	fillRect(x, y, 150, 120); // 다이얼로그판
	setRGBA(1, 1, 1, 1);

	if (stVN == NULL)
	{
		stVN = new iStrTex * [4];
		for (int i = 0; i < 4; i++)
			stVN[i] = new iStrTex(NULL);
	}

	setStringSize(30);
	setStringRGBA(1, 1, 1, 1);

	// 글자 출력
	for (int i = 0; i < lineNum; i++)
		stVN[i]->paint(x, y + 40 * i, TOP | LEFT, line[i]);

	// 페이지 표현
	stVN[lineNum]->paint(x, y + 40 * lineNum, TOP | LEFT, "%d/%d",
		pageIndex + 1, pageNum);
}

void drawVN(float dt)
{
	setRGBA(0, 0, 0, 0);
	clear();

	sm->paint(dt);
}

void keyVN(iKeyStat stat, iPoint point)
{
	if (stat == iKeyStatBegan)
	{
		sm->nextPage();
		//pageIndex = (pageIndex + 1) % pageNum;
	}
}

WhoSay ws[10]
{
	{"NPC1", "0안녕 안녕 안녕 안녕\n 아무말아무말\n아무말아무말아무말"},
	{"NPC2", "1반가워 반가워 반가워 아무말아무말아무말 아무말아무말 아무말 아무말 아무말 아무말"},
	{"NPC1", "2너는 이름이 뭐니 아무말 아무말 아무말 아무말아무말"},
	{"NPC2", "3안가르쳐줘 아무말  아무말 아무말 아무말아무말"},
	{"NPC1", "4안가르쳐줘 아무말 아무말 아무말아무말 아무말"},
	{"NPC2", "5안가르쳐줘 아무말 아무말 아무말아무말 아무말"},
	{"NPC1", "6안가르쳐줘 아무말 아무말 아무말아무말 아무말"},
	{"NPC2", "7안가르쳐줘 아무말 아무말 아무말아무말 아무말"},
	{"NPC1", "8안가르쳐줘 아무말 아무말 아무말아무말 아무말"},
	{"NPC2", "9안가르쳐줘 아무말 아무말 아무말아무말 아무말"},
};

ScriptMgt::ScriptMgt(MethodSM method)
{
	this->method = method;
	pageStr = NULL;
	pageIndex = 0, pageNum = 0;
	linesOfPage = 0;
}

ScriptMgt::~ScriptMgt()
{
	clean();
}

void ScriptMgt::clean()
{
	if (pageStr == NULL)
		return;

	for (int i = 0; i < pageNum; i++)
	{
		for (int j = 0; j < linesOfPage; j++)
		{
			if (pageStr[i][j])
				delete pageStr[i][j];
		}
		delete pageStr[i];
	}
	delete pageStr;
	pageStr = NULL;
}

void ScriptMgt::set(WhoSay* ws, int wsNum, int lw, int lop)
{
	this->ws = ws;
	this->wsNum = wsNum;
	wsIndex = 0;
	lineWidth = lw;
	linesOfPage = lop;
	load(ws[0].say);
}

void ScriptMgt::load(const char* say)
{
	char t[100];
	iArray* array = new iArray();

	int lineNum;
	char** line = iString::split(lineNum, say); // \n에 따라 문자 나누고
	for (int i = 0; i < lineNum; i++)
	{
		//printf("================\nline[%d] %s\n", i, line[i]);
		int lNum;
		char** l = iString::splitWidth(lNum, line[i], lineWidth); // 가로길이에 따라 나눈다
		for (int j = 0; j < lNum; j++)
		{
			//printf("l[%d] %s\n", j, l[j]);
			array->add(l[j]);
		}
		delete l;
		delete line[i];
	}
	delete line;

	for (int i = 0; i < array->count; i++)
	{
		const char* s = (char*)array->at(i);
		printf("line[%d] = %s\n", i, s);
	}

	// arrayLine->count
	pageNum = 1 + (array->count - 1) / linesOfPage;
	pageIndex = 0;

	pageStr = new char** [pageNum];
	for (int i = 0; i < pageNum; i++)
	{
		char** t = new char* [linesOfPage];
		memset(t, 0x00, sizeof(char*) * linesOfPage);

		pageStr[i] = t;
	}
	// i / 3 = 몇번째 페이지, % 3은 그 페이지의 몇번째!
	for (int i = 0; i < array->count; i++)
		pageStr[i / linesOfPage][i % linesOfPage] = (char*)array->at(i);
	delete array;
}

bool ScriptMgt::nextWs()
{
	wsIndex++;
	if (wsIndex == wsNum)
		return false;

	clean();
	load(ws[wsIndex].say);
	return true;
}

bool ScriptMgt::nextPage()
{
	pageIndex++;
	if (pageIndex < pageNum)
	{

	}
	else // if(pageIndex == pageNum)
	{
		return nextWs();
	}
	return true;
}

void ScriptMgt::paint(float dt)
{
	if (pageIndex == pageNum)
		return;

	char** s = pageStr[pageIndex];
	int lop = linesOfPage;
	for (int i = 0; i < linesOfPage; i++)
	{
		if (s[i] == NULL)
		{
			lop = i;
			break;
		}
	}

	method(s, lop, pageIndex, pageNum);
}

