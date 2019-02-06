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

void Scene::AddObject(Object& object)
{
	objects.push_back(std::move(object));
}

void Scene::Update(float dt)
{
	for (Object& object : objects)
	{
		//object.SetPosition(Vector3f(3.0f * sin(timer.GetTotal()), 0.0f, 0.0f));
		object.Rotate(Vector3f(0.f, 15.0f * dt, 0.f));
	}
}

Camera& Scene::GetCamera() 
{
	return camera;
}

void Scene::BuildScene()
{
	camera.SetTransform(Transform(Vector3f(0.0f, 0.5f, 0.0f), Vector3f(-10.0f, 0.0f, 0.0f)));	
}

