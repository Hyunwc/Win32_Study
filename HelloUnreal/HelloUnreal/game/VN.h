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

#if 0
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

void testGLM()
{
	glm::vec4 v(1, 0, 0, 0); // x, y, z, w

	glm::mat4 m(1.0);
	float a[4][4];
	memcpy(&a[0][0], &m, sizeof(float) * 16);

	glm::rotate(m, glm::radians(45.0f), glm::vec3(0, 0, 1));
	memcpy(&a[0][0], &m, sizeof(float) * 16);

	glm::vec4 result = m * v;
	float b[4];
	memcpy(b, &result, sizeof(float) * 4);

	printf("");
}
#endif