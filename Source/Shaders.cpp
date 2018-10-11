#include "Shaders.h"
#include "Maths.h"
#include "Triangle.h"
#include "Color.h"

#include <algorithm>

#pragma region FlatShading
FlatShading::FlatShading(const Matrix4f& M, const Matrix4f& V, const Matrix4f& P, const Matrix4f& S, const Object& object)
	:M(M),
	V(V),
	P(P),
	S(S),
	object(object)
{
}

FlatShading::~FlatShading()
{
}

Vector3f FlatShading::VertexShader(const Triangle& triangle, unsigned int index)
{
	Vector3f lightDir = { 0.0f , 0.0f, 1.0f};
	float invMaxValue = 1.0f / 255.0f;
	Color lightColor = { 255.0f * invMaxValue , 255 * invMaxValue, 255 * invMaxValue};

	Vector3f normal;
	for (int i = 0; i < Triangle::Size(); ++i)
		normal += triangle.GetNormals()[i];
	normal = normal.Normalize();
	
	Matrix4f invTransM = Matrix4f::Transpose(Matrix4f::Inverse(M));
	Vector3f normalWorld = (invTransM * Vector4f(normal, 0.0f)).GetVec3();

	Color ambient = object.GetMaterial().ambient * lightColor;

	float diff = std::max(0.0f, Dot(normalWorld, lightDir));
	Color fullColor = Color(1.0f, 1.0f, 1.0f);
	if (object.GetTexture())
	{
		Vector2f UV;
		for (int i = 0; i < Triangle::Size(); ++i)
			UV += triangle.GetUVs()[i];
		UV /= (float) Triangle::Size();
		fullColor = object.GetTexture()->GetColor(UV.x, UV.y);
	}
	Color diffuse = diff * fullColor * lightColor;

	color[index] = ambient + diffuse;

	Matrix4f MVP = P * V * M;
	Vector4f vertexNDC = MVP  * Vector4f(triangle[index], 1.0f);
	return Process(vertexNDC);
}

Vector3f FlatShading::Process(const Vector4f& vertex)
{
	return (S * vertex.DivideW()).GetVec3();
}

bool FlatShading::FragmentShader(Vector3f baryInterpolation, Color& output)
{
	output = baryInterpolation.x * color[0] + baryInterpolation.y * color[1] + baryInterpolation.z * color[2];
	return true;
}
#pragma endregion 


#pragma region GouraudShading
GouraudShading::GouraudShading(const Matrix4f& M, const Matrix4f& V, const Matrix4f& P, const Matrix4f& S, const Object& object)
	:M(M),
	V(V),
	P(P),
	S(S),
	object(object)
{
}

GouraudShading::~GouraudShading()
{
}

Vector3f GouraudShading::VertexShader(const Triangle& triangle, unsigned int index)
{
	Vector3f lightDir = { 0.0f , 0.0f, 1.0f };
	float invMaxValue = 1.0f / 255.0f;
	Color lightColor = { 255.0f * invMaxValue , 255 * invMaxValue, 255 * invMaxValue };

	Vector3f normal = triangle.GetNormals()[index];

	Matrix4f invTransM = Matrix4f::Transpose(Matrix4f::Inverse(M));
	Vector3f normalWorld = (invTransM * Vector4f(normal, 0.0f)).GetVec3();

	Color ambient = object.GetMaterial().ambient * lightColor;

	float diff = std::max(0.0f, Dot(normal, lightDir));
	Color fullColor = Color(1.0f, 1.0f, 1.0f);
	if (object.GetTexture())
	{
		Vector2f UV;
		for (int i = 0; i < Triangle::Size(); ++i)
			UV += triangle.GetUVs()[i];
		UV /= (float)Triangle::Size();
		fullColor = object.GetTexture()->GetColor(UV.x, UV.y);
	}
	Color diffuse = diff * fullColor * lightColor;

	color[index] = ambient + diffuse;

	Matrix4f MVP = P * V * M;
	Vector4f vertexNDC = MVP * Vector4f(triangle[index], 1.0f);
	return Process(vertexNDC);
}

Vector3f GouraudShading::Process(const Vector4f& vertex)
{
	return (S * vertex.DivideW()).GetVec3();
}

bool GouraudShading::FragmentShader(Vector3f baryInterpolation, Color& output)
{
	output = baryInterpolation.x * color[0] + baryInterpolation.y * color[1] + baryInterpolation.z * color[2];
	return true;
}
#pragma endregion 