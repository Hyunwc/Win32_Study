#include "Memory.h"


//void cbArray(void* data);

// release -> count를 지우는데 사용하는 용도
// win32에선 releaseDC
//class Super
//{
//	Super()
//	{
//		retainCount = 1;
//	}
//	Super(const Super& s)
//	{
//		retainCount++;
//	}
//
//	int retainCount;
//};

void cbArray(void* data)
{
	char* s = (char*)data;
	printf("%s\n", s);
	delete s;
}

void loadMemory()
{
	void tttttt();
	tttttt();
	// s0 : 문자열의 길이는2, 크기3, 버퍼의 크기는 60
	char s0[60] = "Hi"; // {'H', 'i', 0};
	// stack(60) + heap(3)
	s0[1] = 'o'; // "Ho"
	
	char* s1 = new char[4]; // = "Hi2";
// 아래 문법들 둘다 똑같음
#if 1
	strcpy(s1, "Hi2"); // 4바이트 복사
#else
	memcpy(s1, "Hi2", 4);
	s1[3] = 0;
#endif
	// Hello를 넣고싶다!
	delete s1;

	s1 = new char[6];
	strcpy(s1, "Hello");

	iArray* arr = new iArray(cbArray);

	const char* str[] = {
		"Hello", "World", "!!"
	};

	// 배열 크기 구하기
	//int n = sizeof(str) / sizeof(const char*);

	for (int i = 0; i < 3; i++)
	{
		int len = strlen(str[i]);
		char* t = new char[len + 1];
		strcpy(t, str[i]);
		arr->add(t);
	}

	char* sss = new char[4];
	strcpy(sss, "fun");
	arr->add(1, sss);

	// arr의 첫번째 데이터를 쓰고싶다.
	for (int n = 0; n < arr->count; n++)
	{
		char* t = (char*)arr->at(n);
		printf("arr[%d] = %s\n", n, t);
	}

	delete arr;

}

void freeMemory()
{
}

void drawMemory(float dt)
{
}

void keyMemory(iKeyStat stat, iPoint point)
{
}
