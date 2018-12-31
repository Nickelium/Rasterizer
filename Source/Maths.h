#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3.h"
#include "Matrix4.h"

#define PI 3.14159265359f

namespace Maths
{
	float Radians(float degrees);
	float Degrees(float rad);
	float Clamp(float min, float max, float v);
	
	Vector2f Min(const Vector2f& v, const Vector2f& w);
	Vector2f Max(const Vector2f& v, const Vector2f& w); //lower case conflicts name
}	