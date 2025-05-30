#include "iShortestPath.h"

iShortestPath::iShortestPath()
{
	tile = NULL;
	tileX = 0;
	tileY = 0;
	tileW = 0;
	tileH = 0;
	path = NULL;
}

iShortestPath::~iShortestPath()
{
	clean();
}

void iShortestPath::clean()
{
	if (tile)
	{
		delete tile;
		tile = NULL;
	}
	if (path)
	{
		int len = tileX * tileY;
		for (int i = 0; i < len; i++)
			delete path[i].path;
		delete path;
		path = NULL;
	}
}

void iShortestPath::set(uint8* tile, int tileX, int tileY, int tileW, int tileH)
{
	int len = tileX * tileY;
	if (this->tileX * this->tileY >= len)
	{
		this->tileW = tileW;
		this->tileH = tileH;
		memcpy(this->tile, tile, sizeof(uint8) * len);
		return;
	}

	clean();

	uint8* t = new uint8[len];
	memcpy(t, tile, sizeof(uint8) * len);
	this->tile = t;
	this->tileX = tileX; this->tileY = tileY;
	this->tileW = tileW; this->tileH = tileH;

	path = new Path[len];
	for (int i = 0; i < len; i++)
	{
		path[i].path = new int[len];
	}
}


// 다익스트라
// 모든 방문지역 1번만 방문해서 시간 단축 목적

// 진행방법
// 1) 모든 방문지를 무한값 + 방문X + 경로 0개)+ 출발지 값 0
// 2) 루프(한번에 한곳만 방문) : 더이상 방문할 수 없으면 끝
//    - 방문하지않고 이동가능한 경로 중 -> 가장 작은 값
// 3) 목표지역 정보가 얻으려는 정보
void iShortestPath::run(iPoint start, iPoint end, iPoint* result, int& resultNum)
{
	int x = (int)start.x / tileW;
	int y = (int)start.y / tileH;
	int s = tileX * y + x;

	x = (int)end.x / tileW;
	y = (int)end.y / tileH;
	int e = tileX * y + x;

	// initialize...
	int len = tileX * tileY;
	for (int i = 0; i < len; i++)
	{
		Path* p = &path[i];
		p->visit = false;
		p->value = Inf;
		p->pathNum = 0;
	}
	path[s].value = 0;
	tile[s] = 0;
	// 
	for (;;)
	{
		int index = -1;
		int m = Inf;
		for (int i = 0; i < len; i++)
		{
			Path* p = &path[i];
			if (tile[i] == CX ||
				p->visit) continue;
			if (m > p->value)
			{
				m = p->value;
				index = i;
			}
		}
		if (index == -1)
			break;

		Path* curr = &path[index];
		curr->visit = true;
		// #1
		curr->path[curr->pathNum] = index;
		curr->pathNum++;

		for (int i = 0; i < 4; i++)
		{
			int idx;
			if (i == 0)// left
			{
				if (index % tileX == 0)
					continue;
				idx = index - 1;
			}
			else if (i == 1)// right
			{
				if (index % tileX == tileX - 1)
					continue;
				idx = index + 1;
			}
			else if (i == 2)// up
			{
				if (index / tileX == 0)
					continue;
				idx = index - tileX;
			}
			else// if (i == 3)// down
			{
				if (index / tileX == tileY - 1)
					continue;
				idx = index + tileX;
			}
			if (tile[idx] == CX)
				continue;
		
			Path* p = &path[idx];
			if (p->value > curr->value + tile[idx])
			{
				p->value = curr->value + tile[idx];
				// #2
				memcpy(p->path, curr->path, sizeof(int) * curr->pathNum);
				//p->path[curr->pathNum] = idx;
				p->pathNum = curr->pathNum;
			}
		}
	}

	Path* p = &path[e];

	// result
	/*Path* p = &path[e];
	resultNum = p->pathNum;
	for (int i = 0; i < resultNum; i++)
	{
		int x = p->path[i] % tileX;
		int y = p->path[i] / tileX;
		result[i] = iPointMake(tileW * x + tileW / 2, tileH * y + tileH / 2);
	}
	result[0] = start;
	result[resultNum - 1] = end;*/

	//remove(p->path, p->pathNum);

	int j = p->pathNum - 1;
	for (int i = 1; i < j; i++)
	{
		int x = p->path[i] % tileX;
		int y = p->path[i] / tileX;
		result[i] = iPointMake(tileW * x + tileW / 2,
							   tileH * y + tileH / 2);
	}

	result[0] = start;
	result[j] = end;
	resultNum = p->pathNum;
}

