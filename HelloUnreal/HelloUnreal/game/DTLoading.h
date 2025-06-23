#pragma once

#include "iStd.h"

enum DTState
{
	DTStateLogin = 0,
	DTStateProc,

	DTStateMax
};

extern DTState ds;

// 지워야될 화면과 로드해야할 화면
void setLoading(DTState dsNext, METHOD_VOID free, METHOD_VOID load);

void drawDTLoading(float dt);
bool keyDTLoading(iKeyStat stat, iPoint point);
