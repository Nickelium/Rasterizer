#include "Shaders.h"
#include "Maths.h"
#include "Triangle.h"
#include "Color.h"

#include <algorithm>

#pragma region FlatShading
void FlatShading::UpdateUniforms(const Matrix4f& M, const Matrix4f& V, const  Matrix4f& P, const Matrix4f& S, const Object& object, const LightParam& lightParam)
{
	this->M = M;
	this->V = V;
	this->P = P;
	this->S = S;
	this->object = &object;
	this->lightParam = lightParam;
}


FlatShading::FlatShading()
	: object(nullptr)
{
}

FlatShading::~FlatShading()
{
}

Vector3f FlatShading::VertexShader(const Triangle& triangle, unsigned int index)
{
	Vector3f lightDir = lightParam.direction;
	Color lightColor = lightParam.diffuseColor;

	Vector3f normal(0.0f,0.0f,0.0f);
	for (int i = 0; i < Triangle::Size(); ++i)
		normal += triangle.GetNormals()[i];
	normal = normal.Normalize();
	
	Matrix4f invTransM =  Matrix4f::Transpose(Matrix4f::Inverse(M));
	Vector3f normalWorld = (invTransM * Vector4f(normal, 0.0f)).GetVec3().Normalize();

	Color ambient = object->GetMaterial().ambient * lightParam.ambientColor;

	float diff = std::max(0.0f, Dot(normalWorld, lightDir));
	Color albedo = object->GetMaterial().diffuse;
	
	Color diffuse = diff * albedo * lightColor;

	color[index] = ambient + diffuse;

	Matrix4f MVP = P * V * M;
	Vector4f vertexNDC = MVP  * Vector4f(triangle[index], 1.0f);
	return Process(vertexNDC);
}

Vector3f FlatShading::Process(const Vector4f& vertex)
{
	return (S * vertex.DivideW()).GetVec3();
}

bool FlatShading::FragmentShader(Vector3f baryInterpolation, float depth, Color& output)
{
	output = Color::Clamp((color[0] + color[1] + color[2]) / 3.0f);
	//baryInterpolation.x * color[0] + baryInterpolation.y * color[1] + baryInterpolation.z * color[2];
	return true;
}
#pragma endregion 


#pragma region GouraudShading

GouraudShading::GouraudShading()
	: object(nullptr)
{
}

void GouraudShading::UpdateUniforms(const Matrix4f& M, const Matrix4f& V, const  Matrix4f& P, const Matrix4f& S, const Object& object, const LightParam& lightParam)
{
	this->M = M;
	this->V = V;
	this->P = P;
	this->S = S;
	this->object = &object;
	this->lightParam = lightParam;
}

GouraudShading::~GouraudShading()
{
}

Vector3f GouraudShading::VertexShader(const Triangle& triangle, unsigned int index)
{
	Vector3f lightDir = lightParam.direction;
	Color lightColor = lightParam.diffuseColor;

	Vector3f normal = triangle.GetNormals()[index];

	Matrix4f invTransM = Matrix4f::Transpose(Matrix4f::Inverse(M));
	Vector3f normalWorld = (invTransM * Vector4f(normal, 0.0f)).GetVec3().Normalize();

	Color ambient = object->GetMaterial().ambient * lightParam.ambientColor;

	float diff = std::max(0.0f, Dot(normalWorld, lightDir));
	Color albedo = object->GetMaterial().diffuse;
	if (useTexture && object->GetDiffuseMap())
		albedo = object->GetDiffuseMap()->GetColor(
			triangle.GetUVs()[index].x, 
			triangle.GetUVs()[index].y);

	Vector3f worldPosition = (M * Vector4f(triangle[index], 1.0f)).GetVec3();
	Vector3f eyePosition = Vector3f(V[3][0], V[3][1], V[3][2]);
	Vector3f eyeDirection = (eyePosition - worldPosition).Normalize();
	Vector3f reflectDirection = -lightDir - 2.0f * Dot(-lightDir, normalWorld) * normalWorld;
	float spec = pow(std::max(0.0f, Dot(-reflectDirection, eyeDirection)), object->GetMaterial().shininess);
	Color specular = object->GetMaterial().specular * spec * lightParam.specularColor;

	Color diffuse = diff * albedo * lightColor;

	color[index] = ambient + diffuse + specular;

	Matrix4f MVP = P * V * M;
	Vector4f vertexNDC = MVP * Vector4f(triangle[index], 1.0f);
	return Process(vertexNDC);
}

