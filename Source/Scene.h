#pragma once
#include <vector>
#include "Object.h"
#include "Camera.h"

class Scene
{
public:
	Scene();
	std::vector<Object>& GetObjects();
	void AddObject(Object& object);
	void Update(float dt);

	Camera& GetCamera();
private:
	void BuildScene();
	std::vector<Object> objects;

	Camera camera;
};
