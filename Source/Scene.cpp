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

void Scene::BuildScene()
{
}

