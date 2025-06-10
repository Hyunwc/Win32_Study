#include "ImageText.h"

ImageText** it;

void loadImageText()
{
	setStringSize(32);
	setStringRGBA(1, 1, 1, 1);

	it = new ImageText * [3];
	for (int i = 0; i < 2; i++)
	{
		setStringName((i == 0) ? "궁서체" : "굴림체");
		it[i] = new ImageText();
		it[i]->loadFont("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ");
	}

	const char* strName[5] = {
		"abc.png", "def.png", "down0.png",
		"down1.png", "down2.jpg"
	};

	Texture** texs = new Texture * [256];
	memset(texs, 0x00, sizeof(Texture*) * 256);
	for (int i = 0; i < 5; i++)
	{
		//texs['a' + i] = createImage("assets/%s", strName[i]);
		Texture* t = createImage("assets/%s", strName[i]);
		t->width /= 5.0f;
		t->height /= 5.0f;
		t->potWidth /= 5.0f;
		t->potHeight /= 5.0f;
		texs['a' + i] = t;
	}

	it[2] = new ImageText();
	it[2]->loadImage(texs);
}

void freeImageText()
{
	for (int i = 0; i < 3; i++)
		delete it[i];
	delete it;
}

void drawImageText(float dt)
{
	for(int i = 0; i < 2; i++)
		it[i]->paint(0, 300 + 40 * i, TOP | LEFT, "HELLO MY WORLD 1818");
	it[2]->paint(0, 200, TOP | LEFT, "abcde");
	it[2]->paint(0, 400, TOP | LEFT, "cabde");

	int fps = iFPS::instance()->framePerSec();
	it[0]->paint(devSize.width / 2, 5, TOP | LEFT, "FPS %d", fps);
}

void keyImageText(iKeyStat stat, iPoint point)
{
}

ImageText::ImageText()
{
	texs = new Texture * [256];
	memset(texs, 0x00, sizeof(Texture*) * 256);

	interval = 1;
}

ImageText::~ImageText()
{
	for (int i = 0; i < 256; i++)
	{
		if (texs[i])
			freeImage(texs[i]);
	}
	delete texs;
}

//setStringSize(32);
//setStringRGBA(1, 1, 1, 1);
//const char* s = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ";

void ImageText::loadFont(const char* s)
{
	// 외부에서 지정한 폰트를 그려내는 과정
	iGraphics* g = iGraphics::getInstance();
	
	// i < strlen(s); << 하지마라. 프로그래밍 못해보인다.
	for (int i = 0; s[i]; i++)
	{
		char c = s[i];
		char str[] = { c, 0 };
		iRect rt = rectOfString(str);
		if (rt.size.width == 0 || rt.size.height == 0)
			rt.size = iSizeMake(10, 20);

		g->init(rt.size.width, rt.size.height);

		g->drawString(0, 0, TOP | LEFT, str);

		texs[c] = g->getTexture();
		g->clean();
	}
}

void ImageText::loadImage(Texture** texs)
{
	for (int i = 0; i < 256; i++)
	{
		if (texs[i] == NULL) continue;

		this->texs[i] = texs[i];
		texs[i]->retainCount++;
	}
}

void ImageText::paint(int x, int y, int anc, const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szFormat, szText);

	for (int i = 0; szText[i]; i++)
	{
		Texture* t = texs[szText[i]];
		drawImage(t, x, y, TOP | LEFT);

		// +1 => 글자마다 간격
		x += t->width + interval;
	}
}
