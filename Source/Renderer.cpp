#include "Renderer.h"

#include <SDL.h>
#include <cstdio>
#include <algorithm>

#include "Window.h"
#include "Triangle.h"
#include "Maths.h"
#include "Scene.h" 
#include "Texture.h"
#include "Camera.h"
#include "Vertex.h"


Renderer::Renderer(const Window& window)
	:sdlSurface(nullptr),
	sdlRenderer(nullptr), 
	sdlTexture(nullptr),
	width(window.GetWidth()),
	height(window.GetHeight()),
	image(width, height),
	zBuffer(new float[width * height]),
	backgroundColor(0xFF, 0xFF, 0x0)
{
	Init(window);
}

Renderer::~Renderer()
{
	delete[] zBuffer;
}

void Renderer::Init(const Window& window)
{
	sdlSurface = SDL_GetWindowSurface(window.GetSDLWindow());
	sdlRenderer = SDL_CreateSoftwareRenderer(sdlSurface);
	if (!sdlRenderer)
		fprintf(stderr, "Renderer could not be created: %s\n", SDL_GetError());

	sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ABGR32, SDL_TEXTUREACCESS_STREAMING, width, height);
	if (!sdlTexture)
		fprintf(stderr, "Texture rendering could not be created: %s\n", SDL_GetError());
}

void Renderer::Clear()
{
	image.Clear(backgroundColor);
	ClearZBuffer();
}

void Renderer::ClearZBuffer()
{
	float minValue = std::numeric_limits<float>::max();
	for (float* p = zBuffer, *end = zBuffer + width * height;
		p < end; ++p)
		*p = minValue;
}

void Renderer::Finalize()
{
	//update texture
	SDL_UpdateTexture(sdlTexture, nullptr, image.GetBuffer(), sizeof(uint32_t) * image.GetWidth());
	SDL_RenderCopy(sdlRenderer, sdlTexture,  nullptr, nullptr);
	SDL_RenderPresent(sdlRenderer);
}

void Renderer::Render(Scene& scene, Camera& camera)
{
	Clear();

	Vector3f lightDir = scene.GetLight().Normalize();
	Matrix4f V = camera.GetViewMatrix();
	Matrix4f P = Matrix4f::Perspective(60.0f, float(width) / height, 0.1f, 100.0f);
	Matrix4f S = Matrix4f::Viewport(width, height);
	Matrix4f VPS =   S * P * V ;
	for (const Object& object : scene.GetObjects())
	{
		Texture* texture = object.GetTexture();
		Matrix4f MVPS = VPS * object.GetTransform().GetMatrix();
		Matrix4f M = object.GetTransform().GetMatrix();

		for (const Triangle& objectSpaceTriangle : object.GetMesh()->GetTriangles())
		{
			Triangle worldSpaceTriangle = M * objectSpaceTriangle;
			Triangle imageSpaceTriangle = MVPS * objectSpaceTriangle;


			Vector3f normal;
			normal = Cross(worldSpaceTriangle[2] - worldSpaceTriangle[0], worldSpaceTriangle[1] - worldSpaceTriangle[0]).Normalize();

			float intensity = Dot(normal, lightDir);
			if (intensity > 0.0f)
			{
				//ScanlineFast(imageSpaceTriangle, intensity);	
				if(!imageSpaceTriangle.GetNormals().empty())
				ScanlineClean(imageSpaceTriangle, texture, lightDir);
				else ScanlineClean(imageSpaceTriangle, texture, intensity);
				//DrawTriangle(imageSpaceTriangle);
			}
		}
	}

	Finalize();
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
	Color color(255, 255, 255);

	p0.y = height - p0.y;
	p1.y = height - p1.y;

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
			image.SetPixel(pi.x, pi.y, color);
		else
			image.SetPixel(pi.y, pi.x, color);
			//SDL_RenderDrawPoint(sdlRenderer, pi.y, pi.x);

		totalError += error;
		if (abs(totalError) > dx)
		{
			pi.y += derror;
			totalError -= 2 * dx * derror;
		}
	}
}

void Renderer::ScanlineClean(const Triangle& triangle, Texture* texture, float intensity)
{
	if (abs(triangle[0].y - triangle[1].y) < 0.0001f &&
		abs(triangle[1].y - triangle[2].y) < 0.0001f)
		return;

	Vector2f min(std::numeric_limits<float>::max()), 
			 max(-std::numeric_limits<float>::max());
	for (const Vector3f& p : triangle.GetVertices())
	{
		Vector2f p2D(p.x, p.y);
		min = Maths::min(min, p2D);
		max = Maths::max(max, p2D);
	}

	Color color(0xFF, 0xFF, 0xFF);
	color = color * intensity;

	float u, v, w;
	for (int y = int((min.y)), endY = int((max.y)); y <= endY; ++y)
		for (int x = int((min.x)), endX = int((max.x)); x <= endX; ++x)
		{
			Triangle::Barycentric(triangle, Vector3f(float(x), float(y), 0.0f), u, v, w);
			if (u < 0.0f || v < 0.0f || w < 0.0f) continue;
			float Z = u * triangle[0].z + v * triangle[1].z + w * triangle[2].z;
			float zBufferValue = GetZBuffer(x, y);
			if (Z < zBufferValue)
			{
				SetZBuffer(x, y, Z);
				if (texture)
				{
					auto UVs = triangle.GetUVs();
					Vector2f st = u * UVs[0] + v * UVs[1] + w * UVs[2];
					color = texture->GetColor(st.x, st.y) * intensity;
				}
				image.SetPixel(x, y, color);
			}
		}
}

