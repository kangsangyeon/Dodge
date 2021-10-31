#include "WorldObject.h"

#include <stdlib.h>

#include "Sprite.h"

WorldObject* WorldObject_Create(Sprite* _sprite, Collider* _collider, Vector2D _pivot, Vector2D _position)
{
	WorldObject* _outWorldObject = (WorldObject*)malloc(sizeof(WorldObject));
	_outWorldObject->sprite = _sprite;
	_outWorldObject->collider = _collider;
	_outWorldObject->pivot = _pivot;
	_outWorldObject->position = _position;

	return _outWorldObject;
}

WorldObject* WorldObject_CreateWithSprite(wchar_t* _spriteImageFilePath, wchar_t* _colliderFilePath, Vector2D _pivot, Vector2D _position)
{
	Sprite* _sprite = Sprite_LoadFromImageFile(_spriteImageFilePath);
	Collider* _collider = Collider_LoadFromTextFile(_colliderFilePath);

	return WorldObject_Create(_sprite, _collider, _pivot, _position);
}

WorldObject* WorldObject_CreateWithSpriteMask(wchar_t* _spriteImageFilePath, wchar_t* _spriteMaskFilePath, wchar_t* _colliderFilePath, Vector2D _pivot, Vector2D _position)
{
	Sprite* _sprite = Sprite_LoadFromImageAndMaskFiles(_spriteImageFilePath, _spriteMaskFilePath);
	Collider* _collider = Collider_LoadFromTextFile(_colliderFilePath);

	return WorldObject_Create(_sprite, _collider, _pivot, _position);
}

void WorldObject_Release(WorldObject* _worldObject)
{
	if (_worldObject == NULL)
		return;

	if (_worldObject->sprite != NULL)
		Sprite_Release(_worldObject->sprite);

	free(_worldObject);
}
