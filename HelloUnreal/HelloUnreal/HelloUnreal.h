#pragma once

#include "resource.h"

#include "iDefine.h"
void resize(int width, int height);
iPoint convertCoord(float x, float y);
void setCurrentMonitor(HWND hwnd, RECT& rt);
void setWindowStyle(bool fullscreen, DWORD& dwExStyle, DWORD& dwStyle);
void enforceSize(int edge, RECT& rect);
void goFullscreen();

#if 0

Win32 API + GDI / GDI +
OpenGL1.x
OpenGL3.x + glsl
OpenAL

디버깅 시작 F5
브레이크 포인트 F9
스텝 오버 F10(해당 라인을 실행)
스텝 인투 F11(함수등에 들어갈 수 있는 곳으로 실행)
Q) 실수형의 값을 디버깅 모드에서 알 수 없나 ? !
Q) include ", < 구분
- "" << 사용자가 직접 만든 헤더 파일
- <> std에서 지원하는 라이브러리
Q) 디버깅 nan, inf 언제 등장
Q) 링크리스트 헤드타입
Q) WM_SIZING reinterpret_cast
Q) iPointAbs(수식) 왜 에러 ?

#endif



