#include "iArray.h"


iArray::iArray(MethodArray method)
{
	tail = NULL;
	head = NULL;
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
	n->prev = tail;
	n->data = data;

	tail = n; // 마지막 노드 업데이트
	count++;
}

void iArray::add(int index, void* data)
{
	Node* b = tail; // 백업

	for (int i = count - 1; i > -1; i--)
	{
		if (i == index)
		{
			// n을 가르키는 이전 node
			Node* newNode = new Node; // 새로 삽입할 데이터
			newNode->prev = b->prev;
			newNode->data = data;

			b->prev = newNode;
			count++;
			return;
		}
		b = b->prev;
	}

	// 못 넣었으니까 여기까지 옴. early return 필요 x
	add(data);
}

void* iArray::at(int index)
{
	int i = count - 1;

	for (Node* n = tail; n; n = n->prev, i--)
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
	for (Node* n = tail; n;)
	{
		Node* t = n->prev; // 이전값을 t에 담고
		// 콜백이 등록되어있다면 데이터 지워달라
		if (method)
			method(n->data); // delete
		delete n;
		n = t; 
	}
	tail = NULL;
	count = 0;
}

void iArray::remove(int index)
{
	Node* prevN = NULL; // 이전
	Node* n = tail; // 현재
	int i = count - 1;
	for (Node* n = tail, *prevN = NULL; n; prevN = n, n = n->prev, i--)
	{
		if (i == index)
		{
			if (prevN)
				prevN->prev = n->prev;
			else// if(prevN == NULL)
				tail = n->prev; // 마지막 노드를 업데이트

			if (method)
				method(n->data);
			delete n;
			return;
		}
		/*prevN = n;
		n = n->prev;*/
	}
}
