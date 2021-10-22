#include "PointerStack.h"

#include <wchar.h>

PointerStack* PointerStack_Create(int size)
{
	PointerStack* outStack = (PointerStack*)calloc(1, sizeof(PointerStack));
	outStack->arrCount = size;
	outStack->top = -1;
	outStack->arr = (void**)calloc(size + 1, sizeof(void*));

	return outStack;
}

void PointerStack_Release(PointerStack* stack)
{
	free(stack->arr);
	free(stack);
}

bool PointerStack_IsEmpty(const PointerStack* stack)
{
	return stack->top < 0;
}

bool PointerStack_IsFull(const PointerStack* stack)
{
	return stack->top >= stack->arrCount - 1;
}

int PointerStack_GetSize(const PointerStack* stack)
{
	return stack->top + 1;
}

void PointerStack_Push(PointerStack* stack, void* elem)
{
	if (PointerStack_IsFull(stack) == true)
		PointerStack_Resize(stack, stack->arrCount * 2);

	++stack->top;
	stack->arr[stack->top] = elem;
}

void* PointerStack_Pop(PointerStack* stack)
{
	if (PointerStack_IsEmpty(stack) == true)
		return NULL;

	void* outValue = stack->arr[stack->top];
	stack->arr[stack->top] = NULL;

	--stack->top;
	return outValue;
}

void PointerStack_Resize(PointerStack* stack, int size)
{
	if (stack->arrCount >= size)
		return;

	void** _newArr = (void**)calloc(size, sizeof(void*));
	const int _bytesToCopy = PointerStack_GetSize(stack) * sizeof(void*);
	const int _bytesDestination = size * sizeof(void*);

	memcpy_s(_newArr, _bytesDestination, stack->arr, _bytesToCopy);

	free(stack->arr);

	stack->arr = _newArr;
	stack->arrCount = size;
}
