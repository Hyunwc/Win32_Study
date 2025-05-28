#pragma once

#include "iDefine.h"
#include "iPoint.h"
#include "iRect.h"
#include "iArray.h"

#define aniDtDefault 0.017f

typedef void (*iImageAnimation)(void* data);

class iImage
{
public:
	iImage();
	virtual ~iImage();
	static void cbArray(void* data);
	void add(Texture* tex);

	void paint(float dt, iPoint position);

	// p : parm
	void startAnimation(iImageAnimation m = NULL, void* p = NULL);

	iRect touchRect(iPoint position = iPointZero);
	
public:
	// 텍스처 0 1 2 있다면 012순서대로 재생
	iArray* array; // 텍스처들 리스트에 보관
	Texture* tex;
	int index; 
	bool animation;
	float aniDt, _aniDt;
	iPoint position;
	float rate; 
	int anc;
	int reverse;

	iImageAnimation method;
	void* parm;
};