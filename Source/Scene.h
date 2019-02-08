#pragma once
#include <vector>
#include "Object.h"
#include "Camera.h"

/*
 * Class Scene
 * 
 * - Contains all objects in the scene
 * - Contains a camera
 */
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
