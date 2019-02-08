#pragma once
#include <cstdint>
#include <vector>
#include "SceneParser.h"


class Scene;

/*
 * Class SceneHandler
 * 
 * - Provides communication with the scenes it contains
 * - Will create specific scenes
 * TODO: Scenes are still hard coded 
 */
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

	SceneParser parser;
};
