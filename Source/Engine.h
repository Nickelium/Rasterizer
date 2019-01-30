#pragma once

#include "Window.h"
#include "Renderer.h"
#include "Timer.h"
#include "ObjectFactory.h"
#include "InputHandler.h"
#include "SceneHandler.h"

class Camera;

class Engine
{
public:
	Engine(int width, int height);
	~Engine();
	void Run();
private:
	void Update(float dt);
	void Draw();

	Window* window;
	InputHandler* inputHandler;
	SceneHandler* sceneHandler;
	Renderer* renderer;

	Timer timer;
};