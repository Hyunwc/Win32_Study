#include "VN.h"

ScriptMgt* sm;

void methodSM(char** line, int lineNum,
	int pageIndex, int pageNum,
	const char* stringName, float stringSize);

// whosay s[whoIndex] -> pageIndex / pageNum
// index total

// click
// index < num -> index = total
// index == num -> (page++ pageNum -> whoIndex)


void loadVN()
{
	sm = new ScriptMgt(methodSM);
	int lineWidth = 150;
	int linesOfPage = 3;
	sm->set(ws, 10, 
		"assets/CRRegular.ttf", 25,
		lineWidth, linesOfPage);
}

void freeVN()
{
	delete sm;
}

iStrTex** stVN = NULL;

void methodSM(char** line, int lineNum,
	int pageIndex, int pageNum,
	const char* stringName, float stringSize)
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
	setStringName(stringName);
	setStringSize(stringSize);
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

void ScriptMgt::set(WhoSay* ws, int wsNum, 
	const char* sn, float ss, 
	int lw, int lop, float aDt)
{
	this->ws = ws;
	this->wsNum = wsNum;
	wsIndex = 0;
	stringName = iString::copy(sn);
	stringSize = ss;
	lineWidth = lw;
	linesOfPage = lop;
	aniStr = new char* [lop];
	for (int i = 0; i < lop; i++)
		aniStr[i] = new char[512];
	_aniDt = aDt;
	aniDt = 0.0f;
	aniIndex = 0;
	load(ws[0].say);
}

void ScriptMgt::load(const char* say)
{
	char* bkSN = iString::copy(getStringName());
	float bkSS = getStringSize();
	setStringName(stringName);
	setStringSize(stringSize);

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

	setStringName(bkSN);
	setStringSize(bkSS);
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
	if (aniIndex < aniTotal)
	{
		// skip
		aniIndex = aniTotal;
	}
	else
	{
		// next page
		aniIndex = 0;
		aniDt = 0.0f;

		if (pageIndex < pageNum)
		{
		}
		else // if(pageIndex == pageNum)
		{
			return nextWs();
		}
	}
	return true;
}

void copyLine(char** src, int srcNum, char** dst, int& dstNum, int len);

void ScriptMgt::paint(float dt)
{
	if (pageIndex == pageNum)
		return;

	char** s = pageStr[pageIndex];
	int sNum = linesOfPage;
	for (int i = 0; i < linesOfPage; i++)
	{
		if (s[i] == NULL)
		{
			sNum = i;
			break;
		}
	}

	int total = 0;
	for (int i = 0; i < sNum; i++)
	{
		char* t = s[i];
		for (int j = 0; t[j];)
		{
			// 총 글자수 -> 한글은 한자에 3바이트기 때문
			total++;

			j += (iString::isUTF8(&t[j]) ? 3 : 1);
		}
	}

	aniTotal = total;

	aniDt += dt;
	while (aniDt >= _aniDt)
	{
		aniDt -= _aniDt;
		aniIndex++;
		if (aniIndex > aniTotal)
			aniIndex = aniTotal;
	}

	// aniTotal, aniIndex 고려해서 aniStr입력, aniStrNum
	int aniNum;
	for (int i = 0; i < linesOfPage; i++)
		memset(aniStr[i], 0x00, sizeof(char) * 512);
	copyLine(s, sNum, aniStr, aniNum, aniIndex);
	method(aniStr, aniNum, pageIndex, pageNum, stringName, stringSize);
}

void copyLine(char** src, int srcNum, char** dst, int& dstNum, int len)
{
	int m = 0, n = 0;
	for (int i = 0; i < len; i++) // 글자 수
	{
		int cpy = iString::isUTF8(&src[m][n]) ? 3 : 1;
		memcpy(&dst[m][n], &src[m][n], cpy);
		//dst[m][n + cpy] = 0; // 문자열의 끝
		n += cpy;

		// 줄바꿈
		if (src[m][n] == 0)
		{
			m++;
			n = 0;
		}
	}
	// 라인수
	dstNum = m + 1;
	if (dstNum > srcNum)
		dstNum = srcNum;
}