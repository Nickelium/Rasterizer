#pragma once
#include <SDL/SDL.h>
#include "Buffer.h"

struct SDL_Window;
struct SDL_Texture;

class Window
{
public:
	Window(int width, int height);
	~Window();

	SDL_Window* GetSDLWindow() const;
	int GetWidth() const;
	int GetHeight() const;

	void SetInfo(float fps, float ms);

	void Update(float dt);
	void SwapBuffer(const Buffer<uint32_t>& buffer);
private:
	bool Init();

	bool InitSDL();
	bool CreateWindowSDL();
	bool CreateSurfaceSDL();

	const char* title;
	int width, height;

	SDL_Window* sdlWindow;
	SDL_Surface* sdlSurface;
};