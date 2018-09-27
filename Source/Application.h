#pragma once

#include "Window.h"
#include "Renderer.h"

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

	bool requestClose;

	Window* window;
	Renderer* renderer;
};