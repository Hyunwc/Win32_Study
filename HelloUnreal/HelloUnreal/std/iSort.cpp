#include "iSort.h"


iSort::iSort(int max)
{
	// 메모리 상엔 2개 밖에 생성안됨
	_s = new Sort[max];
	s = new Sort * [max];
	for (int i = 0; i < max; i++)
		s[i] = &_s[i];

	num = 0;
	this->max = max;
}

iSort::~iSort()
{
	delete _s;
	delete s;
}

void iSort::init(int max)
{
	if (this->max < max)
	{
		delete _s;
		delete s;

		_s = new Sort[max];
		s = new Sort * [max];
		for (int i = 0; i < max; i++)
			s[i] = &_s[i];

		this->max = max;
	}
	num = 0;
}

void iSort::add(float y)
{
	Sort* t = s[num];
	t->index = num;
	t->y = y;
	num++;
}

void iSort::update(MethodSort method)
{
	Sort* t;
	// for문 안에 num이 1만일 경우? 부하가 클것.
	// 따로 빼놓는게 좋음
	int n = num - 1;
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < num; j++)
		{
			if (method(s[i]->y, s[j]->y))
			{
				t = s[i];
				s[i] = s[j];
				s[j] = t;
			}
		}
	}
}

//int iSort::get(int i)
//{
//	return s[i].index;
//}
