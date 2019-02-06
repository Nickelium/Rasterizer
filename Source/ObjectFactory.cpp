#include "ObjectFactory.h"

ObjectFactory::ObjectFactory()
{}

Object ObjectFactory::CreateTriangle() const
{
	Object object("Triangle/triangle.obj", "Triangle/checker.jpg");
	object.SetMaterial(Material{
		Color{0.75, 0.75f, 0.75f},
		Color{0.75, 0.75f, 0.75f},
		Color{1.0, 1.0f, 1.0f},
		10.0f });
	object.SetTransform(Transform(Vector3f(0.0f, -0.1f, -4.0f), Vector3f(0.0f, 0.0f, 45.0f)));
	return object; // don't move because of RVO
}

Object ObjectFactory::CreateCube() const
{
	Object object("Cube/cube.obj", "Cube/container.jpg");
	object.SetMaterial(Material{
		Color{0.75, 0.75f, 0.75f},
		Color{0.75, 0.75f, 0.75f},
		Color{0.70f, 0.70f, 0.70f},
		22.0f });
	object.SetTransform(Transform(Vector3f(0.0f, -0.1f, -4.0f), Vector3f(0.0f, 0.0f, 0.0f)));
	return object;
}

Object ObjectFactory::CreateTeapot() const
{
	Object object("Utah/teapot.obj", "Utah/wood.jpg");
	object.SetMaterial(Material{
		Color{0.75, 0.75f, 0.75f},
		Color{0.75, 0.75f, 0.75f},
		Color{1.0, 1.0f, 1.0f},
		50.0f });
	object.SetTransform(Transform(Vector3f(0.0f, -0.5f, -3.5f), Vector3f(0.0f, 0.0f, 0.0f)));
	return object;
}

Object ObjectFactory::CreateSuzanne() const
{
	Object object("Suzanne/suzanne.obj", "Suzanne/clay.jpg");
	object.SetMaterial(Material{
		Color{0.75, 0.75f, 0.75f},
		Color{0.75, 0.75f, 0.75f},
		Color{0.25, 0.25f, 0.25f},
		50.0f });
	object.SetTransform(Transform(Vector3f(0.0f, -0.25f, -4.5f), Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.85f, 0.85f, 0.85f)));
	return object;
}

Object ObjectFactory::CreateAfrican() const
{
	Object object("African/african_head.obj", "African/african_head_diffuse.tga");
	object.SetMaterial(Material{
		Color{0.75, 0.75f, 0.75f},
		Color{0.75, 0.75f, 0.75f},
		Color{0.1f, 0.1f, 0.1f},
		32.0f });
	object.SetTransform(Transform(Vector3f(0.0f, -0.15f, -3.75f), Vector3f(0.0f, 0.0f, 0.0f)));
	return object;
}

Object ObjectFactory::CreateEyesInner() const
{
	Object object("African/african_head_eye_inner.obj", "African/african_head_eye_inner_diffuse.tga");
	object.SetMaterial(Material{
		Color{0.75, 0.75f, 0.75f},
		Color{0.75, 0.75f, 0.75f},
		Color{1.0, 1.0f, 1.0f},
		32.0f });
	object.SetTransform(Transform(Vector3f(0.0f, -0.15f, -3.75f), Vector3f(0.0f, 0.0f, 0.0f)));
	return object;
}

Object ObjectFactory::CreateEyesOuter() const
{
	Object object("African/african_head_eye_outer.obj", "African/african_head_eye_outer_diffuse.tga");
	object.SetMaterial(Material{
		Color{0.75, 0.75f, 0.75f},
		Color{0.75, 0.75f, 0.75f},
		Color{1.0, 1.0f, 1.0f},
		32.0f });
	object.SetTransform(Transform(Vector3f(0.0f, -0.15f, -3.75f), Vector3f(0.0f, 0.0f, 0.0f)));
	return object;
}

Object ObjectFactory::CreateDiablo() const
{
	Object object("Diablo/diablo3.obj", "Diablo/diablo3_pose_diffuse.tga");
	object.SetMaterial(Material{
		Color{0.75, 0.75f, 0.75f},
		Color{0.75, 0.75f, 0.75f},
		Color{0.2f, 0.2f, 0.2f},
		32.0f });
	object.SetTransform(Transform(Vector3f(0.0f, -0.15f, -3.75f), Vector3f(0.0f, 0.0f, 0.0f)));
	return object;
}

Object ObjectFactory::CreateHelmet() const
{
	Object object("Helmet/helmet.obj", "Helmet/helmet_diffuse.png");
	object.SetMaterial(Material{
		Color{0.75, 0.75f, 0.75f},
		Color{1.0, 1.0f, 1.0f},
		Color{0.25f, 0.75f, 0.75f},
		32.0f });
	object.SetTransform(Transform(Vector3f(0.0f, -0.0f, -4.5f), Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.80f, 0.80f, 0.80f)));
	object.SetBackFaceCulling(false);
	return object;
}

Object ObjectFactory::CreateIllidan() const
{
	Object object("Illidan/illidan.obj", "Illidan/illidan_diffuse.jpg");
	object.SetMaterial(Material{
		Color{0.75, 0.75f, 0.75f},
		Color{0.75, 0.75f, 0.75f},
		Color{0.25f, 0.25f, 0.25f},
		12.0f });
	object.SetTransform(Transform(Vector3f(0.0f, -0.25f, -4.5f), Vector3f(0.0f, 0.0f, 0.0f)));
	return object;
}

Object ObjectFactory::CreateCar() const
{
	Object object("Car/car.obj", "Car/skylineColor.png");
	object.SetMaterial(Material{
		Color{0.95f, 0.95f, 0.95f},
		Color{0.75f, 0.75f, 0.75f},
		Color{0.4f, 0.4f, 0.4f},
		3.0f });
	object.SetTransform(Transform(Vector3f(0.0f, -0.35f, -5.0f), Vector3f(0.0f, 0.0f, 0.0f), Vector3f(1.f, 1.f, 1.f)));
	return object;
}

Object ObjectFactory::CreateGuitar() const
{
	Object object("Guitar/guitar.obj", "Guitar/guitar_diffuse.png");
	object.SetMaterial(Material{
		Color{0.75, 0.75f, 0.75f},
		Color{0.75, 0.75f, 0.75f},
		Color{0.8f, 0.75f, 0.75f},
		32.0f });
	object.SetTransform(Transform(Vector3f(0.0f, -0.25f, -4.5f), Vector3f(0.0f, 0.0f, 0.0f), Vector3f(1.75f, 1.75f, 1.75f)));
	return object;
}

Object ObjectFactory::CreateWeapon() const
{
	Object object("Weapon/weapon.obj", "Weapon/weapon_diffuse.jpg");
	object.SetMaterial(Material{
		Color{0.85f, 0.75f, 0.75f},
		Color{0.75, 0.75f, 0.75f},
		Color{1.0, 251.0f / 255.0f, 245.0f / 255.0f},
		50.0f });
	object.SetTransform(Transform(Vector3f(0.0f, -0.3f, -5.5f), Vector3f(5.0f, 0.0f, 45.0f)));
	return object;
}