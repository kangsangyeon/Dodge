#include "WorldObject.h"

#include <stdlib.h>
#include "Sprite.h"

WorldObject* WorldObject_Create(Sprite* _sprite, Vector2D _pivot, Vector2D _position)
{
	WorldObject* _outWorldObject = (WorldObject*)malloc(sizeof(WorldObject));
	_outWorldObject->sprite = _sprite;
	_outWorldObject->pivot = _pivot;
	_outWorldObject->position = _position;

	return _outWorldObject;
}

WorldObject* WorldObject_CreateWithSprite(wchar_t* _spriteImageFilePath, Vector2D _pivot, Vector2D _position)
{
	Sprite* _sprite = Sprite_LoadFromImageFile(_spriteImageFilePath);

	return WorldObject_Create(_sprite, _pivot, _position);
}

WorldObject* WorldObject_CreateWithSpriteMask(wchar_t* _spriteImageFilePath, wchar_t* _spriteMaskFilePath, Vector2D _pivot, Vector2D _position)
{
	Sprite* _sprite = Sprite_LoadFromImageAndMaskFiles(_spriteImageFilePath, _spriteMaskFilePath);

	return WorldObject_Create(_sprite, _pivot, _position);
}

void WorldObject_Release(WorldObject* _worldObject)
{
	if (_worldObject == NULL)
		return;

	if (_worldObject->sprite != NULL)
		Sprite_Release(_worldObject->sprite);

	free(_worldObject);
}
