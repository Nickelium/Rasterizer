#pragma once

#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"

struct Material
{
	Vector3f ambient = Vector3f{0.0f};
	Vector3f diffuse = Vector3f{1.0f};
	Vector3f specular = Vector3f{1.0f};
	float shininess = 0.5f;
};

class Object 
{
public:
	Object(const char* fileName);
	Object(const char* fileName, const char* textureName);
	Object(Object&& other);
	~Object();

	Mesh* GetMesh() const;
	Texture* GetTexture() const;
	Material GetMaterial() const;
	Transform GetTransform() const;

	void SetMaterial(const Material& transform);
	void SetTransform(const Transform& transform);

	void Move(const Vector3f& dR);
	void Rotate(const Vector3f& dR);
	void Scale(const Vector3f& dR);
private:
	Mesh* mesh;
	Texture* texture;

	Material material;

	Transform transform;
};