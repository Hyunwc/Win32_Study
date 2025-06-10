#include "iStd.h"

int keydown, keystat;      // 00000000 00000000 00000000 00000011 
iSize devSize;
iRect viewport;

ULONG_PTR           gdiplusToken;

static float _r, _g, _b, _a;

METHOD_VOID methodFree;
METHOD_FLOAT methodDraw;
METHOD_KEY methodKey;

void loadApp(HWND hWnd, METHOD_VOID load, METHOD_VOID free,
	METHOD_FLOAT draw, METHOD_KEY key)
{
	// openGL 한글지원해주는 함수 없음
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	loadOpenGL(hWnd);

	// back buffer
	keydown = keydown_none;
	keystat = keydown_none;
	devSize = iSizeMake(DEV_WIDTH, DEV_HEIGHT);
	viewport = iRectMake(0, 0, 1, 1);

	_r = 1.0f;
	_g = 1.0f;
	_b = 1.0f;
	_a = 1.0f;

	setMakeCurrent(true);
	load();
	setMakeCurrent(false);
	methodFree = free;
	methodDraw = draw;
	methodKey = key;
}

void freeApp()
{
	methodFree();

	freeOpenGL();
	GdiplusShutdown(gdiplusToken);
}

void drawApp(float dt)
{
	setMakeCurrent(true);
	// ================================
	resizeOpenGL(0, 0);

	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	methodDraw(dt);
	// ================================ line
	//if (1)
	//{
	//	setLineWidth(10);
	//	setRGBA(1, 1, 1, 1);
	//	iPoint sp = iPointMake(10, 10);
	//	iPoint ep = iPointMake(devSize.width - 10,
	//		devSize.height - 10);
	///*	static float delta = 0.0f;
	//	delta += dt;
	//	iPoint mp = iPointMake(50 * sin(delta* 10), 0);
	//	sp += mp;
	//	ep += mp;*/

	//	drawLine(sp, ep);
	//}
	// ================================ rect
	if (0)
	{
		setRGBA(1, 1, 1, 1);
		fillRect(0, 0, devSize.width, devSize.height);

		setRGBA(1, 0, 1, 1);
		fillRect(0, 0, devSize.width - 100, devSize.height - 100);
		setRGBA(1, 1, 1, 1);
	}
	// ================================ Texture
	if (0)
	{
		setRGBA(1, 1, 1, 1);
		static Texture* tex = createImage("assets/down1.png");

		static float delta = 0.0f;
		delta += dt;
		
		float degree = 360 * fabsf(sin(delta * 3));
		//drawImage(tex, 0, 0, TOP | LEFT);
		drawImage(tex, 0, 0, TOP | LEFT);
		
	}
	// ================================ triangle
	if (0)
	{
		float tri[] = {
		0.0f, 0.9f,		1, 0, 0, 1,
		-0.9f, -0.9f,	0, 1, 0, 1,
		0.9f, -0.9f,	0, 0, 1, 1,
		};

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(2, GL_FLOAT, sizeof(float) * 6, &tri[0]);
		glColorPointer(4, GL_FLOAT, sizeof(float) * 6, &tri[2]);

		uint8 indices[] = { 0, 1, 2 };
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, indices);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}
	
	// ================================
	swapBuffer();
	setMakeCurrent(false);
}

void keyApp(iKeyStat stat, iPoint point)
{
	methodKey(stat, point);
}

void setRGBA(float r, float g, float b, float a)
{
	_r = r; _g = g;	_b = b; _a = a;
}

void getRGBA(float& r, float& g, float& b, float& a)
{
	r = _r; g = _g; b = _b; a = _a;
}

void clear()
{
	glClearColor(_r, _g, _b, _a);
	glClear(GL_COLOR_BUFFER_BIT);
}

float lineWidth = 1;
void setLineWidth(float width)
{
	lineWidth = width;
}

void drawLine(float x0, float y0, float x1, float y1)
{
	float position[] = {
		x0, y0, x1, y1
	};

	glLineWidth(lineWidth);
	glColor4f(_r, _g, _b, _a);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, position);

	uint8 indices[] = { 0, 1 };
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_BYTE, indices);

	glDisableClientState(GL_VERTEX_ARRAY);
}

void drawLine(iPoint p0, iPoint p1)
{
	drawLine(p0.x, p0.y, p1.x, p1.y);
}

void drawRect(float x, float y, float width, float height)
{
	drawLine(x, y, x + width, y);
	drawLine(x, y + height, x + width, y + height);
	drawLine(x, y, x, y + height);
	drawLine(x + width, y, x + width, y + height);
}

void drawRect(iRect rt)
{
	drawRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height);
}

