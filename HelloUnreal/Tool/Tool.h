#pragma once

#include "resource.h"

#include "iWindow.h"

void loadApp(HWND hwnd);
void freeApp();
void drawApp(float dt);

#define ITEM_FILTER L"ITM Files(*.ITM, *.itm)\0*.ITM;*.itm\0ALL Files(*.*)\0*.*\0"

