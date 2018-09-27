#include "Window.h"

#include <SDL.h>
#include <cstdio>
#include <string>
#include <iomanip>
#include <sstream>


Window::Window(int width, int height)
	:width(width),
	height(height), 
	title("Software Rasterizer"),
	sdlWindow(nullptr)
{
	Init();
}

Window::~Window()
{
	SDL_DestroyWindow(sdlWindow);
}

SDL_Window* Window::GetSDLWindow() const
{
	return sdlWindow;
}

int Window::GetWidth() const
{
	return width;
}

int Window::GetHeight() const
{
	return height;
}

void Window::SetInfo(float fps, float ms)
{
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << fps;
	std::string  strFps = stream.str();
	stream.str(std::string());
	stream << std::fixed << std::setprecision(2) << ms;
	std::string strMs = stream.str();

	SDL_SetWindowTitle(sdlWindow,  (std::string(title) + " | " + strFps+ "FPS, " +
	strMs + "ms").c_str());
}

void Window::Update()
{
	SDL_UpdateWindowSurface(sdlWindow);
}

void Window::Init()
{
	sdlWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (!sdlWindow)
		fprintf(stderr, "Window could not be created: %s\n", SDL_GetError());
	SDL_GL_SetSwapInterval(1); //Sync update with monitor screen
}