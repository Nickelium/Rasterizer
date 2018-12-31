#include "Application.h"

#include <SDL/SDL.h>
#include <glad/glad.h>

#include <cstdio>

#include "Timer.h"
#include "Scene.h"
#include "Camera.h"

#include <chrono>
#include <algorithm>

Application::Application(int width, int height)
	:requestClose(false),
	index(0u)
{
	InitSDL();
	window =  new Window(width, height);
	renderer = new Renderer(*window);

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

	activeScene = scenes[index];
}

Application::~Application()
{
	for (auto& sc : scenes)
		delete sc;

	delete renderer;
	delete window;
	SDL_Quit();
}

void Application::Update(Scene& scene, Camera& camera)
{
	for (Object& object : scene.GetObjects())
	{
		//object.SetPosition(Vector3f(3.0f * sin(timer.GetTotal()), 0.0f, 0.0f));
		object.Rotate(Vector3f(0.f, 45.0f * timer.GetDelta(), 0.f));
		//camera.Move(Vector3f(0.0f, 0.0f, +0.0005f));
	}
}

void Application::ChangeScene(bool incrOrder)
{
	if (incrOrder)
		index = std::min(index + 1, scenes.size() - 1);
	else
		index = std::max((signed)index - 1, 0);
	activeScene = scenes[index];
}

void Application::Run()
{
	float time = 0.0f;
	const float updateTime = 0.25f;

	Camera camera;

	camera.Move(Vector3f(0.0f, 0.0f, 4.5f));
	while (!requestClose)
	{
		timer.Count();
		time += timer.GetDelta();
		HandleEvents();

		Update(*activeScene, camera);

		renderer->Render(*activeScene, camera);
		//gui
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
			else if (event.key.keysym.sym == SDLK_UP)
				renderer->ChangeShader();
			else if (event.key.keysym.sym == SDLK_DOWN)
				renderer->ChangeShader(true);
			else if (event.key.keysym.sym == SDLK_SPACE)
				renderer->ChangeRenderMode();
			else if (event.key.keysym.sym == SDLK_LEFT)
				ChangeScene(false);
			else if (event.key.keysym.sym == SDLK_RIGHT)
				ChangeScene(true);
		}
}