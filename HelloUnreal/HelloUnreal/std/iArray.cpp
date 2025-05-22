#include "iArray.h"


iArray::iArray(MethodArray method)
{
	lastNode = NULL;
	count = 0;
	this->method = method;
}

iArray::~iArray()
{
	removeAll();
}

void iArray::add(void* data)
{
	Node* n = new Node;
	n->prev = lastNode;
	n->data = data;

	lastNode = n; // 마지막 노드 업데이트
	count++;
}

void iArray::add(int index, void* data)
{
	Node* n = lastNode; // 백업

	for (int i = count - 1; i > -1; i--)
	{
		if (i == index)
		{
			// n을 가르키는 이전 node
			Node* t = new Node; // 새로 삽입할 데이터
			t->prev = n->prev;
			t->data = data;

			n->prev = t;
			count++;
			return;
		}
		n = n->prev;
	}

	// 못 넣었으니까 여기까지 옴. early return 필요 x
	add(data);
}

void* iArray::at(int index)
{
	int i = count - 1;

	for (Node* n = lastNode; n; n = n->prev, i--)
	{
		if (i == index)
		{
			return n->data;
		}
	}
	// 여기까지 오면 못찾은 것이므로 NULL 반환
	return NULL;
}

void iArray::removeAll()
{
	for (Node* n = lastNode; n;)
	{
		Node* t = n->prev; // 이전값을 t에 담고
		// 콜백이 등록되어있다면 데이터 지워달라
		if (method)
			method(n->data); // delete
		delete n;
		n = t; 
	}
	lastNode = NULL;
	count = 0;
}

void iArray::remove(int index)
{
	Node* prevN = NULL; // 이전
	Node* n = lastNode; // 현재
	int i = count - 1;
	for (Node* n = lastNode, *prevN = NULL; n; prevN = n, n = n->prev, i--)
	{
		if (i == index)
		{
			if (prevN)
				prevN->prev = n->prev;
			else// if(prevN == NULL)
				lastNode = n->prev; // 마지막 노드를 업데이트

			if (method)
				method(n->data);
			delete n;
			return;
		}
		/*prevN = n;
		n = n->prev;*/
	}
}
