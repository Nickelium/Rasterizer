#include "Camera.h"
#include "Maths.h"
#include "Transform.h"

Camera::Camera(const Vector3f& pos, const Vector3f& rot, const Vector3f& scale)
	: transform(pos, rot, scale)
{
}

Camera::Camera(Transform transform)
	:transform(transform)
{
}

void Camera::SetTransform(const Transform& transform)
{
	this->transform = transform;
}

Transform Camera::GetTransform() const
{
	return transform;
}

Matrix4f Camera::GetViewMatrix() const
{
	//(TRS)^-1 = S^-1 * R^-1 * T^-1
	//S(1/sc) * Transpos(R) * T(-v)
	return Matrix4f::Inverse(transform.GetMatrix());
}

void Camera::Move(const Vector3f& dR)
{
	transform.position += dR;
}