void fillRect(float x, float y, float width, float height)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float rt[] = {
		x, y,             _r, _g, _b, _a,    x + width, y,             _r, _g, _b, _a,
		x, y + height,    _r, _g, _b, _a,    x + width, y + height,    _r, _g, _b, _a,
	};
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, sizeof(float) * 6, &rt[0]);
	glColorPointer(4, GL_FLOAT, sizeof(float) * 6, &rt[2]);

	uint8 indices[] = { 0, 1, 2,  2, 1, 3 };
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void fillRect(iRect rt)
{
	fillRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height);
}

uint32 nextPot(uint32 x)
{
	// 1, 2, 4, 8 ,16, 32 ,64, 128, 256, 512
#if 0
	uint32 i = 1;
	for (; i < x; i = i << 1);
	return i;
#else
	x = x - 1;
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >> 16);
	return x + 1;
#endif
}

uint8* bmp2rgba(Bitmap* bmp, int& width, int& height)
{
	Rect rt;
	rt.X = 0, rt.Y = 0,
	rt.Width = bmp->GetWidth(), rt.Height = bmp->GetHeight();
	BitmapData bmpData;
	bmp->LockBits(&rt, ImageLockModeRead, PixelFormat32bppARGB, &bmpData);

	uint8* bgra = (uint8*)bmpData.Scan0;
	int stride = bmpData.Stride;

	int pw = nextPot(rt.Width), ph = nextPot(rt.Height);
	// 픽셀당 4바이트이므로 * 4
	uint8* rgba = new uint8[pw * ph * 4];
	memset(rgba, 0x00, sizeof(uint8) * pw * ph * 4);
	for (int j = 0; j < rt.Height; j++)
	{
		for (int i = 0; i < rt.Width; i++)
		{
			uint8* s = &bgra[stride * j + 4 * i];
			uint8* d = &rgba[pw * 4 * j + 4 * i];
			d[0] = s[2];
			d[1] = s[1];
			d[2] = s[0];
			d[3] = s[3];
		}
	}
	bmp->UnlockBits(&bmpData);

	width = rt.Width;
	height = rt.Height;

	return rgba;
}

#define GL_CLAMP_TO_EDGE 0x812F
Texture* createImageWithRGBA(uint8* rgba, int width, int height)
{
	uint32 texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	// 안티 알리아싱
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// 색상 그라데이션
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);// GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//GL_NEAREST

	int pw = nextPot(width), ph = nextPot(height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pw, ph,
		0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);

	glBindTexture(GL_TEXTURE_2D, 0);

	Texture* tex = new Texture;
	tex->texID = texID;
	tex->width = width;
	tex->height = height;
	tex->potWidth = pw;
	tex->potHeight = ph;
	tex->retainCount = 1;

	return tex;
}

static MethodImageFilter method = NULL;

void setImageFilter(MethodImageFilter method)
{
	::method = method;
}

Texture* createImageFilter(const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szFormat, szText);

	wchar_t* path = utf8_to_utf16(szText);
	//Image* img = Image::FromFile(path);
	Bitmap* bmp = Bitmap::FromFile(path);
	delete path;
	int width, height;
	uint8* rgba = bmp2rgba(bmp, width, height);
	delete bmp;

	if (method)
		method(rgba, width, height, nextPot(width));

	Texture* tex = createImageWithRGBA(rgba, width, height);
	delete rgba;

	return tex;
}

void imageFilterGrey(uint8* bgra, int width, int height, int stride)
{
	//uint8* bgra = bgra;

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			uint8* c = &bgra[stride * 4 * j + 4 * i];
			// 회색처리
			uint8 grey = c[0] * 0.3f + c[1] * 0.4f + c[2] * 0.3f;
			c[0] = grey; // blue
			c[1] = grey; // green
			c[2] = grey; // red
		}
	}
}

void imageFilterMirror(uint8* bgra, int width, int height, int stride)
{
	float rateHeight = 0.5f;
	int h = height * rateHeight;

	int* pixels = (int*)bgra;
	for (int j = 0; j < h; j++)
		memcpy(&pixels[stride * j], &pixels[stride * (int)(j / rateHeight)], sizeof(int) * stride);

	for (int j = h; j < height; j++)
		memset(&pixels[stride * j], 0x00, sizeof(int) * stride);

	uint8 t[4];

	int len = sizeof(uint8) * 4;

	for (int j = 0; j < h / 2; j++)
	{
		for (int i = 0; i < width; i++)
		{
			uint8* s = &bgra[stride * 4 * j + 4 * i];
			uint8* d = &bgra[stride * 4 * (h - 1 - j) + 4 * i];

			memcpy(t, s, len);
			memcpy(s, d, len);
			memcpy(d, t, len);

			s[3] = linear(0xFF, 0x00, (float)j / h);
			d[3] = 0xFF - s[3];
			//uint8 grey = c[0] * 0.3f + c[1] * 0.4f + c[2] * 0.3f; // blue
			//c[0] = grey;
			//c[1] = grey; // green
			//c[2] = grey; // red
		}
	}
}