Vector3f GouraudShading::Process(const Vector4f& vertex)
{
	return (S * vertex.DivideW()).GetVec3();
}

bool GouraudShading::FragmentShader(Vector3f baryInterpolation, float depth, Color& output)
{
	//Attribute Interpolation
	output =
	Color::Clamp
	(
		depth * 
		(
			baryInterpolation.x * color[0] / depths[0] +
			baryInterpolation.y * color[1] / depths[1] + 
			baryInterpolation.z * color[2] / depths[2]
		)
	);
	return true;
}
#pragma endregion 


#pragma region PhongShading

PhongShading::PhongShading()
	: object(nullptr)
{
}

void PhongShading::UpdateUniforms(const Matrix4f& M, const Matrix4f& V, const  Matrix4f& P, const Matrix4f& S, const Object& object, const LightParam& lightParam) 
{
	this->M = M;
	this->V = V;
	this->P = P;
	this->S = S;
	this->object = &object;
	this->lightParam = lightParam;
}

PhongShading::~PhongShading()
{
}

Vector3f PhongShading::VertexShader(const Triangle& triangle, unsigned int index)
{
	Vector3f normal = triangle.GetNormals()[index];
	Matrix4f invTransM = Matrix4f::Transpose(Matrix4f::Inverse(M));

	Vector3f normalWorld = (invTransM * Vector4f(normal, 0.0f)).GetVec3().Normalize();
	Vector3f positionWorld = (M * Vector4f(triangle[index], 1.0f)).GetVec3();
	normals[index] = normalWorld;
	positions[index] = positionWorld;
	uvs[index] = triangle.GetUVs()[index];


	Matrix4f MVP = P * V * M;
	Vector4f vertexNDC = MVP * Vector4f(triangle[index], 1.0f);
	return Process(vertexNDC);
}

Vector3f PhongShading::Process(const Vector4f& vertex)
{
	return (S * vertex.DivideW()).GetVec3();
}

bool PhongShading::FragmentShader(Vector3f baryInterpolation, float depth, Color& output)
{
	//Attribute Interpolation
	Vector3f position =
	depth *
	(
		baryInterpolation.x * positions[0] / depths[0] +
		baryInterpolation.y * positions[1] / depths[1] +
		baryInterpolation.z * positions[2] / depths[2]
	);

	Vector3f normal = 
	depth *
	(
		baryInterpolation.x * normals[0] / depths[0] +
		baryInterpolation.y * normals[1] / depths[1] +
		baryInterpolation.z * normals[2] / depths[2]
	);
	normal = normal.Normalize();
	Vector2f uv =
	depth *
	(
		baryInterpolation.x * uvs[0] / depths[0] +
		baryInterpolation.y * uvs[1] / depths[1] +
		baryInterpolation.z * uvs[2] / depths[2]
	);

	//Actual Shader
	Vector3f lightDir = lightParam.direction;
	Color lightColor = lightParam.diffuseColor;

	Color ambient = object->GetMaterial().ambient * lightParam.ambientColor;

	float diff = std::max(0.0f, Dot(normal, lightDir));
	Color albedo = object->GetMaterial().diffuse;

	Vector3f eyePosition = Vector3f(V[3][0], V[3][1], V[3][2]);
	Vector3f eyeDirection = (eyePosition - position).Normalize();
	Vector3f reflectDirection = -lightDir + 2.0f * Dot(lightDir, normal) * normal;
	float spec = pow(std::max(0.0f, Dot(-reflectDirection, eyeDirection)), object->GetMaterial().shininess);
	Color specular = spec * lightParam.specularColor;
	if (useTexture && object->GetSpecularMap())
		specular = 0.6f * specular * object->GetSpecularMap()->GetColor(uv.x, uv.y);
	else
		specular = 0.6f * specular * object->GetMaterial().specular;
	Color diffuse = diff * albedo * lightColor;

	output = Color::Clamp(ambient + diffuse + specular);

	return true;
}
#pragma endregion 

#pragma region texturedShading
TexturedShader::TexturedShader()
	: object(nullptr)
{
}

void TexturedShader::UpdateUniforms(const Matrix4f& M, const Matrix4f& V, const  Matrix4f& P, const Matrix4f& S, const Object& object, const LightParam& lightParam)
{
	this->M = M;
	this->V = V;
	this->P = P;
	this->S = S;
	this->object = &object;
	this->lightParam = lightParam;
}

