#pragma once
#include <stdbool.h>

typedef struct TIntQueue
{
	int size;
	int front;
	int back;
	int* arr;
} IntQueue;

IntQueue* IntQueue_Create(int _size);

void IntQueue_Release(IntQueue* _queue);

void IntQueue_Resize(IntQueue* _queue, int _size);

bool IntQueue_IsEmpty(IntQueue* _queue);

bool IntQueue_IsFull(IntQueue* _queue);

int IntQueue_GetSize(IntQueue* _queue);

void IntQueue_Enqueue(IntQueue* _queue, int _elem);

int IntQueue_Dequeue(IntQueue* _queue);
