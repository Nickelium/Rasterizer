#pragma once

#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"

class Object 
{
public:
	Object(const char* fileName);
	Object(const char* fileName, const char* textureName);
	Object(Object&& other);
	~Object();

	Mesh* GetMesh() const;
	Texture* GetTexture() const;
	Transform GetTransform() const;

	void SetTransform(const Transform& transform);

	void Move(const Vector3f& dR);
	void Rotate(const Vector3f& dR);
	void Scale(const Vector3f& dR);
private:
	Mesh* mesh;
	Texture* texture;

	Transform transform;
};