#include "iImage.h"
#include "iStd.h"

iImage::iImage()
{
	array = new iArray(cb);
}

iImage::~iImage()
{
	delete array;
}

void iImage::cb(void* data)
{
	Texture* tex = (Texture*)data;
	freeImage(tex);
	delete tex;
}

void iImage::add(Texture* tex)
{
	array->add(tex);
	// 이미지 추가됐으니 count + 1
	tex->retainCount++;
}

void iImage::paint(float dt)
{
	if (animation)
	{
		aniDt += dt;
		if (aniDt >= _aniDt)
		{
			aniDt -= _aniDt;
			index++;
			if (index == array->count)
			{
				animation = false;
				index = 0;
				// 애니메이션 끝
				if (method)
					method(this);
			}
		}
	}

	Texture* tex = (Texture*)array->at(index);
	drawImage(tex, 0, 0);
}

void iImage::startAnimation(MethodImage cb)
{
	method = cb;
	animation = true;
	aniDt = 0.0f;
	index = 0;
}
