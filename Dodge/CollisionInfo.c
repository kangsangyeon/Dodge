#include "CollisionInfo.h"

#include <stdbool.h>
#include <stdlib.h>

CollisionInfo* CollisionInfo_Create(Vector2D _startPosition, bool** _arr, int _width, int _height)
{
	CollisionInfo* _outInfo = (CollisionInfo*)malloc(sizeof(CollisionInfo));
	_outInfo->startPosition = _startPosition;
	_outInfo->arr = _arr;
	_outInfo->width = _width;
	_outInfo->height = _height;

	return _outInfo;
}

void CollisionInfo_Release(CollisionInfo* _collisionInfo)
{
	if (_collisionInfo == NULL)
		return;

	if (_collisionInfo->arr)
	{
		for (int _y = 0; _y < _collisionInfo->height; ++_y)
		{
			if (_collisionInfo->arr[_y] != NULL)
				free(_collisionInfo->arr[_y]);
		}

		free(_collisionInfo->arr);
	}

	free(_collisionInfo);
}
