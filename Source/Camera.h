#pragma once

#include "Maths.h"
#include "Transform.h"

class Camera
{
public:
	Camera(const Vector3f& pos = Vector3f(0.0f), const Vector3f& rot = Vector3f(0.0f), const Vector3f& scale = Vector3f(1.0f));
	Camera(Transform transform);

	void SetTransform(const Transform& transform);
	Transform GetTransform() const;
	Matrix4f GetViewMatrix() const;

	void Move(const Vector3f& dR);
private:
	Transform transform;
};

