#include "InputHandler.h"
#include "SDL2/include/SDL.h"

InputHandler::InputHandler(SceneHandler* sceneHandler, Renderer* renderer)
	:requestClose{ false },
	sceneHandler(sceneHandler),
	renderer(renderer)
{
}

void InputHandler::Update(float dt)
{
	HandleEvents();
}

bool InputHandler::RequestClose() const
{
	return requestClose;
}

void InputHandler::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
		switch (event.type)
		{
		case SDL_QUIT:
			requestClose = true;
		case SDL_KEYDOWN:;
			if (event.key.keysym.sym == SDLK_ESCAPE)
				requestClose = true;
			else if (event.key.keysym.sym == SDLK_SPACE)
				renderer->ChangeRenderMode();
			else if (event.key.keysym.sym == SDLK_LEFT)
				sceneHandler->ChangeScene(false);
			else if (event.key.keysym.sym == SDLK_RIGHT)
				sceneHandler->ChangeScene(true);
		}
}
