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

bool Collider_CheckCollision(Collider* _source, Vector2D _sourcePosition, Collider* _other, Vector2D _otherPosition)
{
	const Vector2D _offset = Vector2D_Sub(_otherPosition, _sourcePosition);

	// _other의 위치가 _source의 위치에 비해 어느 쪽에 있는지에 대한 값을 얻습니다.
	// _other가 _source보다 x축에서 오른쪽에 있으면 true입니다.
	const bool _directionToOtherX = _offset.x >= 0;
	// _other가 _source보다 y축에서 위쪽에 있으면 true입니다.
	const bool _directionToOtherY = _offset.y >= 0;


	const int _sourceStartX = _directionToOtherX ? _offset.x : 0;
	const int _overlappedXCount = _directionToOtherX
		                              ? _source->width - _otherPosition.x
		                              : _other->width - (-_otherPosition.x);

	const int _sourceStartY = _directionToOtherY ? _offset.y : 0;
	const int _overlappedYCount = _directionToOtherY
		                              ? _source->height - _otherPosition.y
		                              : _other->height - (-_otherPosition.y);

	const int _otherStartX = _directionToOtherX
		                         ? 0
		                         : _overlappedXCount;

	const int _otherStartY = _directionToOtherY
		                         ? 0
		                         : _overlappedYCount;

	int _sourceY = _sourceStartY;
	int _otherY = _otherStartY;

	for (;
		_sourceY < _source->height && _otherY < _other->height;
		++_sourceY, ++_otherY)
	{
		int _sourceX = _sourceStartX;
		int _otherX = _otherStartX;

		for (;
			_sourceX < _source->width && _otherX < _other->width;
			++_sourceX, ++_otherX)
		{
			const bool _sourceCollision = _source->dataArr[_sourceY][_sourceX];
			const bool _otherCollision = _other->dataArr[_otherY][_otherX];

			if (_sourceCollision & _otherCollision)
				return true;
		}
	}

	return false;
}
