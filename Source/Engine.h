#pragma once

#include "Window.h"
#include "Renderer.h"
#include "Timer.h"
#include "InputHandler.h"
#include "SceneHandler.h"

class Camera;

/*
 * Class Engine
 * 
 * - Main class binds the subcomponents together
 * - Defer task to subcomponents
 */
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