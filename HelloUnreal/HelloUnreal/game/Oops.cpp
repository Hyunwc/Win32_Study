#include "Oops.h"

Graphics* gOops;
Texture* texOops;

iPoint clickS, clickE;
bool click = false;
iRect rtClick;

void loadOops()
{
	Bitmap* bmp = new Bitmap(devSize.width, devSize.height);
	Texture* tex = new Texture;
	tex->texID = bmp;
	tex->width = devSize.width;
	tex->height = devSize.height;
	tex->potWidth = devSize.width;
	tex->potHeight = devSize.height;
	tex->retainCount = 1;
	texOops = tex;
	gOops = Graphics::FromImage(bmp);
}

void freeOops()
{
	freeImage(texOops);
	delete gOops;
	//delete texOops;
}

iRect dragRect(const iPoint& s, const iPoint& e)
{
	iRect rt;

	if (s.x < e.x)
	{
		rt.origin.x = s.x;
		rt.size.width = e.x - s.x;
	}
	else
	{
		rt.origin.x = e.x;
		rt.size.width = s.x - e.x;
	}

	if (s.y < e.y)
	{
		rt.origin.y = s.y;
		rt.size.height = e.y - s.y;
	}
	else
	{
		rt.origin.y = e.y;
		rt.size.height = s.y - e.y;
	}

	return rt;
}

void drawOops(float dt)
{
	// back buffer
	static bool bDraw = false;
	if (bDraw == false)
	{
		// back buffer
		bDraw = true;
		Graphics* bk = getGraphics();
		setGraphics(gOops);

		// 화면을 비우고
		setRGBA(0, 0, 0, 0);
		clear();

		const char* s = "한글TEST";
		setStringRGBA(1, 1, 1, 1);
		drawString(0, 0, TOP | LEFT, s);

		setRGBA(1, 0, 0, 1);
		iRect rt = rectOfString(s);
		drawRect(rt);

		const char* str[] = {
			"부산광역시",
			"해운대구",
			"센텀동로99",
			"벽산e센텀클래스원",
			"506호"
		};

		for (int i = 0; i < 5; i++)
		{
			drawString(300, 20 + 40 * i, TOP | RIGHT, str[i]);
			iRect rt = rectOfString(str[i]);
			printf("srt[%d] = (%.0f, %.0f)\n", i, rt.size.width, rt.size.height);
		}

		// restore - front buffer(위에서 작업 내용 전면 버퍼에)
		setGraphics(bk);
	}
	
	setRGBA(1, 1, 1, 1);
	drawImage(texOops, 0, 0, TOP | LEFT);

	setRGBA(1, 0, 0, 1);
	drawRect(rtClick);
	
	if (click)
	{
		iRect rt = dragRect(clickS, clickE);
		setRGBA(0, 0, 1, 0.5f);
		fillRect(rt);

		setRGBA(0, 0, 1, 1.0f);
		drawRect(rt);
	}
}

iRect rectOfString(Bitmap* bmp, iPoint s, iPoint e)
{
	// 드래그한 영역
	iRect rtSrc = dragRect(s, e);
	// 글자가 포함되는 영역
	iRect rtDst;

	///////////////////////////
	Rect rt;
	rt.X = 0;
	rt.Y = 0;
	rt.Width = bmp->GetWidth();
	rt.Height = bmp->GetHeight();
	// 비트맵 : rgb단위로 접근할수 있는 단위
	BitmapData bmpData;
	bmp->LockBits(&rt, ImageLockModeRead,
		PixelFormat32bppARGB, &bmpData);

	uint8* bgra = (uint8*)bmpData.Scan0;
	int stride = bmpData.Stride;

	// 드래그 영역
	int x = rtSrc.origin.x, y = rtSrc.origin.y;
	int w = rtSrc.size.width, h = rtSrc.size.height;
	// 탐색할 왼쪽 시작위치
	int left = 0;
	// left -> right
	for (int i = x; i < w; i++)
	{
		bool found = false;
		int dh = y + h;
		// 높이 탐색
		for (int j = y; j < dh; j++)
		{
			// stride = 버퍼 크기만큼 저장
			if (bgra[stride * j + 4 * i + 3])
			{
				found = true;
				left = i;
				break;
			}
		}
		// 찾았다면
		if (found)
			break;
	}

	// right -> left
	int right = bmp->GetWidth();
	int sx = x - 1;
	for (int i = x + w - 1; i > sx; i--)
	{
		bool found = false;
		int dh = y + h;
		for (int j = y; j < dh; j++)
		{
			// stride = 버퍼 크기만큼 저장
			if (bgra[stride * j + 4 * i + 3])
			{
				found = true;
				right = i;
				break;
			}
		}
		// 찾았다면
		if (found)
			break;
	}
	// top -> bottom
	int top = 0;
	int dh = y + h;
	for (int j = y; j < dh; j++)
	{
		bool found = false;
		for (int i = x; i < w; i++)
		{
			// stride = 버퍼 크기만큼 저장
			if (bgra[stride * j + 4 * i + 3])
			{
				found = true;
				top = j;
				break;
			}
		}
		// 찾았다면
		if (found)
			break;
	}

	// bottom -> top
	int bottom = 0;
	int ty = y - 1;
	for (int j = y + h - 1; j > ty; j--)
	{
		bool found = false;
		for (int i = x; i < w; i++)
		{
			// stride = 버퍼 크기만큼 저장
			if (bgra[stride * j + 4 * i + 3])
			{
				found = true;
				bottom = j;
				break;
			}
		}
		// 찾았다면
		if (found)
			break;
	}

	rtDst.origin.x = left;
	rtDst.origin.y = top;
	rtDst.size.width = right - left + 1;
	rtDst.size.height = bottom - top + 1;

	bmp->UnlockBits(&bmpData);

	///////////////////////////
	return rtDst;
}

