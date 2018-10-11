#pragma once
#include "IShader.h"
#include "Maths.h"
#include "Color.h"

#include "Object.h"

class Matrix4f;

class FlatShading final : public IShader
{
	
public:
	FlatShading(const Matrix4f& M, const Matrix4f& V, const Matrix4f& P, const Matrix4f& S, const Object& object);
	virtual ~FlatShading() override;
	virtual Vector3f VertexShader(const Triangle& triangle, unsigned int index) override;
	virtual bool FragmentShader(Vector3f baryInterpolation, Color& output) override;
private:
	virtual Vector3f Process(const Vector4f& vertex) override;

	//Uniform variables
	const Matrix4f M, V, P, S;
	const Object& object;

	//Varying variables
	Color color[3];
};

class GouraudShading final : public IShader
{

public:
	GouraudShading(const Matrix4f& M, const Matrix4f& V, const Matrix4f& P, const Matrix4f& S, const Object& object);
	virtual ~GouraudShading() override;
	virtual Vector3f VertexShader(const Triangle& triangle, unsigned int index) override;
	virtual bool FragmentShader(Vector3f baryInterpolation, Color& output) override;
private:
	virtual Vector3f Process(const Vector4f& vertex) override;

	//Uniform variables
	const Matrix4f M, V, P, S;
	const Object& object;

	//Varying variables
	Color color[3];
};