void Renderer::ScanlineClean(const Triangle& triangle, Texture* texture, const Vector3f& lightDir)
{
	if (abs(triangle[0].y - triangle[1].y) < 0.0001f &&
		abs(triangle[1].y - triangle[2].y) < 0.0001f)
		return;

	Vector2f min(std::numeric_limits<float>::max()),
		max(-std::numeric_limits<float>::max());
	for (const Vector3f& p : triangle.GetVertices())
	{
		Vector2f p2D(p.x, p.y);
		min = Maths::min(min, p2D);
		max = Maths::max(max, p2D);
	}

	Color color(0xFF, 0xFF, 0xFF);
	float diffuse0 = Dot(lightDir, triangle.GetNormals()[0]);
	float diffuse1 = Dot(lightDir, triangle.GetNormals()[1]);
	float diffuse2 = Dot(lightDir, triangle.GetNormals()[2]);

	float u, v, w;
	for (int y = int((min.y)), endY = int((max.y)); y <= endY; ++y)
		for (int x = int((min.x)), endX = int((max.x)); x <= endX; ++x)
		{
			Triangle::Barycentric(triangle, Vector3f(float(x), float(y), 0.0f), u, v, w);
			if (u < 0.0f || v < 0.0f || w < 0.0f) continue;
			float Z = u * triangle[0].z + v * triangle[1].z + w * triangle[2].z;
			float diffuse = u * diffuse0 + v * diffuse1 + w * diffuse2;
			diffuse = std::max(diffuse, 0.0f);
			float zBufferValue = GetZBuffer(x, y);
			if (Z < zBufferValue)
			{
				SetZBuffer(x, y, Z);
				if (texture)
				{
					auto UVs = triangle.GetUVs();
					Vector2f st = u * UVs[0] + v * UVs[1] + w * UVs[2];
					color = texture->GetColor(st.x, st.y) * diffuse;
				}
				image.SetPixel(x, y, color);
			}
		}
}

//Scanline algorithm
void Renderer::ScanlineFast(Triangle triangle, float intensity)
{
	if (abs(triangle[0].y - triangle[1].y) < 0.0001f &&
		abs(triangle[1].y - triangle[2].y) < 0.0001f)
		return;

	Triangle::Sort(triangle);
	Color color(255, 255, 255);
	color = color * intensity;

	//Draw lower half
	float totalHeight = triangle[2].y - triangle[0].y;
	float heightT1T0 = triangle[1].y - triangle[0].y + 1; 
	for (int y = int(triangle[0].y); y <= triangle[1].y; ++y)
	{
		float alpha = (y - int(triangle[0].y)) / totalHeight;
		float beta = (y - int(triangle[0].y)) / heightT1T0;
		Vector3f A = triangle[0] + (triangle[2] - triangle[0]) * alpha;
		Vector3f B = triangle[0] + (triangle[1] - triangle[0]) * beta;
		if (A.x > B.x)
			std::swap(A, B);
		uint32_t colorInt = color.GetInt();
		float u = 0.0f, v = 0.0f, w = 0.0f;
		for (uint32_t* p = image.buffer + y * image.width + int(A.x),
			*end = image.buffer + y * image.width + int(B.x), 
			x = int(A.x); p <= end; ++p, ++x)
		{
			Triangle::Barycentric(triangle, Vector3f(float(x), float(y), 0.0f), u, v, w);
			float Z = u * triangle[0].z + v * triangle[1].z + w * triangle[2].z;
			if (Z < GetZBuffer(x, y))
			{
				SetZBuffer(x, y, Z);
				image.SetPixel(x, y, color);
			}
		}
	}

	//Draw upper half
	float heightT1T2 = triangle[2].y - triangle[1].y + 1;
	for (int y = int(triangle[1].y); y <= triangle[2].y; ++y)
	{
		float alpha = (y - triangle[0].y) / totalHeight;
		float beta = (y - triangle[1].y) / heightT1T2;
		Vector3f A = triangle[0] + (triangle[2] - triangle[0]) * alpha;
		Vector3f B = triangle[1] + (triangle[2] - triangle[1]) * beta;
		if (A.x > B.x)
			std::swap(A, B);
		uint32_t colorInt = color.GetInt();
		float u = 0.0f, v = 0.0f, w = 0.0f;
		for (uint32_t* p = image.buffer + y * image.width + int(A.x),
			*end = image.buffer + y * image.width + int(B.x),
			x = int(A.x); p <= end; ++p, ++x)
		{
			Triangle::Barycentric(triangle, Vector3f(float(x), float(y), 0.0f), u, v, w);
			float Z = u * triangle[0].z + v * triangle[1].z + w * triangle[2].z;
			if (Z > GetZBuffer(x, y))
			{
				SetZBuffer(x, y, Z);
				image.SetPixel(x, y, color);
			}
		}
	}
}

void Renderer::SetZBuffer(int x, int y, float depth)
{
	if(0 <= x && x < width &&
		0 <= y && y < height)
	zBuffer[x + image.width * y] = depth;
}

float Renderer::GetZBuffer(int x, int y) const
{
	if (0 <= x && x < width &&
		0 <= y && y < height)
		return zBuffer[x + image.width * y];
	else return
		std::numeric_limits<float>::max();
}