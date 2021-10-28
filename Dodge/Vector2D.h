#pragma once

#include <stdbool.h>

typedef struct TVector2D
{
	double x;
	double y;
} Vector2D;

extern const Vector2D Vector2D_Left;
extern const Vector2D Vector2D_Right;
extern const Vector2D Vector2D_Up;
extern const Vector2D Vector2D_Down;
extern const Vector2D Vector2D_RightUp;
extern const Vector2D Vector2D_RightDown;
extern const Vector2D Vector2D_LeftUp;
extern const Vector2D Vector2D_LeftDown;
extern const Vector2D Vector2D_Zero;
extern const Vector2D Vector2D_Center;
extern const Vector2D Vector2D_One;

Vector2D Vector2D_Add(Vector2D _first, Vector2D _second);

Vector2D Vector2D_Sub(Vector2D _first, Vector2D _second);

Vector2D Vector2D_Multiply(Vector2D _vector, double _scalar);

Vector2D Vector2D_Divide(Vector2D _vector, double _scalar);

bool Vector2D_IsEquals(Vector2D _first, Vector2D _second);

Vector2D Vector2D_Normalized(Vector2D _vector);

double Vector2D_Magnitude(Vector2D _vector);

double Vector2D_Distance(Vector2D _first, Vector2D _second);

Vector2D Vector2D_Lerp(Vector2D _origin, Vector2D _destination, float _delta);
