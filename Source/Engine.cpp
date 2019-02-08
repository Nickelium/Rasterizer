#include "Engine.h"

#include "SDL2/include/SDL.h"
#include <glad/glad.h>

#include "Timer.h"
#include "Scene.h"
#include "Camera.h"

#include <chrono>
#include <algorithm>

Engine::Engine(int width, int height)
{
	window =  new Window(width, height);
	sceneHandler = new SceneHandler();
	renderer = new Renderer(window);
	inputHandler = new InputHandler(sceneHandler,  renderer);
}

Engine::~Engine()
{
	delete window;
	delete inputHandler;
	delete sceneHandler;
	delete renderer;
}

void Engine::Update(float dt)
{
	inputHandler->Update(dt);
	window->Update(dt);
	sceneHandler->Update(dt);
}

void Engine::Draw()
{
	renderer->Render(*sceneHandler->GetActiveScene());
}

void Engine::Run()
{
	float time = 0.0f;
	const float updateTime = 0.25f;
	float dt;

	while (!inputHandler->RequestClose())
	{
		timer.Count();
		dt = timer.GetDelta();
		time += timer.GetDelta();

		Update(dt);
		Draw();
		

		if (time > updateTime)
		{
			time -= updateTime;
			window->SetInfo(timer.GetFPS(), timer.GetMS());
		}
	}
}

