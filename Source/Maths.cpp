#include "Maths.h"

#include <cassert>
#include <algorithm>



float Maths::Radians(float degrees)
{
	return (degrees / 180.0f) * PI ;
}

float Maths::Degrees(float rad)
{
	return (rad / PI) * 180.0f;
}

float Maths::Clamp(float min, float max, float v)
{
	assert(min < max && "Clamp min < max: failed\n");
	return std::min(std::max(min, v), max);
}

Vector2f Maths::min(const Vector2f& v, const Vector2f& w)
{
	return Vector2f(std::min(v.x, w.x), std::min(v.y, w.y));
}

Vector2f Maths::max(const Vector2f& v, const Vector2f& w)
{
	return Vector2f(std::max(v.x, w.x), std::max(v.y, w.y));
}