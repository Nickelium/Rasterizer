#pragma once
#include <vector>
#include "Object.h"

class Scene
{
public:
	Scene();
	std::vector<Object>& GetObjects();
	Vector3f& GetLight();
private:
	void BuildScene();
	//Camera	
	Vector3f directionalLight;
	std::vector<Object> objects;

};