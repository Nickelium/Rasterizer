#pragma once

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

	void Update();
private:
	void Init();

	const char* title;
	int width, height;

	SDL_Window* sdlWindow;
};