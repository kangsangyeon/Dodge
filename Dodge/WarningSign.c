#include "WarningSign.h"

#include <stdlib.h>


WarningSign* WarningSign_Create(wchar_t* _spriteFilePath, Vector2D _pivot, Vector2D _position, float _holdWarningTime)
{
	WarningSign* _warningSign = (WarningSign*)malloc(sizeof(WarningSign));
	_warningSign->worldObject = WorldObject_CreateWithSprite(_spriteFilePath, _spriteFilePath, _pivot, _position);
	_warningSign->holdWarningTime = _holdWarningTime;
	return _warningSign;
}

void WarningSign_Release(WarningSign* _warningSign)
{
	if (_warningSign == NULL)
		return;

	if (_warningSign->worldObject != NULL)
		WorldObject_Release(_warningSign->worldObject);

	free(_warningSign);
}

void WarningSign_Destroy(WarningSign* _warningSign)
{
	if (_warningSign == NULL)
		return;
	// float _destroyWarningTime = _warningSign->holdWarningTime;
	// 시간
}
