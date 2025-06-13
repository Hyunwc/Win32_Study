#pragma once

#include "iDefine.h"
#include "iArray.h"
#include "iPoint.h"
#include "iImage.h"

enum iPopupStat
{
	iPopupStatOpen = 0,
	iPopupStatProc,
	iPopupStatClose
};

enum iPopupStyle
{
	iPopupStyleAlpha = 0,
	iPopupStyleMove,
	iPopupStyleZoom,
};

class iPopup;
typedef void (*MethodPopupOpen)(iPopup* pop);
typedef void (*MethodPopupDraw)(iPopup* pop, float rate);

typedef void (*MethodStyle)(float dt, iPopup* pop, iPoint& position,
	float& alpha, float& scale, float& degree);

class iPopup
{
public:
	iPopup();
	virtual ~iPopup();

	static void cbArray(void* data);
	void add(iImage* img);

	void paint(float dt);

	void show(bool show);

public:
	iArray* array;  // iImage

	bool bShow;
	iPopupStyle style;
	iPopupStat stat;
	iPoint sp, ep;
	float aniDt, _aniDt;

	int selected;

	MethodPopupOpen methodOpen, methodClose;
	MethodPopupDraw methodDrawBefore, methodDrawAfter;
};


void drawPopStyleAlpha(float delta, iPopup* pop, iPoint& position,
	float& alpha, float& scale, float& degree);

void drawPopStyleMove(float delta, iPopup* pop, iPoint& position,
	float& alpha, float& scale, float& degree);

void drawPopStyleZoom(float delta, iPopup* pop, iPoint& position,
	float& alpha, float& scale, float& degree);