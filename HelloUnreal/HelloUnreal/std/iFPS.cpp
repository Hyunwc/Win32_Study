#include "iFPS.h"


#if 1 // Windows

// later... (framework.h)
#include <Windows.h>

iFPS::iFPS()
{
    lastUpdate = GetTickCount64();

    count = 0;
    takeTime = 0.000001f;
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
    delta = (now - lastUpdate) / 1000.0f; // 현재 시간 - 이전시간
    lastUpdate = now;

    count++;
    takeTime += delta;
    while (takeTime > fpsInterval)
    {
        int m = count / takeTime;
        takeTime -= fpsInterval;

        count = m * takeTime;
    }

    return delta;
}

int iFPS::framePerSec()
{
    int fps = count / takeTime;
    return fps;
}

#else
#endif