#include "Application.h"

#include <SDL.h>
#include <cstdio>

#include "Timer.h"
#include "Scene.h"
#include "Camera.h"

#include <chrono>

Application::Application(int width, int height)
	:requestClose(false)
{
	InitSDL();
	window =  new Window(width, height);
	renderer = new Renderer(*window);
}

Application::~Application()
{
	delete renderer;
	delete window;
	SDL_Quit();
}

void Application::Update(Scene& scene, Camera& camera)
{
	for (Object& object : scene.GetObjects())
	{
		//object.Move(Vector3f(0.001f));
		object.Rotate(Vector3f(0.f, 0.75f, 0.f));
		camera.Move(Vector3f(0.0f, 0.0f, +0.005f));
	}
}

void Application::Run()
{
	Timer timer;
	float time = 0.0f;
	const float updateTime = 0.25f;
	
	Scene scene;
	Camera camera;

	while (!requestClose)
	{
		timer.Count();
		time += timer.GetDelta();
		HandleEvents();

		Update(scene, camera);
		renderer->Render(scene, camera);

		window->Update();
		if (time > updateTime)
		{
			time -= updateTime;
			window->SetInfo(timer.GetFPS(), timer.GetMS());
		}
		//SDL_Delay(1);
	}
}

void Application::InitSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		fprintf(stderr, "SDL Init error: %s\n", SDL_GetError());
}

void Application::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
		switch (event.type)
		{
		case SDL_QUIT:
			requestClose = true;
		case SDL_KEYDOWN: 
			if (event.key.keysym.sym == SDLK_ESCAPE)
				requestClose = true;
		}
}