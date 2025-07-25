#include "DTLogin.h"

#include "DTCommon.h"
#include "DTLoading.h"
#include "DTProc.h"

// 버튼(접속하기/문의하기)
iImage** imgLoginBtn;
int selectedLoginBtn;

LoginBar* lb;

int* progress;
#define maxProgress 50000

void loadDTLogin()
{
	printf("LoadDTLogin()");
#ifdef DEBUG_TEXTURE
	printf("loadDTLogin::textureNum = %d\n", textureNum);
#endif
	iGraphics* g = iGraphics::getInstance();

	const char* strBtn[] = { "접속하기", "문의하기" };
	imgLoginBtn = new iImage * [2];
	iSize size = iSizeMake(300, 40);
	setStringSize(30);
	setStringRGBA(1, 1, 1, 1);
	for (int i = 0; i < 2; i++)
	{
		iImage* img = new iImage();
		for (int j = 0; j < 2; j++)
		{
			g->init(size.width, size.height);
			if (j == 0)
			{
				setRGBA(0.7f, 0.7f, 0.7f, 1.0f);
				g->fillRect(0, 0, size.width, size.height);
				setRGBA(1, 1, 1, 1);
				g->drawRect(10, 10, size.width - 20, size.height - 20);
			}
			else// if (j == 1)
			{
				setRGBA(0.25f, 0.25f, 0.25f, 1.0f);
				g->fillRect(15, 15, size.width - 30, size.height - 30);
				setRGBA(0.7f, 0.7f, 0.7f, 1.0f);
				g->drawRect(25, 25, size.width - 50, size.height - 50);
			}
			g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strBtn[i]);

			Texture* tex = g->getTexture();
			img->add(tex);
			g->clean();
			freeImage(tex);
		}

		img->position = iPointMake((devSize.width - size.width) / 2,
									devSize.height * 0.3 + (size.height + 20) * i);
		imgLoginBtn[i] = img;
	}
	selectedLoginBtn = -1;

	lb = new LoginBar();
	progress = new int[10];
#ifdef DEBUG_TEXTURE
	printf("loadDTLogin::textureNum = %d\n", textureNum);
#endif
}

void freeDTLogin()
{
	printf("FreeDTLogin()");
	for (int i = 0; i < 2; i++)
		delete imgLoginBtn[i];
	delete imgLoginBtn;
	delete lb;
	delete progress;
}

bool loginSuccess = false;

void drawDTLogin(float dt)
{
	//setRGBA(1, 0, 0, 1);
	//fillRect(0, 0, devSize.width, devSize.height);
	setRGBA(1, 1, 1, 1);
	static Texture* tex = createImage("assets/def.png");
	drawImage(tex, 0, 0, 0, 0, tex->width, tex->height,
		devSize.width / tex->width, devSize.height / tex->height,
		2, 0, TOP | LEFT);

	for (int i = 0; i < 2; i++)
	{
		imgLoginBtn[i]->index = (i == selectedLoginBtn);
		imgLoginBtn[i]->paint(dt, iPointZero);
	}

	lb->paint(dt, iPointMake(devSize.width / 2, devSize.height / 2));

	if (loginSuccess)
	{
		loginSuccess = false;
		setLoading(DTStateProc, freeDTLogin, loadDTProc);
	}

#if DISPLAY_THREAD
	setRGBA(1, 0, 0, 1);
	for (int i = 0; i < 10; i++)
	{
		float r = (float)progress[i] / maxProgress;
		fillRect(10, 10 + 20 * i, (devSize.width - 20) * r, 10);
	}
	setRGBA(1, 1, 1, 1);
#endif
}

void resultDtLogin(int result)
{
	if (result == 0)
	{
		// 성공
		loginSuccess = true;
		lb->show(false); // 성공이든 실패든 닫아야함
	}
	else
	{
		// 실패
		printf("로그인 실패원인 : %d\n", result);
	}
}

void keyDTLogin(iKeyStat stat, iPoint point)
{
	// 키입력 순간 페이드아웃 됐다가 프로시저 화면으로 넘어갈것
	//if (stat == iKeyStatBegan)
	//{
	//	setLoading(DTStateProc, freeDTLogin, loadDTProc);
	//}

	int i, j = -1;
	switch (stat)
	{
	case iKeyStatBegan:
		if (selectedLoginBtn == -1) break; // 선택된 버튼이없음

		playAudio(AudioBtnOk);
		if (selectedLoginBtn == 0)
		{
			printf("접속하기\n");
#if DISPLAY_THREAD
			const char* str[10][2] =
			{
				{"김태현", "천재"},
				{"언리얼", "바보"},
				{"유니티", "바보"},
				{"홍유성", "바보"},
				{"가나디", "바보"},
				{"김동우", "바보"},
				{"장지훈", "바보"},
				{"고민철", "바보"},
				{"김재학", "바보"},

				{"윤근용", "바보"},
			};
			for(int i = 0; i < 10; i++)
				dtLogin(resultDtLogin, str[i][0], str[i][1], i);

			lb->show(true);
#else
			loginSuccess = true;
#endif
		}
		else if (selectedLoginBtn == 1)
		{
			printf("문의하기\n");
		}
		break;

	case iKeyStatMoved:
		for (i = 0; i < 2; i++)
		{
			if (containPoint(point, imgLoginBtn[i]->touchRect()))
			{
				j = i;
				break;
			}
		}
		if (selectedLoginBtn != j)
		{
			if(j != -1)
				playAudio(AudioBtnClr);
			printf("sound : 버튼음\n");
		}
		selectedLoginBtn = j;
		break;
	case iKeyStatEnded:
		break;
	}

}

LoginBar::LoginBar()
{
	_delta = 1.0f;
	delta = 0.0f;
	bShow = false;
}

LoginBar::~LoginBar()
{
}

void LoginBar::show(bool s)
{
	bShow = s;
	delta = 0.0f;
}

void LoginBar::paint(float dt, iPoint position)
{
	if (bShow == false)
		return;

	float x = position.x - 50;
	float y = position.y - 25;
	setRGBA(1, 1, 1, 1);
	fillRect(x, y, 100, 50, 25);

	delta += dt;
	x = position.x - 25 - 25 * sin(delta/_delta * 10);
	y = position.y - 25;
	setRGBA(1, 0, 0, 1);
	fillRect(x, y, 50, 50, 25);
	setRGBA(1, 1, 1, 1);
}

// 서버 개발자가 구현한 부분
#include <process.h>

struct IDPW
{
	char* id;
	char* pw;
	cbDtLogin m;
	int n;
};

// 메인스레드가 아닌 서브스레드
unsigned __stdcall run(void* parm)
{
	IDPW* idpw = (IDPW*)parm;
	//for (int i = 0; i < maxProgress; i++)
	int& i = progress[idpw->n];
	for (i = 0; i < maxProgress; i++)
		printf("%s,%s,%d\n", idpw->id, idpw->pw, i);

	printf("%s,%s the end\n", idpw->id, idpw->pw);
	bool success = strcmp(idpw->id, "언리얼") == 0;
	idpw->m(success);
	delete idpw->id;
	delete idpw->pw;
	delete idpw;

	return 0;
}

void dtLogin(cbDtLogin func, const char* name, const char* pw, int n)
{
	IDPW* idpw = new IDPW;
	idpw->id = iString::copy(name);
	idpw->pw = iString::copy(pw);
	idpw->m = func;
	idpw->n = n;

	uint32 id;
	_beginthreadex(NULL, 0, run, (void*)idpw, 0, &id);
}
