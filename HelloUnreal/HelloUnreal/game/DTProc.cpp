#include "DTProc.h"

#include "DTLoading.h"
#include "DTLogin.h"

void loadDTProc()
{
	printf("LoadDTProc()");
}

void freeDTProc()
{
	printf("FreeDTProc()");
}

void drawDTProc(float dt)
{
	//setRGBA(0, 0, 1, 1);
	//fillRect(0, 0, devSize.width, devSize.height);
    setRGBA(1, 1, 1, 1);
	static Texture* tex = createImage("assets/down2.jpg");
	drawImage(tex, 0, 0, 0, 0, tex->width, tex->height,
		devSize.width / tex->width, devSize.height / tex->height,
		2, 0, TOP | LEFT);
}

void keyDTProc(iKeyStat stat, iPoint point)
{
	// 키입력 순간 페이드아웃 됐다가 프로시저 화면으로 넘어갈것
	if (stat == iKeyStatBegan)
		setLoading(DTStateLogin, freeDTProc, loadDTLogin);
}
