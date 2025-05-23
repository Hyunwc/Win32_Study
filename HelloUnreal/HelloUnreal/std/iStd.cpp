#include "iStd.h"

int keydown = 0;      // 00000000 00000000 00000000 00000011 

ULONG_PTR           gdiplusToken;
HDC hdc;
Graphics* graphics;

// 가상 메모리 
Bitmap* bmp;
Graphics* gFromBmp;

static float _r, _g, _b, _a;

METHOD_VOID methodFree;
METHOD_FLOAT methodDraw;
METHOD_KEY methodKey;

void loadApp(HWND hWnd, METHOD_VOID load, METHOD_VOID free,
	METHOD_FLOAT draw, METHOD_KEY key)
{
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	hdc = GetDC(hWnd);
	graphics = new Graphics(hdc);

	bmp = new Bitmap(640, 480);
	gFromBmp = Graphics::FromImage(bmp);

	_r = 1.0f;
	_g = 1.0f;
	_b = 1.0f;
	_a = 1.0f;

	load();
	methodFree = free;
	methodDraw = draw;
	methodKey = key;
}

void freeApp()
{
	delete graphics;
	GdiplusShutdown(gdiplusToken);
}

void drawApp(float dt)
{
	// 전역변수 그래픽스의 백업
	Graphics* bk = graphics;
	// 백버퍼(후면버퍼)에 그림 업데이트
	graphics = gFromBmp;

	methodDraw(dt);

	graphics = bk;

	// 백버퍼의 그림을 전면 버퍼에 그리기
	graphics->DrawImage(bmp, 0, 0);
	//graphics->DrawString()

	SwapBuffers(hdc);
}

void keyApp(iKeyStat stat, iPoint point)
{
	methodKey(stat, point);
}

void setRGBA(float r, float g, float b, float a)
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}

void clear()
{
	graphics->Clear(Color(_a * 0xFF,
						  _r * 0xFF,
						  _g * 0xFF,
					      _b * 0xFF));
}

void drawLine(float x0, float y0, float x1, float y1)
{
	Pen pen(Color(_a * 0xFF,
				  _r * 0xFF,
				  _g * 0xFF,
				  _b * 0xFF));
	graphics->DrawLine(&pen, x0, y0, x1, y1);
}

void drawLine(iPoint p0, iPoint p1)
{
	drawLine(p0.x, p0.y, p1.x, p1.y);
}

void drawRect(float x, float y, float width, float height)
{
	Pen pen(Color(_a * 0xFF,
		_r * 0xFF,
		_g * 0xFF,
		_b * 0xFF));
	graphics->DrawRectangle(&pen, x, y, width, height);
}

void drawRect(iRect rt)
{
	drawRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height);
}

void fillRect(float x, float y, float width, float height)
{
	SolidBrush brush(Color(_a * 0xFF,
		_r * 0xFF,
		_g * 0xFF,
		_b * 0xFF));
	graphics->FillRectangle(&brush, x, y, width, height);
}

void fillRect(iRect rt)
{
	fillRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height);
}

Texture* createImage(const char* szFormat, ...)
{
	return NULL;
}

void freeImage(Texture* tex)
{
	if (tex->retainCount > 1)
	{
		tex->retainCount--;
		return;
	}

	// real tex 지우기
}

void drawImage(Texture* tex, float x, float y)
{
}

float stringsize = 25.0f;
float sr, sg, sb, sa = 1.0f;

void setStringSize(float size)
{
	stringsize = size;
}

void setStringRGBA(float r, float g, float b, float a)
{
	sr = r; sg = g; sb = b; sa = a;
}

void drawString(float x, float y, const WCHAR* str)
{
	FontFamily  fontFamily(L"Times New Roman");
	Font        font(&fontFamily, stringsize, FontStyleRegular, UnitPixel);
	PointF      pointF(x, y);
	SolidBrush  solidBrush(Color(sa * 0xFF,
								 sr * 0xFF,
								 sg * 0xFF,
								 sb * 0xFF));

	graphics->DrawString(str, -1, &font, pointF, &solidBrush);
}

float linear(float s, float e, float rate)
{
	// 시작값 + 차값(끝값 - 시작값)
	return s + (e - s) * rate;
}

// x : 0 ~ 100
// r : 30  =>  0 + (100 - 0) * r == s + (e-s) * r;

iPoint linear(iPoint s, iPoint e, float rate)
{
	return s + (e - s) * rate;
}

float easeln(float s, float e, float rate)
{
	float r = rate * rate;
	return s + (e - s) * r;
}

iPoint easeln(iPoint s, iPoint e, float rate)
{
	float r = rate * rate;
	return s + (e - s) * r;
}

float easeOut(float s, float e, float rate)
{
	// sin, sq
	return 0.0f;
}

iPoint easeOut(iPoint s, iPoint e, float rate)
{
	return iPointZero;
}

//float clamp2(float f, float min2, float max2)
//{
//	return min(max(f, min2), max2);
//}

//#define clamp(a, b, c) min(max(a, b), c)

float clamp(float f, float min, float max)
{
	if (f < min)
		f = min;
	else if (f > max)
		f = max;

	return f;
}

void move(iPoint* cp, const iPoint* tp, const iPoint& mp)
{
	if (cp->x < tp->x)
	{
		cp->x += mp.x;
		if (cp->x > tp->x)
			cp->x = tp->x;
	}
	else if (cp->x > tp->x)
	{
		cp->x += mp.x;
		if (cp->x < tp->x)
			cp->x = tp->x;
	}

	if (cp->y < tp->y)
	{
		cp->y += mp.y;
		if (cp->y > tp->y)
			cp->y = tp->y;
	}
	else if (cp->y > tp->y)
	{
		cp->y += mp.y;
		if (cp->y < tp->y)
			cp->y = tp->y;
	}
}

char* loadFile(const char* szFormat, ...)
{
	FILE* pf = fopen(szFormat, "rb");
	
	// offset : 파일 끝, origin : 파일 시작 
	fseek(pf, 0, SEEK_END); // 파일 끝
	long len = ftell(pf); 
	// 문자열일수도 있으니 + 1
	char* buf = new char[len + 1];
	
	fseek(pf, 0, SEEK_SET); // 파일 시작
	fread(buf, sizeof(char), len, pf);
	buf[len] = 0;

	fclose(pf);
	return buf;
}

void saveFile(char* buf, int bufLen, const char* szFormat, ...)
{
	FILE* pf = fopen(szFormat, "wb");

	fwrite(buf, sizeof(char), bufLen, pf);

	fclose(pf);
}

