#pragma once

struct Node
{
	Node* prev;
    Node* next;
	void* data;
};

typedef void (*MethodArray)(void* data);
struct iArray
{
	Node* tail; // 마지막 노드 즉, tail
	Node* head; // 제일 첫 노드
	int count;
	MethodArray method;

	iArray(MethodArray method = 0);
	virtual ~iArray();

	// 불특정 데이터를 받기위해 void 타입으로
	void add(void* data);
	void add(int index, void* data); // 몇번째에 데이터를 삽입할 것인가?

	void* at(int index);

	void removeAll();
	void remove(int index);
};