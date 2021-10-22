#include "WorldObject.h"

#include <stdlib.h>
#include "Sprite.h"

WorldObject* WorldObject_Create(Sprite* _sprite, Vector2D _position)
{
	WorldObject* _outWorldObject = (WorldObject*)malloc(sizeof(WorldObject));
	_outWorldObject->sprite = _sprite;
	_outWorldObject->position = _position;

	return _outWorldObject;
}

WorldObject* WorldObject_Create(wchar_t* _spriteTextFilePath, Vector2D _position)
{
	Sprite* _sprite = Sprite_LoadFromTextFile(_spriteTextFilePath);

	return WorldObject_Create(_sprite, _position);
}

void WorldObject_Release(WorldObject* _worldObject)
{
	if (_worldObject == NULL)
		return;

	if (_worldObject->sprite != NULL)
		Sprite_Release(_worldObject->sprite);

	free(_worldObject);
}
