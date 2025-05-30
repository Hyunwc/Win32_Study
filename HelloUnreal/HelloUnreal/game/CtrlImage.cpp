#include "CtrlImage.h"

#include "iStd.h"

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
	Image* img = bmp;

	Rect rt;
	rt.X = 0;
	rt.Y = 0;
	rt.Width = bmp->GetWidth();
	rt.Height = bmp->GetHeight();
	// 비트맵 : rgb단위로 접근할수 있는 단위
	BitmapData bmpData;
	bmp->LockBits(&rt, ImageLockModeRead | ImageLockModeWrite,
		PixelFormat32bppARGB, &bmpData);

	//int* pixels = (int*)bmpData.Scan0;
	uint8* bgra = (uint8*)bmpData.Scan0;
	//bmpData.Stride;

	if (method)
		method((uint8*)bmpData.Scan0, bmp->GetWidth(), bmp->GetHeight(), bmpData.Stride / 4);

	bmp->UnlockBits(&bmpData);

	Texture* tex = new Texture;
	tex->texID = img;
	tex->width = img->GetWidth();
	tex->height = img->GetHeight();
	tex->potWidth = nextPot(img->GetWidth());
	tex->potHeight = nextPot(img->GetHeight());
	tex->retainCount++;

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
			uint8 grey = c[0] * 0.3f + c[1] * 0.4f + c[2] * 0.3f; // blue
			c[0] = grey;
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
		memcpy(&pixels[stride * j], &pixels[stride * (int)(j / rateHeight)], sizeof(int)*stride);

	for (int j = h; j < height; j++)
		memset(&pixels[stride * j], 0x00, sizeof(int) * stride);

	uint8 t[4];

	int len = sizeof(uint8) * 4;

	for (int j = 0; j < h / 2; j++)
	{
		for (int i = 0; i < width; i++)
		{
			uint8* s = &bgra[stride * 4 * j + 4 * i];
			uint8* d = &bgra[stride * 4 * (h - 1-j) + 4 * i];

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


#if 0

#endif