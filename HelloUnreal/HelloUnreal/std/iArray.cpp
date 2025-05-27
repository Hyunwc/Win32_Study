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

#pragma region 이전 버전 Add
//void iArray::add(void* data)
//{
//	Node* newNode = new Node;
//
//	/*if (head == NULL)
//		head = tail;*/
//
//	newNode->prev = tail;
//	newNode->data = data;
//	
//	tail = newNode; // 마지막 노드 업데이트
//	//tail->next = NULL;
//	count++;
//}
#pragma endregion

void iArray::add(void* data)
{
	Node* newNode = new Node;
	newNode->data = data;

	if (head == NULL)
	{
		// 둘이 같은 주소를 공유
		head = newNode;
		tail = newNode;
		count++;
		return;
	}
	// 여기서 처음에 헤드랑 공유됐으므로 헤드의 next도 new로
	// tail을 새로운 new로 덮어썼으므로 헤드와는 별개가됨
	tail->next = newNode;
	newNode->prev = tail;
	tail = newNode; // tail 업데이트
	count++;
}

void iArray::add(int index, void* data)
{
	// 예외처리 : 인덱스가 음수이거나 count를 초과할경우
	if (index < 0 || index > count)
	{
		printf("Out of range!!!\n");
		return;
	}

	Node* b = head; // 백업

	Node* newNode = new Node;
	newNode->data = data;

	//index 0
	if (index == 0)
	{
		head->prev = newNode;
		newNode->next = head;
		head = newNode;
		count++;
		// 리스트가 비어있는 상태에서 이함수가 호출ㄷ됐을 경우
		if (count == 0)
			tail = newNode;

		return;
	}
	// 가장 끝 -> tail 바로 뒤에 삽입
	if (index == count)
	{
		tail->next = newNode;
		newNode->prev = tail;
		tail = newNode;
		count++;
		return;
	}
	// 중간 삽입
	for (int i = 1; i <= count - 1; i++)
	{
		if (i = index)
		{
			newNode->next = b->next;
			b->next->prev = newNode;
			b->next = newNode;
			newNode->prev = b;
			count++;
			break;
		}

		b = b->next; // 업데이트
	}
}

#pragma region 이전 버전 Add(index, data)
//void iArray::add(int index, void* data)
//{
//	Node* b = tail; // 백업
//
//	for (int i = count - 1; i > -1; i--)
//	{
//		if (i == index)
//		{
//			// n을 가르키는 이전 node
//			Node* newNode = new Node; // 새로 삽입할 데이터
//			newNode->prev = b->prev;
//			newNode->data = data;
//
//			b->prev = newNode;
//			count++;
//			return;
//		}
//		b = b->prev;
//	}
//
//	// 못 넣었으니까 여기까지 옴. early return 필요 x
//	add(data);
//}
#pragma endregion 

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
	Node* prevN = NULL;
	Node* n = tail;
	for (int i = count - 1; i > -1; i--)
	{
		if (i == index)
		{
			if (prevN)
				prevN->prev = n->prev;
			else// if (prevN == NULL)
				tail = n->prev;

			if (method)
				method(n->data);
			delete n;
			return;
		}

		prevN = n;
		n = n->prev;
	}
}
