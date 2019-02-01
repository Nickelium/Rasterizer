#include "ObjectFactory.h"

ObjectFactory::ObjectFactory()
{}

Object ObjectFactory::CreateTriangle() const
{
	Object object("triangle.obj", "container.jpg");
	object.SetMaterial(Material{
		Color{0.75, 0.75f, 0.75f},
		Color{0.75, 0.75f, 0.75f},
		Color{1.0, 1.0f, 1.0f},
		32.0f });
	object.SetTransform(Transform(Vector3f(0.0f, 0.0f, -5.0f), Vector3f(0.0f, 0.0f, 0.0f)));
	return object; // don't move because of RVO
}

Object ObjectFactory::CreateCube() const
{
	Object object("cube2.obj", "container.jpg");
	object.SetMaterial(Material{
		Color{0.75, 0.75f, 0.75f},
		Color{0.75, 0.75f, 0.75f},
		Color{1.0, 1.0f, 1.0f},
		32.0f });
	object.SetTransform(Transform(Vector3f(0.0f, 0.0f, -5.0f), Vector3f(0.0f, 0.0f, 0.0f)));
	return object;
}

Object ObjectFactory::CreateTeapot() const
{
	Object object("teapot.obj", "wood.jpg");
	object.SetMaterial(Material{
		Color{0.75, 0.75f, 0.75f},
		Color{0.75, 0.75f, 0.75f},
		Color{1.0, 1.0f, 1.0f},
		32.0f });
	object.SetTransform(Transform(Vector3f(0.0f, -0.5f, -3.5f), Vector3f(0.0f, 0.0f, 0.0f)));
	return object;
}

Object ObjectFactory::CreateSuzanne() const
{
	Object object("suzanne.obj", "wood.jpg");
	object.SetMaterial(Material{
		Color{0.75, 0.75f, 0.75f},
		Color{0.75, 0.75f, 0.75f},
		Color{1.0, 1.0f, 1.0f},
		32.0f });
	object.SetTransform(Transform(Vector3f(0.0f, 0.0f, -5.5f), Vector3f(0.0f, 0.0f, 0.0f)));
	return object;
}

Object ObjectFactory::CreateAfrican() const
{
	Object object("african_head.obj", "african_head_diffuse.tga", "african_head_spec.tga", "african_head_nm.tga");
	object.SetMaterial(Material{
		Color{0.75, 0.75f, 0.75f},
		Color{0.75, 0.75f, 0.75f},
		Color{1.0, 1.0f, 1.0f},
		32.0f });
	object.SetTransform(Transform(Vector3f(0.0f, 0.0f, -3.75f), Vector3f(0.0f, 0.0f, 0.0f)));
	return object;
}

Object ObjectFactory::CreateEyesInner() const
{
	Object object("african_head_eye_inner.obj", "african_head_eye_inner_diffuse.tga");
	object.SetMaterial(Material{
		Color{0.75, 0.75f, 0.75f},
		Color{0.75, 0.75f, 0.75f},
		Color{1.0, 1.0f, 1.0f},
		32.0f });
	object.SetTransform(Transform(Vector3f(0.0f, 0.0f, -3.75f), Vector3f(0.0f, 0.0f, 0.0f)));
	return object;
}

Object ObjectFactory::CreateEyesOuter() const
{
	Object object("african_head_eye_outer.obj", "african_head_eye_outer_diffuse.tga");
	object.SetMaterial(Material{
		Color{0.75, 0.75f, 0.75f},
		Color{0.75, 0.75f, 0.75f},
		Color{1.0, 1.0f, 1.0f},
		32.0f });
	object.SetTransform(Transform(Vector3f(0.0f, 0.0f, -3.75f), Vector3f(0.0f, 0.0f, 0.0f)));
	return object;
}

Object ObjectFactory::CreateDiablo() const
{
	Object object("diablo3.obj", "diablo3_pose_diffuse.tga", "diablo3_pose_spec.tga");
	object.SetMaterial(Material{
		Color{0.75, 0.75f, 0.75f},
		Color{0.75, 0.75f, 0.75f},
		Color{1.0, 1.0f, 1.0f},
		32.0f });
	object.SetTransform(Transform(Vector3f(0.0f, 0.0f, -4.0f), Vector3f(0.0f, 0.0f, 0.0f)));
	return object;
}

Object ObjectFactory::CreateBMax() const
{
	Object object("Bigmax_White_OBJ.obj", "container.jpg");
	object.SetMaterial(Material{
		Color{0.75, 0.75f, 0.75f},
		Color{0.75, 0.75f, 0.75f},
		Color{1.0, 1.0f, 1.0f},
		32.0f });
	object.SetTransform(Transform(Vector3f(0.0f, -35.0f, -200.0f), Vector3f(0.0f, 0.0f, 0.0f)));
	return object;
}

Object ObjectFactory::CreateChest() const
{
	Object object("chest_mesh.obj", "container.jpg");
	object.SetMaterial(Material{
		Color{0.75, 0.75f, 0.75f},
		Color{0.75, 0.75f, 0.75f},
		Color{1.0, 1.0f, 1.0f},
		32.0f });
	object.SetTransform(Transform(Vector3f(0.0f, -0.0f, -2.0f), Vector3f(0.0f, 0.0f, 0.0f)));
	return object;
}