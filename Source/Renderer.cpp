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


Renderer::Renderer(const Window& window)
	:sdlSurface(nullptr),
	sdlRenderer(nullptr), 
	sdlTexture(nullptr),
	width(window.GetWidth()),
	height(window.GetHeight()),
	image(width, height),
	zBuffer(new float[width * height]),
	backgroundColor(234.0f / Color::MAX_VALUEF,
					239.0f / Color::MAX_VALUEF,
					247.0f / Color::MAX_VALUEF),
	index(0u),
	useTexture(false),
	wireframeRender(true)
{
	shaders.push_back(new FlatShading());
	shaders.push_back(new GouraudShading());
	shaders.push_back(new PhongShading());
	shader = shaders[index];
	Init(window);
}

Renderer::~Renderer()
{
	delete[] zBuffer;
	for (auto& shad : shaders)
		delete shad;
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

	Matrix4f V = camera.GetViewMatrix();
	Matrix4f P = Matrix4f::Perspective(60.0f, float(width) / height, 0.1f, 100.0f);
	Matrix4f S = Matrix4f::Viewport(width, height);
	LightParam lightInfo;
	lightInfo.direction = Vector3f(1.0f, 1.0f, 1.0f).Normalize();
	const float invMaxValue = 1.0f / 255.0f;
	lightInfo.diffuseColor = Color(
		255.0f * invMaxValue, 
		249.0f * invMaxValue, 
		232.0f * invMaxValue, 1.0f);
	for (const Object& object : scene.GetObjects())
	{
		Texture* texture = object.GetDiffuseMap();
		Matrix4f M = object.GetTransform().GetMatrix();
		
		shader->UpdateUniforms(M, V, P, S, object, lightInfo, useTexture);

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
				//ScanlineFast(imageSpaceTriangle, intensity);	
				ScanlineClean(*shader, vertices, objectSpaceTriangle);
			}
			
		}
	}

	Finalize();
}

void Renderer::ChangeShader(bool orderDown)
{
	if (!orderDown)
		index = std::min(index + 1, shaders.size() - 1);
	else
		index = std::max((signed)index - 1, 0);

	shader = shaders[index];
}

void Renderer::ChangeRenderMode()
{
	if (wireframeRender && !useTexture)
	{
		wireframeRender = false;
		useTexture = false;
	}
	else if(!wireframeRender && !useTexture)
	{
		wireframeRender = false;
		useTexture = true;
	}
	else if(!wireframeRender && useTexture)
	{
		wireframeRender = true;
		useTexture = false;
	}
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
	Color color(1.0f, 0, 0);

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
//
//void Renderer::ScanlineClean(IShader& shader, Vector3f* vertices, const Triangle& triangle, Texture* texture)
//{
//	if (abs(triangle[0].y - triangle[1].y) < 0.0001f &&
//		abs(triangle[1].y - triangle[2].y) < 0.0001f)
//		return;
//
//	Vector2f Min(std::numeric_limits<float>::Max()), 
//			 Max(-std::numeric_limits<float>::Max());
//	for (const Vector3f& p : triangle.GetVertices())
//	{
//		Vector2f p2D(p.x, p.y);
//		Min = Maths::Min(Min, p2D);
//		Max = Maths::Max(Max, p2D);
//	}
//
//	Color color(0xFF, 0xFF, 0xFF);
//	float u, v, w;
//	for (int y = int((Min.y)), endY = int((Max.y)); y <= endY; ++y)
//		for (int x = int((Min.x)), endX = int((Max.x)); x <= endX; ++x)
//		{
//			Triangle::Barycentric(vertices, Vector3f(float(x), float(y), 0.0f), u, v, w);
//			if (u < 0.0f || v < 0.0f || w < 0.0f) continue;
//			float Z = u * triangle[0].z + v * triangle[1].z + w * triangle[2].z;
//			float zBufferValue = GetZBuffer(x, y);
//			if (Z < zBufferValue)
//			{
//				SetZBuffer(x, y, Z);
//				/*if (texture)
//				{
//					auto UVs = triangle.GetUVs();
//					Vector2f st = u * UVs[0] + v * UVs[1] + w * UVs[2];
//					color = texture->GetColor(st.x, st.y);*/
//				//}
//				shader.FragmentShader(Vector3f(u, v, w), color);
//				image.SetPixel(x, y, color);
//			}
//		}
//}

void Renderer::ScanlineClean(IShader& shader, Vector3f* vertices, const Triangle& triangle)
{
	if (abs(vertices[0].y - vertices[1].y) < 0.0001f &&
		abs(vertices[1].y - vertices[2].y) < 0.0001f)
		return;

	Vector2f min(std::numeric_limits<float>::max()),
		max(-std::numeric_limits<float>::max());
	for (int i = 0; i < Triangle::Size(); ++i)
	{
		Vector3f p = vertices[i];
		Vector2f p2D(p.x, p.y);
		min = Maths::Min(min, p2D);
		max = Maths::Max(max, p2D);
	}

	Color color(1.0f, 1.0f, 1.0f);
	float u, v, w;
	for (int y = int((min.y)), endY = int((max.y)); y <= endY; ++y)
		for (int x = int((min.x)), endX = int((max.x)); x <= endX; ++x)
		{
			Triangle::Barycentric(vertices, Vector3f(float(x), float(y), 0.0f), u, v, w);
			if (u < 0.0f || v < 0.0f || w < 0.0f) continue;
			float Z = u * vertices[0].z + v * vertices[1].z + w * vertices[2].z;
			float zBufferValue = GetZBuffer(x, y);
			if (Z < zBufferValue)
			{
				SetZBuffer(x, y, Z);
				shader.FragmentShader(Vector3f(u, v, w), color);
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
	Color color(1.0f, 1.0f, 1.0f);
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
			Triangle::Barycentric(nullptr, Vector3f(float(x), float(y), 0.0f), u, v, w);
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
			Triangle::Barycentric(nullptr, Vector3f(float(x), float(y), 0.0f), u, v, w);
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