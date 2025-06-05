#include "Game.h"

#include "Lotto.h"
#include "Tripple.h"
#include "AirShooting.h"
#include "Memory.h"
#include "PS.h"
#include "Animating.h"
#include "CtrlImage.h"
#include "Oops.h"
#include "Comp.h"

ParticleSystem** _ps;
ParticleSystem* ps;
iPoint psPos;
int psIndex;

void testGame();

//Texture** texBtn;
iImage** imgBtn;
int selectedBtn;

Texture* texBg; // 배경
Texture* texMirror;

void printY(const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szFormat, szText);

	printf("%s\n", szText);
}

void printX(const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szFormat, szText);

	printY(szText);
}

//void testStr(char** t)
//{
//	printf("testStr = %s\n", *t);
//
//	char* s = new char[12];
//	strcpy(s, "Hi World");
//	delete* t;
//	*t = s;
//}

void loadGame()
{
	/*char* t = new char[5];
	strcpy(t, "hi");
	testStr(&t);

	printf("t = %s\n", t);*/
	texBg = createImageFilter("assets/down1.png");
	setImageFilter(imageFilterMirror);
	texMirror = createImageFilter("assets/down1.png");

#if 0
	loadComp();
	return;
#elif 0
	loadOops();
	return;
#elif 0
	loadAnimating();
	return;
#elif 0
	loadLotto();
#elif 0
	loadTripple();
#elif 0
	loadAirShootng();
#elif 0
	loadMemory();
#elif 0
	/*_ps = new ParticleSystem * [7];
	for (int i = 0; i < 7; i++)
	{
		char path[32];
		sprintf(path, "test%d.ptc", i);
		_ps[i] = new ParticleSystem(path);
	}
	ps = _ps[psIndex = 0];*/
	ps = new ParticleSystem();
	//ps = new ParticleSystem();
	//ps->save("test6.ptc");
	//
	//// 파일 이름으로 Load
	//ps = new ParticleSystem("test.ptc");
	
#endif
	struct BtnInfo
	{
		iSize size;
		char str[32];
		iColor4f cB, cS;
	};

	BtnInfo btnInfo[3] = {
		{{100, 30}, "Hello", {0, 1, 0, 1}, {1, 0, 0, 1}},
		{{150, 35}, "World", {0, 0, 1, 1}, {0, 1, 0, 1}},
		{{120, 32}, "Hi", {1, 0, 0 ,1}, {1, 1, 1, 1}}
	};

	iGraphics* g = iGraphics::getInstance();

	imgBtn = new iImage * [3];
	for (int i = 0; i < 3; i++)
	{
		iImage* img = new iImage();
		img->position = iPointMake(10, 10 + 40 * i);

		BtnInfo* bi = &btnInfo[i];

		for (int j = 0; j < 2; j++)
		{
			g->init(bi->size.width, bi->size.height);

			setRGBA(bi->cB.r * (1.0f - 0.5f * j), // 1 ~ 0.5
					bi->cB.g * (1.0f - 0.5f * j),
					bi->cB.b * (1.0f - 0.5f * j), 
					bi->cB.a);
			g->fillRect(0, 0, bi->size.width, bi->size.height);

			// 글자
			setRGBA(bi->cS.r, bi->cS.g, bi->cS.b, bi->cS.a);
			g->drawString(3, 3, TOP|LEFT, bi->str);
			
			// ref카운팅 관리를 위해 free를
			// ref카운트를 1로 유지시키기 위함
			Texture* tex = g->getTexture();//+1
			img->add(tex);//+2
			freeImage(tex);//+1
		}

		imgBtn[i] = img;
	}

	selectedBtn = -1;
}

void freeGame()
{
	freeImage(texBg);
	freeImage(texMirror);
#if 0
	freeComp();
	return;
#elif 0
	freeOops();
	return;
#elif 0
	freeAnimating();
	return;
#elif 0
	freeLotto();
#elif 0
	freeTripple();
#elif 0
	freeAirShootng();
#elif 0
	freeMemory();
#elif 0

	//delete ps;
	//delete _ps;
#endif
	for (int i = 0; i < 3; i++)
		delete imgBtn[i];
	delete imgBtn;
}

