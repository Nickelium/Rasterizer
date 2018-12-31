#include "Window.h"

#include <SDL\SDL.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl_gl3.h>
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
	:width(width),
	height(height), 
	title("Software Rasterizer"),
	sdlWindow(nullptr)
{
	Init();
}

Window::~Window()
{
	//SDL_GL_DeleteContext(glcontext);
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

void Window::Update()
{
	SDL_UpdateWindowSurface(sdlWindow);
}

void Window::Init()
{
	// Setup SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);
	sdlWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (!sdlWindow)
		fprintf(stderr, "Window could not be created: %s\n", SDL_GetError());
	SDL_GLContext glcontext = SDL_GL_CreateContext(sdlWindow);

	SDL_GL_SetSwapInterval(1); //Sync update with monitor screen
}