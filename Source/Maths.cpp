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
	assert(min < max && "Clamp Min < Max: failed\n");
	return std::min(std::max(min, v), max);
}

Vector2f Maths::Min(const Vector2f& v, const Vector2f& w)
{
	return Vector2f(std::min(v.x, w.x), std::min(v.y, w.y));
}

Vector2f Maths::Max(const Vector2f& v, const Vector2f& w)
{
	return Vector2f(std::max(v.x, w.x), std::max(v.y, w.y));
}

void Maths::Sort(Vector3f* vertices, int size)
{
	size_t nbVertices = size, k = 0; float minY;
	for (size_t i = 0; i < nbVertices; ++i)
	{
		k = i;
		minY = vertices[i].y;
		for (size_t j = i + 1; j < nbVertices; ++j)
			if (minY > vertices[j].y)
			{
				minY = vertices[j].y;
				k = j;
			}
		std::swap(vertices[k], vertices[i]);
	}
}

