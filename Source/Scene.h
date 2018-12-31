#pragma once
#include <vector>
#include "Object.h"

class Scene
{
public:
	Scene();
	std::vector<Object>& GetObjects();
	void AddObject(Object& object);
private:
	void BuildScene();
	std::vector<Object> objects;
};