#pragma once


#include "WorldObject.h"

typedef struct TWarningSign
{
	WorldObject* worldObject;
	float holdWarningTime;
	Vector2D createPosition;
} WarningSign;

WarningSign* WarningSign_Create(wchar_t* _spriteFilePath, Vector2D _pivot, Vector2D _position, float _holdWarningTime);

void WarningSign_Release(WarningSign* _warningSign);

void WarningSign_Destroy(WarningSign* _warningSign);
