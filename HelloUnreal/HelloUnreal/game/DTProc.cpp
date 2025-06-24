#include "DTProc.h"

#include "DTLoading.h"
#include "DTLogin.h"

#include "DTObject.h"
#include "DTProcUI.h"
#include "DTProcPop.h"

void loadDTProc()
{
	printf("LoadDTProc()");
	loadDTField();
	loadDTObject();
	loadDTProcUI();
	loadDTProcPop();
}

void freeDTProc()
{
	printf("FreeDTProc()");
	freeDTField();
	freeDTObject();
	freeDTProcUI();
	freeDTProcPop();
}

void drawDTProc(float dt)
{
	drawDTField(dt);
	// offField -> 움직인만큼 반영
	drawDTObject(dt, offField);
	drawDTProcUI(dt);
	drawDTProcPop(dt);
}

void keyDTProc(iKeyStat stat, iPoint point)
{
	if (keyDTProcPop(stat, point) ||
		keyDTProcUI(stat, point) ||
		keyDTObject(stat, point))
		return;

	keyDTField(stat, point);
	// 키입력 순간 페이드아웃 됐다가 프로시저 화면으로 넘어갈것
	//if (stat == iKeyStatBegan)
	//	setLoading(DTStateLogin, freeDTProc, loadDTLogin);
}

// =======================================
// DTField
// =======================================
iPoint offField;
static iPoint offMin, offMax;

Texture* texField;

void loadDTField()
{
	offField = iPointZero;
	offMax = iPointZero;
	offMin = iPointMake(devSize.width - FieldWidth,
						devSize.height - FieldHeight);

	texField = createImage("assets/down2.jpg");
}

void freeDTField()
{
	freeImage(texField);
}

void drawDTField(float dt)
{
	//setRGBA(0, 0, 1, 1);
	//fillRect(0, 0, devSize.width, devSize.height);
	setRGBA(1, 1, 1, 1);
	Texture* t = texField;
	drawImage(t, offField.x, offField.y, 
		0, 0, t->width, t->height,
		(float)FieldWidth / t->width, (float)FieldHeight / t->height,
		2, 0, TOP | LEFT);
}

bool dragField = false;
iPoint prevField;

void keyDTField(iKeyStat stat, iPoint point)
{
	switch (stat)
	{
	case iKeyStatBegan:
		// 드래그 순간 현재위치가됨
		dragField = true;
		prevField = point;
		break;

	case iKeyStatMoved:
		if (dragField == false) break;
		offField += point - prevField;
		prevField = point; // 갱신
		// 최솟값 최댓값 제한
		if (offField.x < offMin.x)		offField.x = offMin.x;
		else if (offField.x > offMax.x) offField.x = offMax.x;
		if (offField.y < offMin.y)		offField.y = offMin.y;
		else if (offField.y > offMax.y) offField.y = offMax.y;
		break;
	case iKeyStatEnded:
		dragField = false;
		break;
	}
}

