#include "Window.h"

#include <SDL\SDL.h>
#include <glad/glad.h>

#include <cstdio>
#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2test.lib")
#pragma comment(lib, "SDL2main.lib")


Window::Window(int width, int height)
	:title("Software Rasterizer"),
	width(width),
	height(height), 
	sdlWindow(nullptr)
{
	Init();
}

Window::~Window()
{
	SDL_DestroyWindow(sdlWindow);
	SDL_Quit();
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

void Window::Update(float dt)
{
	SDL_UpdateWindowSurface(sdlWindow);
}

void Window::SwapBuffer(const Buffer<uint32_t>& buffer)
{
	SDL_LockSurface(sdlSurface);
	//Copy pixels buffer results to screen surface
	memcpy(sdlSurface->pixels, buffer.GetBuffer(), buffer.GetSize());
	SDL_UnlockSurface(sdlSurface);
}

bool Window::Init()
{
	if (!InitSDL())
		return  false;

	if (!CreateWindowSDL())
		return false;

	if (!CreateSurfaceSDL())
		return false;

	SDL_GL_SetSwapInterval(1);
	
	return true;
}

bool Window::InitSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return false;
	}
	return true;
}

bool Window::CreateWindowSDL()
{
	sdlWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if(!sdlWindow)
	{
		printf("Window could not be created: %s\n", SDL_GetError());
		return false;
	}
	return true;
}

bool Window::CreateSurfaceSDL()
{
	sdlSurface = SDL_GetWindowSurface(sdlWindow);
	if(!sdlSurface)
	{
		printf("Could not create window surface. Error: %s\n", SDL_GetError());
		return false;
	}
	return true;
}
