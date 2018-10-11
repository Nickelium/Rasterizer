#include "Object.h"
#include "Mesh.h"

Object::Object(const char* fileName)
	: Object(fileName, nullptr)
{
}

Object::Object(const char* fileName, const char* textureName)
	: mesh(new Mesh(fileName)),
	texture(textureName ? new Texture(textureName) : nullptr)
{
}

Object::Object(Object&& other)
	: mesh(other.mesh),
	texture(other.texture),
	transform(std::move(other.transform)),
	material(other.material)
{
	other.mesh = nullptr;
	other.texture = nullptr;
}

Object::~Object()
{
	delete mesh;
	delete texture;
}

Mesh* Object::GetMesh() const
{
	return mesh;
}

Texture* Object::GetTexture() const
{
	return texture;
}

Material Object::GetMaterial() const
{
	return material;
}

Transform Object::GetTransform() const
{
	return transform;
}

void Object::SetMaterial(const Material& material)
{
	this->material = material;
}

void Object::SetTransform(const Transform& transform)
{
	this->transform = transform;
}

void Object::Move(const Vector3f& dR)
{
	transform.position += dR;
}

void Object::Rotate(const Vector3f& dR)
{
	transform.rotation += dR;
}

void Object::Scale(const Vector3f& dR)
{
	transform.scale += dR;
}