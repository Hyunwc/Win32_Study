#include "iStrTex.h"

#include "iStd.h"

iStrTex::iStrTex(MethodSt m)
{
	str = NULL;
	strLength = 0;
	tex = NULL;
	method = m;
}

iStrTex::~iStrTex()
{
	clean();
}

void iStrTex::clean()
{
	if (str)
	{
		delete str;
		str = NULL;
	}
	if (tex)
	{
		freeImage(tex);
		tex = NULL;
	}
}

void iStrTex::set(const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szFormat, szText);

	if (str == NULL || strcmp(szText, str))
	{
		printf("set(%s)\n", szText);
		int len = strlen(szText);
		if (strLength < len + 1)
		{
			if (str)
				delete str;
			str = new char[len + 1];
			strLength = len + 1;
		}
		strcpy(str, szText);

		// renew
		// bmp + graphics -> bgra, rgba -> tex
		Texture* t;
		if (method)
			t = method(szText);
		else
		{
			iGraphics* g = iGraphics::getInstance();

			iRect rt = rectOfString(szText);
			g->init(rt.size.width, rt.size.height);

			//g->drawString(-rt.origin.x, -rt.origin.y, TOP | LEFT, szText);
			g->drawString(0, 0, TOP | LEFT, szText);

			t = g->getTexture();
		}
	
		if (tex == NULL)
			tex = t;
		else
		{
			glDeleteTextures(1, &tex->texID);
			memcpy(tex, t, sizeof(Texture));
			delete t;
		}
	}
}

void iStrTex::paint(float x, float y, int anc, float rateX, float rateY, int xyz, float degree, int reverse)
{
	drawImage(tex, x, y, 0, 0, tex->width, tex->height, rateX, rateY, xyz, degree, anc, reverse);
}

void iStrTex::paint(float x, float y, int anc)
{
	drawImage(tex, x, y, anc);
}

void iStrTex::paint(float x, float y, int anc, const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szFormat, szText);

	set(szText);
	
	drawImage(tex, x, y, anc);
}

Texture* iStrTex::createImage(StInfo* si, int siNum, const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szFormat, szText);

	int lineNum;
	char** line = iString::split(lineNum, szText);

	// 위치 정하기(전체 사이즈 구하기, 글자 크기, 간격, ...)
	float ss = getStringSize();
	float slh = getStringLineHeight();
	int w = 0, h = slh * lineNum;
	for (int i = 0; i < lineNum; i++)
	{
		int t = rectOfString(line[i]).size.width;
		if (w < t)
			w = t;
	}

	iGraphics* g = iGraphics::getInstance();
	g->init(w, h);

	for (int i = 0; i < lineNum; i++)
	{
#if 0
		g->drawString(0, (ss + 10) * i, TOP | LEFT, line[i]);
#endif
		const char* s = line[i];
		int m = 0;
		for (int j = 0, off = 0; s[j]; s++)
		{
			for (int k = 0; k < siNum; k++)
			{
				const char* d = si[k].str;
				if (strncmp(s, d, strlen(d)) == 0)
				{
					// off ~ s[j - 1]
					// white
					
					// s[j] ~ s[j + strlen(d) - 1]
					si[k].color;

					j += strlen(d) - 1;
				}
			}
		}
	}

	Texture* tex = g->getTexture();
	g->clean();
	iString::free(line, lineNum);
	return tex;
}
