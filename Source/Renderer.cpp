#include "Renderer.h"

#include <SDL\SDL.h>
#include <cstdio>
#include <algorithm>

#include "Window.h"
#include "Triangle.h"
#include "Maths.h"
#include "Scene.h" 
#include "Texture.h"
#include "Camera.h"
#include "IShader.h"
#include "Shaders.h"


Renderer::Renderer(Window* window)
	:renderMode(RENDER_MODE::RM_WIREFRAME),
	window(window),
	width(window->GetWidth()),
	height(window->GetHeight()),
	cBuffer(width, height),
	zBuffer(width, height),
	backgroundColor(234.0f / Color::MAX_VALUEF,
					239.0f / Color::MAX_VALUEF,
					247.0f / Color::MAX_VALUEF),
	index(0u),
	wireframeRender(true)
{
	shaders.push_back(new FlatShading());
	shaders.push_back(new GouraudShading());
	shaders.push_back(new PhongShading());
	shaders.push_back(new TexturedShading());

	ConfigureRenderSettings();
}

Renderer::~Renderer()
{
	for (auto& shad : shaders)
		delete shad;
}

void Renderer::Clear()
{
	cBuffer.Clear(ColorToInt(backgroundColor));
	zBuffer.Clear(std::numeric_limits<float>::max());
}

void Renderer::Finalize()
{
	window->SwapBuffer(cBuffer);
}

void Renderer::ConfigureRenderSettings()
{
	switch(renderMode)
	{
	case RENDER_MODE::RM_WIREFRAME:
		wireframeRender = true;
		shader = shaders[static_cast<unsigned char>(SHADER_TYPES::ST_FLAT)];
		break;
	case RENDER_MODE::RM_FLAT:
		wireframeRender = false;
		shader = shaders[static_cast<unsigned char>(SHADER_TYPES::ST_FLAT)];
		break;
	case RENDER_MODE::RM_GOURAUD:
		wireframeRender = false;
		shader = shaders[static_cast<unsigned char>(SHADER_TYPES::ST_GOURAUD)];
		break;
	case RENDER_MODE::RM_PHONG:
		wireframeRender = false;
		shader = shaders[static_cast<unsigned char>(SHADER_TYPES::ST_PHONG)];
		break;
	case RENDER_MODE::RM_TEXTURED:
		wireframeRender = false;
		shader = shaders[static_cast<unsigned char>(SHADER_TYPES::ST_TEXTURED)];
		break;
	default:
		break;
	}
}

void Renderer::Render(Scene& scene)
{
	Clear();

	Matrix4f V = scene.GetCamera().GetViewMatrix();
	Matrix4f P = Matrix4f::Perspective(60.0f, float(width) / height, 0.1f, 100.0f);
	Matrix4f S = Matrix4f::Viewport(width, height);
	LightParam lightInfo;
	lightInfo.direction = Vector3f(1.0f, 1.0f, 1.0f).Normalize();
	const float invMaxValue = 1.0f / 255.0f;
	lightInfo.diffuseColor = 
	Color
	(
		255.0f * invMaxValue, 
		249.0f * invMaxValue, 
		232.0f * invMaxValue
	);
	for (const Object& object : scene.GetObjects())
	{
		Texture* texture = object.GetDiffuseMap();
		Matrix4f M = object.GetTransform().GetMatrix();
		
		shader->UpdateUniforms(M, V, P, S, object, lightInfo);

		for (const Triangle& objectSpaceTriangle : object.GetMesh()->GetTriangles())
		{
			Vector3f vertices[3];
			for (int i = 0; i < 3; ++i)
				vertices[i] = shader->VertexShader(objectSpaceTriangle, i);
			if(wireframeRender)
				DrawTriangle(Triangle(vertices[0], vertices[1], vertices[2]));
			else
			{
				Vector3f normal = Cross(vertices[1] - vertices[0], vertices[2] - vertices[0]);
				bool backfaceCulling = normal.z < 0;
				if (backfaceCulling)
						continue;
				//ScanlineFast(*shader, vertices);
				ScanlineClean(*shader, vertices);
			}
			
		}
	}

	Finalize();
}

