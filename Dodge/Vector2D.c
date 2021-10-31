#include "Vector2D.h"

#include <math.h>

#include "Const.h"
#include "Helper.h"

const Vector2D Vector2D_Left = {-1, 0};
const Vector2D Vector2D_Right = {1, 0};
const Vector2D Vector2D_Up = {0, 1};
const Vector2D Vector2D_Down = {0, -1};
const Vector2D Vector2D_RightUp = {1, 1};
const Vector2D Vector2D_RightDown = {1, -1};
const Vector2D Vector2D_LeftUp = {-1, 1};
const Vector2D Vector2D_LeftDown = {-1, -1};
const Vector2D Vector2D_Zero = {0, 0};
const Vector2D Vector2D_Center = {.5, .5};
const Vector2D Vector2D_One = {1, 1};

Vector2D Vector2D_Add(Vector2D _first, Vector2D _second)
{
	const Vector2D _outVector2D = {_first.x + _second.x, _first.y + _second.y};
	return _outVector2D;
}

Vector2D Vector2D_Sub(Vector2D _first, Vector2D _second)
{
	const Vector2D _outVector2D = {_first.x - _second.x, _first.y - _second.y};
	return _outVector2D;
}

Vector2D Vector2D_Multiply(Vector2D _vector, double _scalar)
{
	const Vector2D _outVector2D = {_vector.x * _scalar, _vector.y * _scalar};
	return _outVector2D;
}

Vector2D Vector2D_Divide(Vector2D _vector, double _scalar)
{
	const Vector2D _outVector2D = {_vector.x / _scalar, _vector.y / _scalar};
	return _outVector2D;
}

bool Vector2D_IsEquals(Vector2D _first, Vector2D _second)
{
	const bool _isXEquals = fabs(_first.x - _second.x) <= EPSILON;
	const bool _isYEquals = fabs(_first.y - _second.y) <= EPSILON;

	return _isXEquals && _isYEquals;
}

Vector2D Vector2D_Normalized(Vector2D _vector)
{
	if (Vector2D_IsEquals(_vector, Vector2D_Zero) == true)
		return Vector2D_Zero;

	return Vector2D_Divide(_vector, Vector2D_Magnitude(_vector));
}

double Vector2D_Magnitude(Vector2D _vector)
{
	if (Vector2D_IsEquals(_vector, Vector2D_Zero) == true)
		return 0;

	const double _powX = _vector.x * _vector.x;
	const double _powY = _vector.y * _vector.y;
	const double _outMagnitude = sqrt(_powX + _powY);
	return _outMagnitude;
}

double Vector2D_Distance(Vector2D _first, Vector2D _second)
{
	const Vector2D _difference = {
		fabs(fabs(_first.x) - fabs(_second.x)),
		fabs(fabs(_first.y) - fabs(_second.y))
	};

	const float _magnitude = Vector2D_Magnitude(_difference);
	return _magnitude;
}

Vector2D Vector2D_Lerp(Vector2D _origin, Vector2D _destination, float _delta)
{
	const Vector2D _direction = Vector2D_Sub(_destination, _origin);
	const Vector2D _deltaVector = Vector2D_Multiply(_direction, _delta);
	const Vector2D _outVector2D = Vector2D_Add(_origin, _deltaVector);

	return _outVector2D;
}

Vector2D Vector2D_GetPivotAppliedPosition(Vector2D _position, Vector2D _pivot, int _width, int _height)
{
	const double _pivotX = FClamp(_pivot.x, 0, 1);
	const double _pivotY = FClamp(_pivot.y, 0, 1);

	const double _startXWorldPosition = _position.x - _width * _pivotX;
	const double _startYWorldPosition = _position.y - _height * _pivotY;

	const Vector2D _outPosition = {_startXWorldPosition, _startYWorldPosition};
	return _outPosition;
}
