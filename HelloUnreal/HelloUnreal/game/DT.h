#pragma once

#include "iStd.h"


void loadDT();
void freeDT();
void drawDT(float dt);
void keyDT(iKeyStat stat, iPoint point);

// DTLogin.h / cpp
// 로그인 (사원번호 / 비밀번호)

// DTProc.h /cpp (Process)
// DTNotice.h / cpp
// 공지사항(점검 / 유의사항)
// DTProcObj(SuperClass) + DTProcMake(SubClass)
// DTProcRepair.h / cpp
// 공정(하나의 부품 -> 제품화 되는 과정)
// DTNoticeUI.h / cpp
// 고장났을때 표시, 알림, 수리
// DTNoticePop.h / cpp
// 수주 관련 정보(팝업)

// 로그아웃