#pragma once

#include "iDefine.h"

#include "iPoint.h"

// ==============================
// Mouse
// ==============================

#define iQueueKeySet(num, method) \
		iQueueKey::getInstance()->set(num, method)
#define iQueueKeyUpdate(dt) \
		iQueueKey::getInstance()->update(dt)
#define iQueueKeyAdd(s, p) \
		iQueueKey::getInstance()->add(s, p)

struct QueueKey
{
	iKeyStat s;
	iPoint p;
};

class iQueueKey
{
private:
	iQueueKey();
public:
	static iQueueKey* getInstance();
	virtual ~iQueueKey();

	void set(int maxNum, METHOD_KEY m);
	void add(iKeyStat s, iPoint p);
	void update(float dt);

	METHOD_KEY method;
	QueueKey* qk;

	int qkNum;
};

// ==============================
// Keboard
// ==============================

//#define iKeyboardSet(n, m) iKeyboard::getInstance()->set(n, m)
#define iKeyboardSet iKeyboard::getInstance
#define iKeyboardAdd(down, parm) iKeyboard::getInstance()->add(down, parm)
#define iKeyboardUpdate(dt) iKeyboard::getInstance()->update(dt)
#define iKeyboardDown(key) iKeyboard::getInstance()->keyDown(key)
#define iKeyboardStat(key) iKeyboard::getInstance()->keyStat(key)

class iKeyboard
{
private:
	iKeyboard();
public:
	static iKeyboard* getInstance();
	virtual ~iKeyboard();

	// 아직, 대기 버퍼를 사용하지 않음
	//void set(int maxNum, METHOD_KEY m);
	void add(bool down, int parm);
private:
	void add(bool pressed, int& keydown, int key);
public:
	void update(float dt);

	bool keyStat(int key);
	bool keyDown(int key);
	// keydown 눌렀을 때 1번 발생
	// keystat 누르고 있을 때 계속 발생
	int keydown, keystat;
};



