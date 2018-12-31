#pragma once

#include "Window.h"
#include "Renderer.h"
#include "Timer.h"
#include "ObjectFactory.h"

class Camera;

class Application
{
public:
	Application(int width, int height);
	~Application();
	void Run();
private:
	void InitSDL();
	void HandleEvents();

	void Update(Scene& scene, Camera& camera);

	void ChangeScene(bool incrOrder);

	bool requestClose;

	Window* window;
	Renderer* renderer;

	std::vector<Scene*> scenes;
	Scene* activeScene;
	std::vector<Scene*>::size_type index;

	ObjectFactory factory;

	Timer timer;
};