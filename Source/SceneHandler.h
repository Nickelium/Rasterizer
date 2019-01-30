#pragma once
#include <cstdint>
#include <vector>
#include "ObjectFactory.h"


class Scene;

class SceneHandler
{
public:
	SceneHandler();
	~SceneHandler();
	SceneHandler(const SceneHandler&) = delete;
	SceneHandler(SceneHandler&&) = delete;
	SceneHandler& operator=(const SceneHandler&) = delete;
	SceneHandler& operator=(SceneHandler&&) = delete;

	void ChangeScene(bool increaseOrder);
	void Update(float dt);

	Scene* GetActiveScene() const;
private:
	void BuilScenes();

	int activeSceneID;
	std::vector<Scene*> scenes;

	ObjectFactory factory;
};
