#pragma once

class iFPS
{
private:
	iFPS();
	
public:
	virtual ~iFPS();

	static iFPS* instance();
	float update();

	unsigned long lastUpdate; // 마지막 시간(이전 시간)
};

