#include "iImage.h"
#include "iStd.h"


iImage::iImage()
{
	array = new iArray(cbArray);
	tex = NULL;
	index = 0;
	animation = false;
	_aniDt = aniDtDefault;
	aniDt = 0.0f;
	position = iPointZero;
	rate = 1.0f;
	anc = TOP | LEFT;
	reverse = REVERSE_NONE;
}

iImage::~iImage()
{
	delete array;
}

void iImage::cbArray(void* data)
{
	Texture* tex = (Texture*)data;
	freeImage(tex);
}

void iImage::add(Texture* tex)
{
	array->add(tex);
	tex->retainCount++; // 참조 수
}

void iImage::paint(float dt, iPoint position)
{
	if (animation)
	{
		// 시간이 흐르면서 지정한 시간이 초과하면 인덱스 증가
		aniDt += dt;
		if (aniDt >= _aniDt)
		{
			aniDt -= _aniDt;
			index++;
			if (index > array->count - 1)
			{
				// 애니메이션 종료됐을때 콜백 호출
				index = 0;
				if (method)
					method(parm);
			}
		}
	}

	tex = (Texture*)array->at(index);
	iPoint p = this->position * rate + position;
	drawImage(tex, p.x, p.y,
		0, 0, tex->width, tex->height,
		rate, rate, 2, 0, anc, reverse);
}

void iImage::startAnimation(iImageAnimation m, void* p)
{
	animation = true;
	index = 0;
	aniDt = 0.0f;

	method = m;
	parm = p;
}

iRect iImage::touchRect(iPoint position)
{
	if (tex == NULL)
		return iRectMake(0, 0, 0, 0);

	iPoint p = this->position + position;
	return iRectMake(p.x, p.y, tex->width, tex->height);	
}

iImage* iImage::copy()
{
	iImage* img = new iImage();
	iArray* a = img->array; // 백업
	memcpy(img, this, sizeof(iImage));
	img->array = a; // 복구

	for (int i = 0; i < array->count; i++)
	{
		img->add((Texture*)array->at(i));
	}

	return img;
}