void Renderer::ChangeRenderMode()
{
	renderMode = static_cast<RENDER_MODE>((static_cast<unsigned char>(renderMode) + 1) % static_cast<unsigned char>(RENDER_MODE::RM_NUMBER_MODES));
	ConfigureRenderSettings();
}

void Renderer::DrawTriangle(const Triangle& triangle)
{
	for (int i = 0; i < 3; ++i)
	{
		Vector2i p0 (int(triangle[i % 3].x), int(triangle[i % 3].y));
		Vector2i p1 (int(triangle[(i + 1) % 3].x), int(triangle[(i + 1) % 3].y));
		DrawLine(p0, p1);
	}
}

//Bresenham's line algorithm
void Renderer::DrawLine(Vector2i p0, Vector2i p1)
{
	Color color(1.0f, 0.0f, 0);

	bool steep = false;
	if (abs(p0.x - p1.x) < abs(p0.y - p1.y))
	{
		p0 = Vector2i(p0.y, p0.x);
		p1 = Vector2i(p1.y, p1.x);
		steep = true;
	}
	if (p0.x > p1.x)
		std::swap(p0, p1);

	//Always map to one octant, either (I or VIII) with derivative/slope [-1, 1]

	int dx = p1.x - p0.x;
	int dy = p1.y - p0.y;
	float dydx = (float)dy / dx;

	int totalError = 0;
	//This deals with wether octant I or VIII
	int derror = dydx > 0 ? +1 : -1;
	int error = 2 * dy;

	Vector2i pi(p0);
	for ( pi.x = p0.x; pi.x < p1.x; ++pi.x)
	{
		if (!steep)
			cBuffer(pi.x, pi.y) = ColorToInt(color);
		else
			cBuffer(pi.y, pi.x) = ColorToInt(color);

		totalError += error;
		if (abs(totalError) > dx)
		{
			pi.y += derror;
			totalError -= 2 * dx * derror;
		}
	}
}

void Renderer::ScanlineClean(IShader& shader, Vector3f* vertScreen)
{
	if (abs(vertScreen[0].y - vertScreen[1].y) < 0.0001f &&
		abs(vertScreen[1].y - vertScreen[2].y) < 0.0001f)
		return;

	Vector2f min(std::numeric_limits<float>::max()),
		max(-std::numeric_limits<float>::max());
	for (int i = 0; i < Triangle::Size(); ++i)
	{
		Vector3f p = vertScreen[i];
		Vector2f p2D(p.x, p.y);
		min = Maths::Min(min, p2D);
		max = Maths::Max(max, p2D);
	}

	Color color(1.0f, 1.0f, 1.0f);
	float u, v, w;

	int xMin = std::max(0, std::min(window->GetWidth(), static_cast<int>(std::floor(min.x))));
	int yMin = std::max(0, std::min(window->GetHeight(), static_cast<int>(std::floor(min.y))));
	int xMax = std::max(0, std::min(window->GetWidth(), static_cast<int>(std::floor(max.x))));
	int yMax = std::max(0, std::min(window->GetHeight(), static_cast<int>(std::floor(max.y))));
	for (int y = yMin, endY = yMax; y <= endY; ++y)
		for (int x = xMin, endX = xMax; x <= endX; ++x)
		{
			Triangle::Barycentric(vertScreen, Vector3f(float(x + 0.5f), float(y + 0.5f), 0.0f), u, v, w);
			if (!(0.0f <= u && u <= 1.0f 
			&& 0.0f <= v && v <= 1.0f
			&& 0.0f <= w && w <= 1.0f)) continue;
			float Z = u * vertScreen[0].z + v * vertScreen[1].z + w * vertScreen[2].z;
			float zBufferValue = zBuffer(x, y);
			if (Z < zBufferValue)
			{
				zBuffer(x, y) = Z;
				shader.FragmentShader(Vector3f(u, v, w), color);
				cBuffer(x, y) = ColorToInt(color);
			}
		}
}