Texture* createImage(const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szFormat, szText);

	wchar_t* path = utf8_to_utf16(szText);
	//Image* img = Image::FromFile(path);
	Bitmap* bmp = Bitmap::FromFile(path);
	delete path;
	int width, height;
	uint8* rgba = bmp2rgba(bmp, width, height);
	delete bmp;

	Texture* tex = createImageWithRGBA(rgba, width, height);
	delete rgba;

	return tex;
}

void freeImage(Texture* tex)
{
	if (tex->retainCount > 1)
	{
		tex->retainCount--;
		return;
	}

	glDeleteTextures(1, &tex->texID);
	delete tex;
}

void drawImage(Texture* tex, float x, float y, int anc)
{
	drawImage(tex, x, y, 0, 0, tex->width, tex->height, 1.0f, 1.0f,
		2, 0, anc);
}

void drawImage(Texture* tex, float x, float y, 
	int sx, int sy, int sw, int sh, 
	float rateX, float rateY, 
	int xyz, float degree, int anc, int reverse)
{
	int w = sw * rateX;
	int h = sh * rateY;

	switch (anc)
	{
	case TOP | LEFT:									break;
	case TOP | HCENTER:		x -= w / 2;					break;
	case TOP | RIGHT:		x -= w;						break;

	case VCENTER | LEFT:    x;			y -= h / 2;		break;
	case VCENTER | HCENTER:	x -= w / 2;	y -= h / 2;		break;
	case VCENTER | RIGHT:	x -= w;		y -= h / 2;		break;
	case BOTTOM | LEFT:					y -= h;			break;
	case BOTTOM | HCENTER:	x -= w / 2;	y -= h;			break;
	case BOTTOM | RIGHT:	x -= w;		y -= h;			break;
	}

	iPoint p[4] = { { -w / 2, -h/ 2}, {w / 2, -h / 2},
					{ -w / 2, h/ 2},  {w / 2, h / 2 } };

	// REVERSE_WIDTH | REVERSE_HEIGHT => 270
	if (reverse & REVERSE_WIDTH)
	{
		for (int i = 0; i < 4; i++)
			p[i].x *= -1;
	}
	if (reverse & REVERSE_HEIGHT)
	{
		for (int i = 0; i < 4; i++)
			p[i].y *= -1;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x + w / 2, y + h / 2, 0);

	// degree가 0이면 회전할 필요x
	if (degree)
	{
		glRotatef(degree, xyz == 0, xyz == 1, xyz == 2);
	}

	float pw = tex->potWidth, ph = tex->potHeight;
	float texCoord[] = {
		sx/ pw, sy / ph,
		(sx + sw) / pw, sy / ph,
		sx/ pw, (sy+sh)/ ph,
		(sx+sw)/ pw, (sy+sh) / ph,
	};

	float color[] = {
		_r, _g, _b, _a,
		_r, _g, _b, _a,
		_r, _g, _b, _a,
		_r, _g, _b, _a,
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
	glVertexPointer(2, GL_FLOAT, 0, p);
	glColorPointer(4, GL_FLOAT, 0, color);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoord);
	glBindTexture(GL_TEXTURE_2D, tex->texID);

	uint8 indices[] = { 0, 1, 2,  2, 1, 3 };
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}

char* stringName = NULL;
float stringsize = 25.0f, stringLineHeight = 30.0f;
float sr = 1.0f, sg = 1.0f, sb = 1.0f, sa = 1.0f;

const char* getStringName()
{
	return stringName;
}

void setStringName(const char* name)
{
	if (stringName)
	{
		if (strcmp(stringName, name) == 0)
			return;
		delete stringName;
	}
	stringName = iString::copy(name);
}

float getStringSize()
{
	return stringsize;
}

void setStringSize(float size)
{
	stringsize = size;
}

float getStringLineHeight()
{
	return stringLineHeight;
}

void setStringLineHeight(float height)
{
	stringLineHeight = height;
}

void setStringRGBA(float r, float g, float b, float a)
{
	sr = r; sg = g; sb = b; sa = a;
}

void getStringRGBA(float& r, float& g, float& b, float& a)
{
	r = sr; g = sg; b = sb; a = sa;
}