int left = 0, right = 1024, top = 0, bottom = 1024;
bool* visit, * canGo;

#if 0
void findPixel(UINT8* bgra,int stride, int x, int y)
{
	visit[stride * y + x] = true;
	if (left < x)	left = x;
	if (right > x)	right = x;
	if (top < y)	top = y;
	if (bottom > y)	bottom = y;

	int index = stride * y + 4 * (x - 1) + 3;
	if (!visit[index] && bgra[index])
		findPixel(bgra, stride, x - 1, y);
	index = stride * y + 4 * (x + 1) + 3;
	if (!visit[index] && bgra[index])
		findPixel(bgra, stride, x + 1, y);
	index = stride * (y - 1) + 4 * x + 3;
	if (!visit[index] && bgra[index])
		findPixel(bgra, stride, x, y - 1);
	index = stride * (y + 1) + 4 * x + 3;
	if (!visit[index] && bgra[index])
		findPixel(bgra, stride, x, y + 1);
}
#elif 1
void _findPixel(UINT8* bgra, int stride, int w, int h, int x, int y)
{
	// initialize
	left = x;
	right = x;
	top = y;
	bottom = y;

	// logic
	canGo[w * y + x] = true;

	for (;;)
	{
		bool exist = false;
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				// color가 존재하지 않다면 패스
				if (canGo[w * j + i] == false
					|| bgra[stride * j + 4 * i + 3] == 0)
					continue;
				
				canGo[w * j + i] = false;
				visit[w * j + i] = true;
				exist = true;
				
				if (i < left)	left = i;
				if (i > right)	right = i;
				if (j < top)	top = j;
				if (j > bottom) bottom = j;

				if (i > 0 && visit[w * j + i - 1] == false)
					canGo[w * j + i - 1] = true;
				if (i < w - 1 && visit[w * j + i + 1] == false)
					canGo[w * j + i + 1] = true;
				if (j > 0 && visit[w * (j - 1) + i] == false)
					canGo[w * (j - 1) + i] = true;
				if (j < h - 1 && visit[w * (j + 1) + i] == false)
					canGo[w * (j + 1) + i] = true;
			}
		}
		// canGo :: visit

		// -1, +1, -w, +w canGo = true
		if (exist == false)
			break;
	}

	//rtClick = iRectMake(left, top, right - left + 1, bottom - top + 1);
}

// 1) 갈수 있는 곳을 지정(canGo)을 다음 프레임에서 호출해야 하는데, 의도치않게 실행
// 2) 'ㅅ'과 관련된 문자의 경우, 정상적으로 구하지 못하는 문제

void findPixel(UINT8* bgra, int stride, int w, int h, int x, int y, bool linked)
{
	if (linked == false)
	{
		_findPixel(bgra, stride, w, h, x, y); // left, right, top, bottom
		return;
	}

	for (;;)
	{
		bool theEnd = true;

		_findPixel(bgra, stride, w, h, x, y);
		for (int j = top; j < bottom; j++)
		{
			for (int i = left; i < right; i++)
			{
				if (visit[w * j + i] == false &&
					bgra[stride * j + 4 * i + 3])
				{
					x = i, y = j;
					theEnd = false;
					break;
				}
			}
		}

		if (theEnd)
			break;
	}
}
#endif

iRect rectOfString(Bitmap* bmp, iPoint p)
{
	Rect rt;
	rt.X = 0; rt.Y = 0;
	rt.Width = bmp->GetWidth(); rt.Height = bmp->GetHeight();
	// 비트맵 : rgb단위로 접근할수 있는 단위
	BitmapData bmpData;
	bmp->LockBits(&rt, ImageLockModeRead | ImageLockModeWrite,
		PixelFormat32bppARGB, &bmpData);

	uint8* bgra = (uint8*)bmpData.Scan0;
	int stride = bmpData.Stride;

	////////////////////////////////
	// 사용되는 영역
	visit = new bool[1024 * 1024];
	canGo = new bool[1024 * 1024];
	memset(visit, false, sizeof(bool) * 1024 * 1024);
	memset(canGo, false, sizeof(bool) * 1024 * 1024);

	int w = rt.Width, h = rt.Height;

	//findPixel(bgra, stride, w, h, p.x, p.y, true);
	// 연결 되어있지 않은데도 포함이 되는 버그
	findPixel(bgra, stride, w, h, p.x, p.y, false);

	delete visit;
	delete canGo;
	////////////////////////////////

	bmp->UnlockBits(&bmpData);

	return iRectMake(left, top, right - left + 1, bottom - top + 1);
}

void keyOops(iKeyStat stat, iPoint point)
{
	if (stat == iKeyStatBegan)
	{
		clickS = point;
		click = true;
	}
	else if (stat == iKeyStatMoved)
	{
		clickE = point;
	}
	else if (stat == iKeyStatEnded)
	{
		click = false;

		// 오차범위 보정
		if (iPointLength(clickS - clickE) < 3)
			rtClick = rectOfString((Bitmap*)texOops->texID, point);
		else
			rtClick = rectOfString((Bitmap*)texOops->texID, clickS, clickE);
	}
}
