#pragma once

#include "Maths.h"

struct Transform
{
public:
	Transform(const Vector3f& pos = Vector3f(0.0f), const Vector3f& rot = Vector3f(0.0f), const Vector3f& sc = Vector3f(1.0f));
	Matrix4f GetMatrix() const;

	Vector3f position;
	Vector3f rotation;
	Vector3f scale;
};