void drawGame(float dt)
{
	setRGBA(0, 0, 0, 1);
	clear();

	setRGBA(1, 1, 1, 1);
	drawImage(texBg, 0, 200, BOTTOM | LEFT);
	drawImage(texMirror, 0, 200, TOP | LEFT);
	
	/*static float delta = 0.0f;
	delta += dt;
	iPoint off = iPointMake(50 * sin(delta), 0);

	for (int i = 0; i < 3; i++)
	{
		imgBtn[i]->index = (selectedBtn == i);
		imgBtn[i]->paint(dt, off);
	}*/

#if 0
	drawComp(dt);
	return;
#elif 0
	drawOops(dt);
	return;
#elif 0
	drawAnimating(dt);
	return;
#elif 0
	drawLotto(dt);
	return;
#elif 0
	drawTripple(dt);
	return;
#elif 0
	drawAirShootng(dt);
	return;
#elif 0
	drawMemory(dt);
	return;
#elif 0
	//ps->paint(dt, iPointMake(DEV_WIDTH/2, DEV_HEIGHT /2));
	//ps->paint(dt, iPointMake(psPos.x, psPos.y));

	/*if (keydown & keydown_space)
	{
		psIndex = (psIndex + 1) % 7;
		ps = _ps[psIndex];
	}*/
	//return;
#endif

}

void keyGame(iKeyStat stat, iPoint point)
{
	

#if 0
	keyComp(stat, point);
	return;
#elif 0
	keyOops(stat, point);
	return;
#elif 0
	keyAnimating(stat, point);
	return;
#elif 0
	keyLotto(stat, point);
	return;
#elif 0
	keyTripple(stat, point);
	return;
#elif 0
	keyAirShootng(stat, point);
	return;
#elif 0
	keyMemory(stat, point);
	return;
#elif 0
	//ps->move(stat, point);
#endif
	
	if (stat == iKeyStatBegan)
	{
		//psPos = point;
		//tp = point;
	}
	else if (stat == iKeyStatMoved)
	{
		//psPos = point;
		int j = -1;
		for (int i = 0; i < 3; i++)
		{
			if (containPoint(point, imgBtn[i]->touchRect()))
			{
				j = i;
				break;
			}
		}
		selectedBtn = j;
	}
	else if (stat == iKeyStatEnded)
	{

	}
}


void testGame()
{
	// utf16 == 멀티바이트
/*wchar_t* wStr = utf8_to_utf16("안녕%d", 1004);
char* str = utf16_to_utf8(L"Hi");*/
// L"안녕1004"

//myPrintf("myPrintf %d, %s\n", 1004, "입니다");
#if 1
	/*struct Score
	{
		char name[32];
		int kor, eng, math;
	};*/

	/*Score score[3] = {
		{"김태현", 100, 100, 100},
		{"김재학", 50, 50, 50},
		{"고민철", 50, 50, 50},
	};*/
	//saveFile((char*)score, sizeof(score), "test.sav");

	// 외부에서 파일을 해석하지 못하게 하는 방법1
	/*int len = sizeof(score);
	char* t = new char[len];
	for (int i = 0; i < len; i++)
		t[i] = ((char*)score)[i] + 128;
	saveFile(t, len, "test.sav");*/

	//int len;
	//char* t = loadFile(len, "test.sav");
	///*for (int i = 0; i < len; i++)
	//	t[i] -= 128;*/
	//Score* s = (Score*)t;
	////Score* s = (Score*)loadFile("test.sav");
	//for (int i = 0; i < 3; i++)
	//{
	//	printf("[%s] %d %d %d\n",
	//		s[i].name, s[i].kor, s[i].eng, s[i].math);
	//}

	/*char* str = loadFile("HelloUnreal.h");
	printf("HelloUnreal.h\n%s\n", str);
	delete str;*/
#else

#pragma pack(push, 1)
	struct Score
	{
		char kor;
		short math;
		int eng;
	};
#pragma pack(pop)

	printf("%d\n", sizeof(Score));
#endif

}


