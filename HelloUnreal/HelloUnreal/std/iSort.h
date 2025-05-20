#pragma once

#define SIZE 100

struct Sort
{
	int index;
	float y;
};

typedef bool (*MethodSort)(float f0, float f1);

struct iSort
{
	iSort(int max = 100);
	virtual ~iSort();

	Sort* _s;
	Sort** s;
	int num, max;

	void init(int max = 100);
	void add(float y);
	static bool compare(float f0, float f1)
	{
		return f0 > f1;
	}

	//MethodUpdate method;
	void update(MethodSort method = compare);

	int get(int i) { return s[i]->index; }
};