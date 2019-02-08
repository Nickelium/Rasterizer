#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Buffer.h"
#include "Color.h"
#include "Shaders.h"

struct SDL_Surface;
struct SDL_Renderer;
struct SDL_Texture;
class Window;
class Scene;
struct Triangle;
class Texture;
class Camera;
class IShader;


/*
 * Class Renderer
 * 
 * - Renderer class will render given a Scene Object
 * - Defer to VertexShader and FragmentShader from the current active Shader
 * - Perform Rasterization (currently bounding box)
 */
class Renderer
{
public:
	Renderer(Window* window);
	void Render(Scene& scene);
	void ChangeRenderMode();

	~Renderer();
private:
	void Clear();
	void Finalize();

	void ConfigureRenderSettings();

	void DrawLine(Vector2i p0, Vector2i p1);
	void DrawTriangle(const Triangle& triangle);
	void ScanlineClean(IShader& shader, Vector3f* vertices);
	void ScanlineFast(IShader& shader, Vector3f* vertices);

	enum class RENDER_MODE : unsigned char
	{
		RM_WIREFRAME = 0,
		RM_FLAT,
		RM_GOURAUD,
		RM_PHONG,
		RM_TEXTURED,
		RM_DEPTH,
		RM_NUMBER_MODES
	} renderMode;

	enum class SHADER_TYPES: unsigned char
	{
		ST_FLAT = 0,
		ST_GOURAUD,
		ST_PHONG,
		ST_TEXTURED,
		ST_DEPTH,
		ST_NUMBER_TYPES
	};

	const int width, height;
	Buffer<uint32_t> cBuffer;
	Buffer<float> zBuffer;
	Color backgroundColor;

	Window* window;

	std::vector<IShader*> shaders;
	std::vector<IShader*>::size_type index;
	IShader* shader;

	bool wireframeRender;
};