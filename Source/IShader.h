#pragma once
#include "Maths.h"
#include <iostream>

struct Color;
struct Triangle;

class IShader
{
public:
	virtual ~IShader() {}
	virtual Vector3f VertexShader(const Triangle& triangle, unsigned int index) = 0;
	virtual bool FragmentShader(Vector3f baryInterpolation, Color& output) = 0;
private:
	virtual Vector3f Process(const Vector4f& vertex) = 0;
};
