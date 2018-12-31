#pragma once
#include "IShader.h"
#include "Maths.h"
#include "Color.h"

#include "Object.h"

class Matrix4f;

struct LightParam
{
	Vector3f direction;

	Color ambientColor = {0.2f, 0.2f, 0.2f};
	Color diffuseColor = {0.5f, 0.5f, 0.5f};
	Color specularColor = {1.0f, 1.0f, 1.0f};
};


class FlatShading final : public IShader
{
	
public:
	FlatShading();
	virtual ~FlatShading() override;
	virtual Vector3f VertexShader(const Triangle& triangle, unsigned int index) override;
	virtual bool FragmentShader(Vector3f baryInterpolation, Color& output) override;

	virtual void UpdateUniforms(const Matrix4f& M, const Matrix4f& V, const Matrix4f& P, const Matrix4f& S, const Object& object, const LightParam& lightParam, bool useTexture = true) override;

private:
	virtual Vector3f Process(const Vector4f& vertex) override;

	//Uniform variables
	Matrix4f M, V, P, S;
	const Object* object;

	LightParam lightParam;
	bool useTexture;

	//Varying variables
	Color color[3];
};

class GouraudShading final : public IShader
{

public:
	GouraudShading();
	virtual ~GouraudShading() override;
	virtual Vector3f VertexShader(const Triangle& triangle, unsigned int index) override;
	virtual bool FragmentShader(Vector3f baryInterpolation, Color& output) override;

	virtual void UpdateUniforms(const Matrix4f& M, const Matrix4f& V, const Matrix4f& P, const Matrix4f& S, const Object& object, const LightParam& lightParam, bool useTexture = true) override;
private:
	virtual Vector3f Process(const Vector4f& vertex) override;

	//Uniform variables
	Matrix4f M, V, P, S;
	const Object* object;

	LightParam lightParam;
	bool useTexture;

	//Varying variables
	Color color[3];
};

class PhongShading final : public IShader
{

public:
	PhongShading();
	virtual ~PhongShading() override;
	virtual Vector3f VertexShader(const Triangle& triangle, unsigned int index) override;
	virtual bool FragmentShader(Vector3f baryInterpolation, Color& output) override;

	virtual void UpdateUniforms(const Matrix4f& M, const Matrix4f& V, const Matrix4f& P, const Matrix4f& S, const Object& object, const LightParam& lightParam, bool useTexture = true) override;
private:
	virtual Vector3f Process(const Vector4f& vertex) override;

	//Uniform variables
	Matrix4f M, V, P, S;
	const Object* object;

	LightParam lightParam;
	bool useTexture;

	//Varying variables
	Vector3f positions[3];
	Vector3f normals[3];
	Vector2f uvs[3];
};