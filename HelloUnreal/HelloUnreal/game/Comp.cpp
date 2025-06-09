#include "Comp.h"

// 배경이미지
iPopup* popComp;

iStrTex* stCompBg;
Texture* methodStCompBtn(const char* s);

// 버튼 예/아니오
iImage** imgCompBtn;
iStrTex*** stCompBtn;
Texture* methodStCompBg(const char* s);

void loadComp()
{
	popComp = new iPopup();

	iStrTex* st = new iStrTex(methodStCompBg);
	st->set("타이틀\n내용물?");
	iImage* img = new iImage();
	img->add(st->tex);
	popComp->add(img);

	StInfo si[3] = {
		{"태현", {0, 0, 1, 1}},
		{"부산", {1, 0, 0, 1}},
		{"언리얼", {1, 0, 1, 1}}
	};

	Texture* tex = iStrTex::createImage(si, 3, 
		"강사 조성목은\n센텀 벤처타운에서\n언리얼 수업중");
	img = new iImage();
	img->add(tex);
	popComp->add(img);

	// imgCompBtn
	const char* strBtn[] = {"예", "아니오"};

	imgCompBtn = new iImage*[2];
	stCompBtn = new iStrTex**[2];

	for (int i = 0; i < 2; i++)
	{
		img = new iImage();
		stCompBtn[i] = new iStrTex * [2];
		for (int j = 0; j < 2; j++)
		{
			iStrTex* st = new iStrTex(methodStCompBtn);
			st->set("%d\n%s", j, strBtn[i]);
			stCompBtn[i][j] = st;
			img->add(st->tex);
		}

		img->position = iPointMake(40 + 120 * i, 190);
		popComp->add(img);
		imgCompBtn[i] = img;
	}

	// 왼쪽에서 오른쪽으로
	/*popComp->sp = iPointMake(-400, 200);
	popComp->ep	= iPointMake(200, 200);*/

	popComp->style = iPopupStyleMove;
	// 위에서 아래로
	popComp->sp = iPointMake(100, -400);
	popComp->ep = iPointMake(100, 200);
	popComp->_aniDt = 0.5f;
}

void freeComp()
{
	delete popComp;
	delete imgCompBtn;
}

Texture* methodStCompBg(const char* s)
{
	int lineNum;
	char** line = iString::split(lineNum, s);

	iGraphics* g = iGraphics::getInstance();
	iSize size = iSizeMake(320, 240);
	g->init(size.width, size.height);

	setRGBA(0.5f, 0.5f, 0.5f, 0.6f);
	g->fillRect(0, 0, size.width, size.height);
	setStringSize(30);
	setStringRGBA(1, 1, 1, 1);
	g->drawString(size.width / 2, size.height / 2,
		VCENTER | HCENTER, line[1]);

	setRGBA(0.3f, 0.f, 0.f, 0.6f);
	g->fillRect(0, 0, size.width, 40);
	g->drawString(10, 20, VCENTER | LEFT, line[0]);

	Texture* tex = g->getTexture();
	g->clean();

	iString::free(line, lineNum);

	return tex;
}

Texture* methodStCompBtn(const char* s)
{
	int lineNum;
	char** line = iString::split(lineNum, s);
	
	int index = atoi(line[0]);
	const char* str = line[1];

	iGraphics* g = iGraphics::getInstance();
	iSize size = iSizeMake(100, 40);

	g->init(size.width, size.height);

	setStringSize(20);
	setStringRGBA(1, 1, 1, 1);
	if (index == 0)
	{
		setRGBA(1, 0, 0, 0.5f);
		g->fillRect(0, 0, size.width, size.height);
		g->drawString(size.width / 2, size.height / 2,
			VCENTER | HCENTER, str);
	}
	else //if (index == 1)
	{
		setRGBA(0, 0, 1, 0.5f);
		g->fillRect(15, 15, size.width - 30, size.height - 30);
		g->drawString(size.width / 2, size.height / 2,
			VCENTER | HCENTER, str);
	}

	Texture* tex = g->getTexture();
	g->clean();

	iString::free(line, lineNum);

	return tex;
}

void drawComp(float dt)
{
	for (int i = 0; i < 2; i++)
		imgCompBtn[i]->index = (i == popComp->selected);

	popComp->paint(dt);

	if (popComp->bShow == false ||
		popComp->stat != iPopupStatProc)
		return;
	static float delta = 0.0f;
	delta += dt;
	while (delta >= 2.0f)
		delta -= 2.0f;

	setStringSize(20);
	setStringRGBA(1, 1, 1, 1);

	const char* strBtn[2][2] = {
		{"예", "아니오"},
		{"Yes", "No"}
	};

	const char** s = NULL;
	if (delta < 1.0f) s = strBtn[0];
	else			  s = strBtn[1];
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			stCompBtn[i][j]->set("%d\ns", j, s[i]);
		}
	}
}

bool keyPopComp(iKeyStat stat, iPoint point)
{
	// 화면에 안보이니 false
	if (popComp->bShow == false)
		return false;
	if (popComp->stat != iPopupStatProc)
		return true;

	int i, j = -1;
	if (stat == iKeyStatBegan)
	{
		printf("imgCompBtn[%d] 눌렀네?\n", popComp->selected);
		popComp->show(false);
	}
	else if (stat == iKeyStatMoved)
	{
		for (int i = 0; i < 2; i++)
		{
			if (containPoint(point,
				imgCompBtn[i]->touchRect(popComp->ep)))
			{
				j = i;
				break;
			}
		}
		popComp->selected = j;
	}

	return true;
}

void keyComp(iKeyStat stat, iPoint point)
{
	if (keyPopComp(stat, point))
		return;

	if (stat == iKeyStatBegan)
	{
		popComp->show(true);
	}
	else if (stat == iKeyStatMoved)
	{
		
	}
}