TexturedShader::~TexturedShader()
{
}

Vector3f TexturedShader::VertexShader(const Triangle& triangle, unsigned int index)
{
	Vector3f normal = triangle.GetNormals()[index];
	Matrix4f invTransM = Matrix4f::Transpose(Matrix4f::Inverse(M));

	Vector3f normalWorld = (invTransM * Vector4f(normal, 0.0f)).GetVec3().Normalize();
	Vector3f positionWorld = (M * Vector4f(triangle[index], 1.0f)).GetVec3();
	normals[index] = normalWorld;
	positions[index] = positionWorld;
	uvs[index] = triangle.GetUVs()[index];


	Matrix4f MVP = P * V * M;
	Vector4f vertexNDC = MVP * Vector4f(triangle[index], 1.0f);
	return Process(vertexNDC);
}

Vector3f TexturedShader::Process(const Vector4f& vertex)
{
	return (S * vertex.DivideW()).GetVec3();
}

bool TexturedShader::FragmentShader(Vector3f baryInterpolation, float depth, Color& output)
{
	//Attribute Interpolation
	Vector3f position =
	depth *
	(
		baryInterpolation.x * (positions[0] / depths[0]) +
		baryInterpolation.y * (positions[1] / depths[1]) +
		baryInterpolation.z * (positions[2] / depths[2])
	);

	Vector3f normal =
	depth *
	(
		baryInterpolation.x * (normals[0] / depths[0]) +
		baryInterpolation.y * (normals[1] / depths[1]) +
		baryInterpolation.z * (normals[2] / depths[2])
	);
	normal = normal.Normalize();
	Vector2f uv =
	depth *
	(
		baryInterpolation.x * (uvs[0] / depths[0]) +
		baryInterpolation.y * (uvs[1] / depths[1]) +
		baryInterpolation.z * (uvs[2] / depths[2])
	);											 

	//Vector2f uv =
	//	baryInterpolation.x * uvs[0] +
	//	baryInterpolation.y * uvs[1] +
	//	baryInterpolation.z * uvs[2];

	//Actual Shader
	Vector3f lightDir = lightParam.direction;
	Color lightColor = lightParam.diffuseColor;

	Color ambient = object->GetMaterial().ambient * lightParam.ambientColor;

	float diff = std::max(0.0f, Dot(normal, lightDir));
	Color albedo = object->GetMaterial().diffuse;
	if (object->GetDiffuseMap())
		albedo = object->GetDiffuseMap()->GetColor(uv.x, uv.y);

	Vector3f eyePosition = Vector3f(V[3][0], V[3][1], V[3][2]);
	Vector3f eyeDirection = (eyePosition - position).Normalize();
	Vector3f reflectDirection = -lightDir + 2.0f * Dot(lightDir, normal) * normal;
	float spec = pow(std::max(0.0f, Dot(-reflectDirection, eyeDirection)), object->GetMaterial().shininess);
	Color specular = spec * lightParam.specularColor;
	if (useTexture && object->GetSpecularMap())
		specular = 0.6f * specular * object->GetSpecularMap()->GetColor(uv.x, uv.y);
	else
		specular = 0.6f * specular * object->GetMaterial().specular;
	Color diffuse = diff * albedo * lightColor;

	output = Color::Clamp(ambient + diffuse + specular);

	return true;
}
#pragma endregion 



#pragma region DepthShader
void DepthShader::UpdateUniforms(const Matrix4f& M, const Matrix4f& V, const  Matrix4f& P, const Matrix4f& S, const Object& object, const LightParam& lightParam)
{
	this->M = M;
	this->V = V;
	this->P = P;
	this->S = S;
	this->object = &object;
	this->lightParam = lightParam;
}


DepthShader::DepthShader()
	: object(nullptr)
{
}

DepthShader::~DepthShader()
{
}

Vector3f DepthShader::VertexShader(const Triangle& triangle, unsigned int index)
{
	Matrix4f MVP = P * V * M;
	Vector4f vertexNDC = MVP * Vector4f(triangle[index], 1.0f);
	return Process(vertexNDC);
}

Vector3f DepthShader::Process(const Vector4f& vertex)
{
	return (S * vertex.DivideW()).GetVec3();
}

bool DepthShader::FragmentShader(Vector3f baryInterpolation, float depth, Color& output)
{
	float value = 1.25f * (1.0f - depth);
	value = value * value;
	output = Color::Clamp(Color(value, value, value));
	return true;
}
#pragma endregion 