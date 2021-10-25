#include "IntQueue.h"

#include <stdlib.h>
#include <string.h>

IntQueue* IntQueue_Create(int _size)
{
	IntQueue* outQueue = (IntQueue*)calloc(1, sizeof(IntQueue));
	outQueue->size = _size;
	outQueue->front = 0;
	outQueue->back = -1;
	outQueue->arr = (int*)calloc(_size, sizeof(int));

	return outQueue;
}

void IntQueue_Release(IntQueue* _queue)
{
	if (_queue == NULL)
		return;

	if (_queue->arr != NULL)
		free(_queue->arr);

	free(_queue);
}

void IntQueue_Resize(IntQueue* _queue, int _size)
{
	int* _newArr = (int*)calloc(_size, sizeof(int));
	memcpy_s(_newArr, _size * sizeof(int),
	         _queue->arr, _queue->size * sizeof(int));

	free(_queue->arr);

	_queue->size = _size;
	_queue->arr = _newArr;
}

bool IntQueue_IsEmpty(IntQueue* _queue)
{
	return _queue->back < _queue->front;
}

bool IntQueue_IsFull(IntQueue* _queue)
{
	if (_queue->back >= _queue->size - 1)
		return true;

	return false;
}

int IntQueue_GetSize(IntQueue* _queue)
{
	return _queue->back - _queue->front + 1;
}

void IntQueue_Enqueue(IntQueue* _queue, int _elem)
{
	if (IntQueue_IsFull(_queue) == true)
		IntQueue_Resize(_queue, _queue->size * 2);

	++_queue->back;
	_queue->arr[_queue->back] = _elem;
}

int IntQueue_Dequeue(IntQueue* _queue)
{
	if (IntQueue_IsEmpty(_queue) == true)
		return -1;

	const int retVal = _queue->arr[_queue->front];
	_queue->arr[_queue->front] = 0;
	++_queue->front;

	if (IntQueue_IsEmpty(_queue) == true)
	{
		_queue->front = 0;
		_queue->back = -1;
	}

	return retVal;
}