void drawString(float x, float y, int anc, const char* szFormat, ...)
{
	printf("직접 사용하지 말고, iString을 권장\n");
}

class iText
{
public:
	iText()
	{
		bmp = new Bitmap(devSize.width, devSize.height);
		g = Graphics::FromImage(bmp);
	}

	virtual ~iText()
	{
		delete bmp;
		delete g;
	}

	iRect rectOfString(const char* str)
	{
		float bkR, bkG, bkB, bkA;
		float bkFR, bkFG, bkFB, bkFA;
		getRGBA(bkR, bkG, bkB, bkA);
		getStringRGBA(bkFR, bkFG, bkFB, bkFA);
		
		setStringRGBA(1, 1, 1, 1);
#if 0
		setRGBA(0, 0, 0, 0);
		clear();
		iGraphics::drawString(g, 0, 0, TOP | LEFT, str);
#else
		iGraphics::clear(g, 0, 0, 0, 0);
		iGraphics::drawString(g, 0, 0, str);
#endif
		setRGBA(bkR, bkG, bkB, bkA);
		setStringRGBA(bkFR, bkFG, bkFB, bkFA);

		Rect rt;
		rt.X = 0;
		rt.Y = 0;
		rt.Width = bmp->GetWidth();
		rt.Height = bmp->GetHeight();
		// 비트맵 : rgb단위로 접근할수 있는 단위
		BitmapData bmpData;
		bmp->LockBits(&rt, ImageLockModeRead,
			PixelFormat32bppARGB, &bmpData);

		iRect rect = rectOfString((uint8*)bmpData.Scan0,
			bmpData.Stride, rt.Width, rt.Height);

		bmp->UnlockBits(&bmpData);

		return rect;
	}

	iRect rectOfString(uint8* bgra, int stride, int w, int h)
	{
		int left = w; 
		for (int i = 0; i < w; i++)
		{
			bool found = false;
			for (int j = 0; j < h; j++)
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
		if (left == w)
			return iRectMake(0, 0, 0, 0);

		// right -> left
		int right = -1;
		for (int i = w - 1; i > -1; i--)
		{
			bool found = false;
			for (int j = 0; j < h; j++)
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

		if (right == -1)
			return iRectMake(0, 0, 0, 0);
		// top -> bottom
		int top = h;
		for (int j = 0; j < h; j++)
		{
			bool found = false;
			for (int i = 0; i < w; i++)
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
		if (top == h)
			return iRectMake(0, 0, 0, 0);
		// bottom -> top
		int bottom = -1;
		for (int j = h - 1; j > -1; j--)
		{
			bool found = false;
			for (int i = 0; i < w; i++)
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
		if (bottom == -1)
			return iRectMake(0, 0, 0, 0);

		return iRectMake(left, top, right - left + 1, bottom - top + 1);
	}

	Bitmap* bmp;
	Graphics* g;
};


static iText* txt = NULL;

iRect rectOfString(const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szFormat, szText);

	//graphics->MeasureString();
	if (txt == NULL)
		txt = new iText();

	return txt->rectOfString(szText);
}

wchar_t* utf8_to_utf16(const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szFormat, szText);
	int len = MultiByteToWideChar(CP_UTF8, 0, szText, -1, NULL, 0);
	wchar_t* wStr = new wchar_t[len];
	MultiByteToWideChar(CP_UTF8, 0, szText, sizeof(szText), wStr, len);

	return wStr;
}

char* utf16_to_utf8(const wchar_t* wStr)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, wStr, lstrlen(wStr), NULL, 0, 0, NULL);
	char* str = new char[len + 1];
	WideCharToMultiByte(CP_UTF8, 0, wStr, lstrlen(wStr), str, len, 0, NULL);
	str[len] = 0;

	return str;
}

//float rad2deg(float radian)
//{
//	// 2pi(r) = 360(d)
//	// 1(r) = 180 / pi
//	return 180 / M_PI;
//}
//
//float deg2rad(float degree)
//{
//	// 2pi(r) = 360(d)
//	// 2pi / 360(r) = 1(d)
//	return M_PI / 180;
//}

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

bool move(iPoint* cp, const iPoint* tp, const iPoint& mp)
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

	return (cp->x == tp->x && cp->y == tp->y);
}

char* loadFile(int& len, const char* szFormat, ...)
{
	FILE* pf = fopen(szFormat, "rb");
	
	// offset : 이동할 거리, origin : 어떻게 이동할지
	// 파일 끝으로 이동해서
	// len에 현재 위치 값을 반환해서 크기 할당
	fseek(pf, 0, SEEK_END); // 파일 끝
	len = ftell(pf); 
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