void iShortestPath::remove(int* path, int& pathNum)
{
    // 대각선 처리
	int n = pathNum - 2;
	for (int i = 0; i < n; i++)
	{
		int cx = path[i] % tileX;
		int cy = path[i] / tileX;
		int nx = path[i + 2] % tileX;
		int ny = path[i + 2] / tileX;
		if (abs(cx - nx) == 1 && abs(cy - ny) == 1)
		{
			// a ~ b : b - a + 1
			int len = pathNum - 1 - i - 2 + 1;// a=i+2, b=p->pathNum-1
			memcpy(&path[i + 1], &path[i + 2], sizeof(int) * len);
			pathNum--;
			n--;
			//i--;
		}
	}

	// 직선 사이 중복 처리
	for (int i = 0; i < n; i++)
	{
		int cx = path[i] % tileX;
		int cy = path[i] / tileX;
		// 바로 다음 경로들
		int nx = path[i + 1] % tileX;
		int ny = path[i + 1] / tileX;

		bool sameX = (cx == nx);

		if (sameX == false)
		{
			// 대각선까지 중복되는걸 제거하고 싶지만, 패스
			if (cy != ny)
				continue; 
		}

		int j;

		for (j = i + 2; j < pathNum; j++)
		{
			if (sameX)
			{
				if (cx != path[j] % tileX)
				{
					break;
				}
			}
			else
			{
				if (cy != path[j] / tileX)
				{
					break;
				}
			}
		}

		// 제거대상 i + 1 ~ j - 1;
		int len = pathNum - 1 - j + 1; // j, pathNum - 1
		memcpy(&path[i + 1], &path[j], sizeof(int) * len);
	}
}



#if 0
void iShortestPath::run(iPoint start, iPoint end, iPoint* result, int& resultNum)
{
	int x = (int)start.x / tileW;
	int y = (int)start.y / tileH;
	int s = tileX * y + x;

	x = (int)end.x / tileW;
	y = (int)end.y / tileH;
	int e = tileX * y + x;

	int len = tileX * tileY;
	// 초기화
	for (int i = 0; i < len; i++)
	{
		Path* p = &path[i];
		p->visit = false;
		p->value = Inf;
		p->pathNum = 0;
	}

	path[s].value = 0; // 출발점 값 0
	tile[s] = 0; // 현재 자기가 서있는 곳은 갈 수 있는 곳으로

	// 루프(방문하지 않고 +  값이 가장 작은 값 부터 방문)
	for (;;)
	{
		int index = -1; // 방문할 곳 index
		int min = Inf;
		// 경로 탐색
		for (int i = 0; i < len; i++)
		{
			Path* p = &path[i];
			// 방문한 곳과 가지 못하는 곳
			if (p->visit || tile[i] == CX) continue;

			if (p->value < min)
			{
				// 갱신
				min = p->value;
				index = i;
			}
		}

		// 더 이상 방문할 곳이 없음
		if (index == -1)
			break;

		Path* curr = &path[index];
		curr->visit = true;
		curr->path[curr->pathNum] = index;
		curr->pathNum++;

		//양끝에 있을 경우의 로직들
		for (int i = 0; i < 4; i++)
		{
			int idx;
			if (i == 0) // 왼쪽
			{
				if (index % tileX == 0) continue;
				idx = index - 1;
			}
			else if (i == 1) // 오른쪽
			{
				if (index % tileX == tileX - 1) continue;
				idx = index + 1;
			}
			else if (i == 2) // 위
			{
				if (index / tileX == 0) continue;
				idx = index - tileX;
			}
			else // if (i == 3) // 아래
			{
				if (index / tileX == tileY - 1) continue;
				idx = index + tileX;
			}

			if (tile[idx] == CX) continue;
			Path* p = NULL;
			p = &path[idx];
			if (p->value < curr->value) continue;

			p->value = curr->value + tile[idx];
			memcpy(p->path, curr->path, sizeof(int) * curr->pathNum);
			p->pathNum = curr->pathNum;
		}
	}

	Path* p = &path[e]; // 결과
	//memcpy(result, path[e].path, sizeof(int) * p->pathNum);
#if 1// 직선 사이에 필요없는 방문지 정리
	// x부터
	// 만약 curx랑 nextx 즉[i +1]이 같다면
	int n = p->pathNum - 1;
	int testCnt = 0;
	for (int i = 1; i < n; i++)
	{
		int cx = p->path[i] % tileX;
		int cy = p->path[i] / tileX;
		int nx = p->path[i + 1] % tileX;
		int ny = p->path[i + 1] / tileX;
		printf("현재x : %d, 다음x : %d\n", cy, ny);

		if ((cy == ny) || (cx == nx))
		{
			testCnt++;
			//p->pathNum--;
		}
		else
		{
			continue;
		}
	}
	printf("탐색 후 : %d\n", testCnt);

#endif 
#if 0// 대각선 방문처리
	int n = p->pathNum - 2;
	for (int i = 0; i < n; i++)
	{
		int cx = p->path[i] % tileX;
		int cy = p->path[i] / tileX;
		int nx = p->path[i + 2] % tileX;
		int ny = p->path[i + 2] / tileX;
		if (abs(cx - nx) == 1 && abs(cy - ny) == 1)
		{
			// i + 2, p->pathNum - 1
			int len = p->pathNum - 1 - i - 2 + 1; // a = i+2, b=p->pathNum - 1
			memcpy(&p->path[i + 1], &p->path[i + 2], sizeof(int) * len);
			p->pathNum--;
			n--;
			//i--;
		}
	}

#endif
	int j = p->pathNum - 1;
	for (int i = 1; i < j; i++)
	{
		int x = p->path[i] % tileX;
		int y = p->path[i] / tileX;
		result[i] = iPointMake(tileW * x + tileW / 2,
							   tileH * y + tileH / 2);
	}

	result[0] = start;
	result[j] = end;
	resultNum = p->pathNum;
}
#endif


