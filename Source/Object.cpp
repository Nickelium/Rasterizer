#include "Object.h"
#include "Mesh.h"


Object::Object(const char* fileName,
	const char* textureName,
	const char* specularMap,
	const char* normalMap)
	: mesh(new Mesh(fileName)),
	texture(textureName ? new Texture(textureName) : nullptr),
	specularMap(specularMap ? new Texture(specularMap) : nullptr),
	normalMap(normalMap ? new Texture(normalMap) : nullptr),
	backFaceCulling(true)
{
}

Object::Object(Object&& other) noexcept
	: mesh(other.mesh),
	texture(other.texture),
	specularMap(other.specularMap),
	normalMap(other.normalMap),
	transform(std::move(other.transform)),
	material(other.material)
{
	other.mesh = nullptr;
	other.texture = nullptr;
	other.specularMap = nullptr;
	other.normalMap = nullptr;
}

Object::~Object()
{
	delete mesh;
	delete texture;
	delete specularMap;
	delete normalMap;
}

Mesh* Object::GetMesh() const
{
	return mesh;
}

Texture* Object::GetDiffuseMap() const
{
	return texture;
}

Texture* Object::GetSpecularMap() const
{
	return specularMap;
}

Texture* Object::GetNormalMap() const
{
	return normalMap;
}

Material Object::GetMaterial() const
{
	return material;
}

Transform Object::GetTransform() const
{
	return transform;
}

bool Object::BackFaceCulling() const
{
	return backFaceCulling;
}

void Object::SetMaterial(const Material& material)
{
	this->material = material;
}

void Object::SetTransform(const Transform& transform)
{
	this->transform = transform;
}

void Object::SetPosition(const Vector3f& pos)
{
	transform.position = pos;
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

void Object::SetBackFaceCulling(bool backfaceCulling)
{
	this->backFaceCulling = backfaceCulling;
}
