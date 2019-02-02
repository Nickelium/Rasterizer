#pragma once
#include "Maths.h"
#include <iostream>

struct LightParam;
class Object;
struct Color;
struct Triangle;

class IShader
{
public:
	virtual ~IShader() {}
	virtual Vector3f VertexShader(const Triangle& triangle, unsigned int index) = 0;
	virtual bool FragmentShader(Vector3f baryInterpolation, float depth, Color& output) = 0;

	virtual void UpdateUniforms(const Matrix4f& M, const Matrix4f& V, const Matrix4f& P, const Matrix4f& S, const Object& object, const LightParam& lightParam) = 0;

	inline void StoreDepthValues(float* depths)
	{
		for (int i = 0; i < 3; ++i)
			this->depths[i] = depths[i];
	}
protected:
	virtual Vector3f Process(const Vector4f& vertex) = 0;
	float depths[3];
};
