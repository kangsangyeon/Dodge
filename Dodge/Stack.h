#pragma once

#include <stdbool.h>
#include <stdlib.h>

typedef struct TStack
{
	int size;
	int top;
	char* arr;
} Stack;

Stack* createStack(int size);

void freeStack(Stack* stack);

bool isEmpty(const Stack* stack);

void push(Stack* stack, char elem);

char pop(Stack* stack);
