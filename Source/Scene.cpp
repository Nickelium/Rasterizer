#include "Scene.h"

#include "Maths.h"

Scene::Scene()
{
	BuildScene();
}

std::vector<Object>& Scene::GetObjects()
{
	return objects;
}

Vector3f& Scene::GetLight()
{
	return directionalLight;
}

void Scene::BuildScene()
{
	directionalLight = { 0.0f, 0.0f, -1.0f };
	//Object object("triangle.obj");
	//Object object("icosahedron.obj");
	//Object object("cube.obj");
	Object object("african_head.obj", "african_head_diffuse.tga");
	//Object object("diablo3.obj", "diablo3_pose_diffuse.tga");
	object.SetMaterial(Material{ Vector3f{0.075f}, Vector3f{1.0f}, Vector3f{1.0f}, 0.5f });
	object.SetTransform(Transform(Vector3f(0.0f, 0.0f, -3.0f), Vector3f(0.0f, 45.0f, 0.0f)));
	objects.push_back(std::move(object));
}

