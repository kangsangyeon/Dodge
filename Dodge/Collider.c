#include "Collider.h"

#include <stdbool.h>
#include <stdlib.h>

#include "Helper.h"

Collider* Collider_LoadFromTextFile(wchar_t* _filePath)
{
	wchar_t** _dataArr = NULL;
	int _colliderWidth = 0;
	int _colliderHeight = 0;

	bool _success = LoadTextFileAsMatrix2D(_filePath, &_dataArr, &_colliderWidth, &_colliderHeight);
	if (_success == false)
		return NULL;

	Collider* _collider = (Collider*)malloc(sizeof(Collider));

	_collider->dataArr = _dataArr;
	_collider->width = _colliderWidth;
	_collider->height = _colliderHeight;

	return _collider;
}

void Collider_Release(Collider* _collider)
{
	if (_collider == NULL)
		return;

	if (_collider->dataArr != NULL)
	{
		for (int i = 0; i < _collider->height; ++i)
		{
			if (_collider->dataArr[i] != NULL)
				free(_collider->dataArr[i]);
		}

		free(_collider->dataArr);
	}

	free(_collider);
}
