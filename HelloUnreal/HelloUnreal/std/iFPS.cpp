#include "iFPS.h"


#if 1 // Windows

// later... (framework.h)
#include <Windows.h>

iFPS::iFPS()
{
    lastUpdate = GetTickCount();
}

iFPS::~iFPS()
{
     
}

iFPS* iFPS::instance()
{
    static iFPS* ptr = new iFPS();
    return ptr;
}

float iFPS::update()
{
    // 델타타임
    DWORD now = GetTickCount64(); // 현재 시간
    float delta = (now - lastUpdate) / 1000.0f; // 현재 시간 - 이전시간
    lastUpdate = now;

    return delta;
}

#else
#endif