#pragma region 기존 runSP 
#if 0
bool runSP(uint8* tile, int tileX, int tileY, int s, int e, int* result, int& resultNum)
{
	int len = tileX * tileY;
	Path* path = new Path[len];
	for (int i = 0; i < len; i++)
	{
		Path* p = &path[i];
		p->visit = false;
		p->value = Inf;
		p->path = new int[len];
		p->pathNum = 0;
	}

	path[s].value = 0; // 출발점 값 0
	tile[s] = 0; // 현재 자기가 서있는 곳은 갈 수 있는 곳으로

	// 루프(방문하지 않고 +  값이 가장 작은 값 부터 방문)
	for (;;)
	{
		int index = -1; // 방문할 곳 index
		int min = Inf;
		for (int i = 0; i < len; i++)
		{
			Path* p = &path[i];
			// 방문한 곳과 가지 못하는 곳
			if (p->visit || tile[i] == CX) continue; 

			if (p->value < min)
			{
				// 갱신
				min = p->value;
				index = i;
			}
		}

		// 더 이상 방문할 곳이 없음
		if (index == -1)
			break;
		Path* curr = &path[index];
		curr->visit = true;
		curr->path[curr->pathNum] = index;
		curr->pathNum++;

		for (int i = 0; i < 4; i++)
		{
			int idx;
			if (i == 0) // 왼쪽
			{
				if (index % tileX == 0) continue;
				idx = index - 1;
			}
			else if (i == 1) // 오른쪽
			{
				if (index % tileX == tileX - 1) continue;
				idx = index + 1;
			}
			else if (i == 2) // 위
			{
				if (index / tileX == 0) continue;
				idx = index - tileX;
			}
			else // if (i == 3) // 아래
			{
				if (index / tileX == tileY - 1) continue;
				idx = index + tileX;
			}

			if (tile[idx] == CX) continue;
			Path* p = NULL;
			p = &path[idx];
			if (p->value < curr->value) continue;

			p->value = curr->value + tile[idx];
			memcpy(p->path, curr->path, sizeof(int) * curr->pathNum);
			p->pathNum = curr->pathNum;
		}

	}

	Path* p = &path[e]; // 결과
	memcpy(result, path[e].path, sizeof(int) * p->pathNum);
	resultNum = p->pathNum;

	for (int i = 0; i < len; i++)
		delete path[i].path;
	delete path;

	return true;
}
#endif
#pragma endregion

#pragma region 테스트용 
#if 0
bool runSP(uint8* tile, int tileX, int tileY, int s, int e, int* path, int& pathNum)
{

	path[0] = s;
	path[1] = e;
	pathNum = 2;

	return true;
}
#endif
#pragma endregion 

