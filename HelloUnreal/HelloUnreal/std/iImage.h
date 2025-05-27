#pragma once

#include "iDefine.h"
#include "iArray.h"
#include "iPoint.h"

class iImage;
typedef void (*MethodImage)(iImage* img);

class iImage
{
public:
	iImage();
	virtual ~iImage();

	static void cb(void* data);
	void add(Texture* tex);
	void paint(float dt, iPoint position);

	void startAnimation(MethodImage cb);

	iRect touchRect();

public:
	iArray* array; // 텍스처가 몇개가 될지 모르니 리스트로
	MethodImage method;
	bool animation;
	float aniDt, _aniDt; // 애니메이션 재생시간
	int index; // 애니메이션 인덱스
	iPoint position;
	Texture* tex;
};