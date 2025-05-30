#pragma once

#include "iDefine.h"
#include "iStd.h"

// Collsion X (가지 못하는 곳)
#define CX 9 
#define Inf 0xFFFFFF

struct Path
{
	bool visit; // 방문 여부
	int value;
	int* path; // 어디서 왔는지?
	int pathNum;
};

class iShortestPath
{
public:
	iShortestPath();
	virtual ~iShortestPath();
	void clean();

	void set(uint8* tile, int tileX, int tileY, int tileW, int tileH);
	//void run(int start, int end, int* result, int& resultNum);
	void run(iPoint start, iPoint end, iPoint* result, int& resultNum);

private:
	void remove(int* pat, int& pathNum);

public:
	uint8* tile;
	int tileX, tileY, tileW, tileH;
	Path* path;
};