//Scanline algorithm
void Renderer::ScanlineFast(IShader& shader, Vector3f* vertices)
{
	if (abs(vertices[0].y - vertices[1].y) < 0.0001f &&
		abs(vertices[1].y - vertices[2].y) < 0.0001f)
		return;

	Maths::Sort(vertices, 3);
	Color color(1.0f, 1.0f, 1.0f);

	//Draw lower half
	float totalHeight = vertices[2].y - vertices[0].y;
	float heightT1T0 = vertices[1].y - vertices[0].y + 1;
	for (int y = int(vertices[0].y); y <= vertices[1].y; ++y)
	{
		float alpha = (y - int(vertices[0].y)) / totalHeight;
		float beta = (y - int(vertices[0].y)) / heightT1T0;
		Vector3f A = vertices[0] + (vertices[2] - vertices[0]) * alpha;
		Vector3f B = vertices[0] + (vertices[1] - vertices[0]) * beta;
		if (A.x > B.x)
			std::swap(A, B);
		uint32_t colorInt = color.GetInt();
		float u = 0.0f, v = 0.0f, w = 0.0f;
		for (uint32_t* p = cBuffer.GetBuffer() + y * cBuffer.GetWidth() + int(A.x),
			*end = cBuffer.GetBuffer() + y * cBuffer.GetWidth() + int(B.x), 
			x = int(A.x); p <= end; ++p, ++x)
		{
			Triangle::Barycentric(vertices, Vector3f(float(x + 0.5f), float(y + 0.5f), 0.0f), u, v, w);
			if (u < 0.0f || v < 0.0f || w < 0.0f) continue;
			float Z = u * vertices[0].z + v * vertices[1].z + w * vertices[2].z;
			float zBufferValue = zBuffer(x, y);
			if (Z < zBufferValue)
			{
				zBuffer(x, y) = Z;
				shader.FragmentShader(Vector3f(u, v, w), color);
				cBuffer(x, y) = ColorToInt(color);
			}
		}
	}

	//Draw upper half
	float heightT1T2 = vertices[2].y - vertices[1].y + 1;
	for (int y = int(vertices[1].y); y <= vertices[2].y; ++y)
	{
		float alpha = (y - vertices[0].y) / totalHeight;
		float beta = (y - vertices[1].y) / heightT1T2;
		Vector3f A = vertices[0] + (vertices[2] - vertices[0]) * alpha;
		Vector3f B = vertices[1] + (vertices[2] - vertices[1]) * beta;
		if (A.x > B.x)
			std::swap(A, B);
		uint32_t colorInt = color.GetInt();
		float u = 0.0f, v = 0.0f, w = 0.0f;
		for (uint32_t* p = cBuffer.GetBuffer() + y * cBuffer.GetWidth() + int(A.x),
			*end = cBuffer.GetBuffer() + y * cBuffer.GetWidth() + int(B.x),
			x = int(A.x); p <= end; ++p, ++x)
		{
			Triangle::Barycentric(vertices, Vector3f(float(x + 0.5f), float(y + 0.5f), 0.0f), u, v, w);
			if (u < 0.0f || v < 0.0f || w < 0.0f) continue;
			float Z = u * vertices[0].z + v * vertices[1].z + w * vertices[2].z;
			float zBufferValue = zBuffer(x, y);
			if (Z < zBufferValue)
			{
				zBuffer(x, y) = Z;
				shader.FragmentShader(Vector3f(u, v, w), color);
				cBuffer(x, y) = ColorToInt(color);
			}
		}
	}
}

