#pragma once

#include <stdbool.h>
#include <stdlib.h>

typedef struct TPointerStack
{
	int arrCount;
	int top;
	void** arr;
} PointerStack;

PointerStack* PointerStack_Create(int size);

void PointerStack_Release(PointerStack* stack);

bool PointerStack_IsEmpty(const PointerStack* stack);

bool PointerStack_IsFull(const PointerStack* stack);

int PointerStack_GetSize(const PointerStack* stack);

void PointerStack_Push(PointerStack* stack, void* elem);

void* PointerStack_Pop(PointerStack* stack);

void PointerStack_Resize(PointerStack* stack, int size);
