#include "SceneHandler.h"
#include "Scene.h"
#include <algorithm>

SceneHandler::SceneHandler()
	: activeSceneID(0)
{
	BuilScenes();
}

SceneHandler::~SceneHandler()
{
	for (auto pScene : scenes)
		delete pScene;
}

void SceneHandler::Update(float dt)
{
	assert(0 <= activeSceneID && activeSceneID < static_cast<int>(scenes.size()) && "Invalid activeSceneID");
	scenes[activeSceneID]->Update(dt);
}

Scene* SceneHandler::GetActiveScene() const
{
	return scenes[activeSceneID];
}

void SceneHandler::BuilScenes()
{
	Scene* sc = new Scene;
	sc->AddObject(factory.CreateTriangle());
	scenes.push_back(sc);

	sc = new Scene;
	sc->AddObject(factory.CreateCube());
	scenes.push_back(sc);

	sc = new Scene;
	sc->AddObject(factory.CreateTeapot());
	scenes.push_back(sc);

	sc = new Scene;
	sc->AddObject(factory.CreateSuzanne());
	scenes.push_back(sc);

	sc = new Scene;
	sc->AddObject(factory.CreateAfrican());
	sc->AddObject(factory.CreateEyesInner());
	sc->AddObject(factory.CreateEyesOuter());
	scenes.push_back(sc);

	sc = new Scene;
	sc->AddObject(factory.CreateDiablo());
	scenes.push_back(sc);
}

void SceneHandler::ChangeScene(bool incrOrder)
{
	if (incrOrder)
		activeSceneID = std::min(activeSceneID + 1, static_cast<int>(scenes.size()) - 1);
	else
		activeSceneID = std::max(activeSceneID - 1, 0);
}