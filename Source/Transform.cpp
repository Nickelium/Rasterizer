#include "Transform.h"



Transform::Transform(const Vector3f& pos, const Vector3f& rot, const Vector3f& sc)
	:position(pos),
	rotation(rot),
	scale(sc)
{
}

Matrix4f Transform::GetMatrix() const
{
	return 
		Matrix4f::Translate(position) * 
		Matrix4f::Rotate(Vector3f(1.0f, 0.0f, 0.0f), rotation.x) * 
		Matrix4f::Rotate(Vector3f(0.0f, 1.0f, 0.0f), rotation.y) *
		Matrix4f::Rotate(Vector3f(0.0f, 0.0f, 1.0f), rotation.z)*
		Matrix4f::Scale(scale) ;
}