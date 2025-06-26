#include "iPopup.h"

#include "iStd.h"

static Texture* texPop = NULL;

iPopup::iPopup()
{
	// 중복되지 않게 
	if (texPop == NULL)
	{
		texPop = iFBO::createImage(devSize.width, devSize.height);
		/*texPop = iFBO::createImage(1920, 1080);
		texPop->width = devSize.width;
		texPop->height = devSize.height;*/
	}

	array = new iArray(cbArray);

	bShow = false;
	style = iPopupStyleAlpha;
	stat = iPopupStatClose;
	sp = iPointZero;
	ep = iPointZero;
	_aniDt = 0.2f;
	aniDt = 0.0f;
	selected = -1;

	methodOpen = NULL;
	methodClose = NULL;
	methodDrawBefore = NULL;
	methodDrawAfter = NULL;
}

iPopup::~iPopup()
{
	if (texPop)
	{
		freeImage(texPop);
		texPop = NULL;
	}

	delete array;
}

void iPopup::cbArray(void* data)
{
	iImage* img = (iImage*)data;
	delete img;
}

void iPopup::add(iImage* img)
{
	array->add(img);
}

void iPopup::paint(float dt)
{
	if (bShow == false)
		return;

	float alpha = 1.0f;
	iPoint p = iPointZero;
	float scale = 1.0f;
	float degree = 0.0f;

	MethodStyle ms[3] = {
		drawPopStyleAlpha, drawPopStyleMove, drawPopStyleZoom
	};

	ms[style](dt, this, p, alpha, scale, degree);

	// back buffer
	fbo->bind(texPop);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	iSize size = iSizeZero;
	for (int i = 0; i < array->count; i++)
	{
		iImage* img = (iImage*)array->at(i);
		img->paint(dt, iPointZero);
		if (size.width < img->position.x + img->tex->width)
			size.width = img->position.x + img->tex->width;
		if (size.height < img->position.y + img->tex->height)
			size.height = img->position.y + img->tex->height;
	}
	fbo->unbind();

	// front buffer
	if (methodDrawBefore)
		methodDrawBefore(this, aniDt / _aniDt);

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); // pre-multiplied alpha

	setRGBA(1, 1, 1, alpha);
	Texture* t = texPop;
#if 0 
	drawImage(t, p.x, p.y, 0, t->potHeight - t->height, t->width, t->height,
		scale, scale, 2, 360 * scale, TOP | LEFT, REVERSE_HEIGHT);
#else
	int w = t->width, h = t->height;
	t->width = size.width;
	t->height = size.height;
	//float degree = 360 * scale * 3; while (degree > 360) degree -= 360;
	drawImage(t, p.x, p.y, 0, t->potHeight - t->height, t->width, t->height,
		scale, scale, 2, degree, TOP | LEFT, REVERSE_HEIGHT);
	t->width = w;
	t->height = h;
#endif
	setRGBA(1, 1, 1, 1);

	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	if (methodDrawAfter)
		methodDrawAfter(this, aniDt / _aniDt);
}

void iPopup::show(bool show)
{
	if (show)
	{
		if (bShow)
			return;

		bShow = true;
		stat = iPopupStatOpen;
	}
	else
	{
		if (stat != iPopupStatProc)
			return;

		stat = iPopupStatClose;
	}
	aniDt = 0.0f;
}

void drawPopStyleAlpha(float dt, iPopup* pop, iPoint& position,
	float& alpha, float& scale, float& degree)
{
	position = pop->ep;
	scale = 1.0f;
	if (pop->stat == iPopupStatOpen)
	{
		alpha = linear(0.0f, 1.0f, pop->aniDt / pop->_aniDt);
		pop->aniDt += dt;
		if (pop->aniDt >= pop->_aniDt)
		{
			if (pop->methodOpen)
				pop->methodOpen(pop);
			pop->stat = iPopupStatProc;
		}
	}
	else if (pop->stat == iPopupStatProc)
	{
		alpha = 1.0f;
	}
	else if (pop->stat == iPopupStatClose)
	{
		alpha = linear(1.0f, 0.0f, pop->aniDt / pop->_aniDt);
		pop->aniDt += dt;
		if (pop->aniDt >= pop->_aniDt)
		{
			if (pop->methodClose)
				pop->methodClose(pop);
			pop->bShow = false;
		}
	}
}

void drawPopStyleMove(float dt, iPopup* pop, iPoint& position,
	float& alpha, float& scale, float& degree)
{
	alpha = 1.0f;
	scale = 1.0f;
	if (pop->stat == iPopupStatOpen)
	{
		position = linear(pop->sp, pop->ep, pop->aniDt / pop->_aniDt);
		pop->aniDt += dt;
		if (pop->aniDt >= pop->_aniDt)
		{
			if (pop->methodOpen)
				pop->methodOpen(pop);
			pop->stat = iPopupStatProc;
		}
	}
	else if (pop->stat == iPopupStatProc)
	{
		position = pop->ep;
	}
	else if (pop->stat == iPopupStatClose)
	{
		position = linear(pop->ep, pop->sp, pop->aniDt / pop->_aniDt);
		pop->aniDt += dt;
		if (pop->aniDt >= pop->_aniDt)
		{
			if (pop->methodClose)
				pop->methodClose(pop);
			pop->bShow = false;
		}
	}
}

void drawPopStyleZoom(float dt, iPopup* pop, iPoint& position,
	float& alpha, float& scale, float& degree)
{
	alpha = 1.0f;
	if (pop->stat == iPopupStatOpen)
	{
		position = linear(pop->sp, pop->ep, pop->aniDt / pop->_aniDt);
		scale = pop->aniDt / pop->_aniDt;
		pop->aniDt += dt;
		if (pop->aniDt >= pop->_aniDt)
		{
			if (pop->methodOpen)
				pop->methodOpen(pop);
			pop->stat = iPopupStatProc;
		}
	}
	else if (pop->stat == iPopupStatProc)
	{
		position = pop->ep;
		scale = 1.0f;
	}
	else if (pop->stat == iPopupStatClose)
	{
		position = linear(pop->ep, pop->sp, pop->aniDt / pop->_aniDt);
		scale = 1.0f - pop->aniDt / pop->_aniDt;
		pop->aniDt += dt;
		if (pop->aniDt >= pop->_aniDt)
		{
			if (pop->methodClose)
				pop->methodClose(pop);
			pop->bShow = false;
		}
	}
}