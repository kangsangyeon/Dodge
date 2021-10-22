#include "Stack.h"

Stack* createStack(int size)
{
	Stack* outStack = (Stack*)calloc(1, sizeof(Stack));
	outStack->size = size;
	outStack->top = -1;
	outStack->arr = (char*)calloc(size + 1, sizeof(char));

	return outStack;
}

void freeStack(Stack* stack)
{
	free(stack->arr);
	free(stack);
}

bool isEmpty(const Stack* stack)
{
	return stack->top < 0;
}

void push(Stack* stack, char elem)
{
	const bool isOutOfRange = (stack->top < stack->size) == false;

	if (isOutOfRange)
		return;

	++stack->top;
	stack->arr[stack->top] = elem;
}

char pop(Stack* stack)
{
	if (isEmpty(stack) == true)
		return -1;

	char outValue = stack->arr[stack->top];
	stack->arr[stack->top] = 0;

	--stack->top;
	return outValue;
}
