#pragma once

#include "iStd.h"

void loadLemon();
void freeLemon();
void drawLemon(float dt);
void keyLemon(iKeyStat stat, iPoint point);

enum GameState
{
	GameStateMenu,
	GameStateProc,  // 인게임
	GameStateResult // 결과
};

extern GameState gameState;

