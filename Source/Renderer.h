#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Image.h"
#include "Color.h"

struct SDL_Surface;
struct SDL_Renderer;
struct SDL_Texture;
class Window;
class Scene;
struct Triangle;
class Texture;
class Camera;
class IShader;

class Renderer
{
public:
	Renderer(const Window& window);
	void Render(Scene& scene, Camera& camera);

	~Renderer();
private:
	void Init(const Window& window);
	void Clear();
	void ClearZBuffer();
	void Finalize();

	void DrawLine(Vector2i p0, Vector2i p1);
	void DrawTriangle(const Triangle& triangle);
	void ScanlineClean(IShader& shader, Vector3f* vertices, const Triangle& triangle);
	void ScanlineFast(Triangle triangle, float intensity);

	void SetZBuffer(int x, int y, float depth);
	float GetZBuffer(int x, int y) const;

	const int width, height;
	Image image;
	float* zBuffer;
	Color backgroundColor;

	SDL_Surface* sdlSurface;
	SDL_Renderer* sdlRenderer;
	SDL_Texture* sdlTexture;
};