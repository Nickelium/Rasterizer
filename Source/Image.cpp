#include "Image.h"
#include "Color.h"
#include <string.h>

Image::Image(int width, int height)
	:width(width),
	height(height),
	buffer(new uint32_t[width * height])
{
}

Image::~Image()
{
	delete buffer;
}

int Image::GetWidth() const
{
	return width;
}

int Image::GetHeight() const
{
	return height;
}

uint32_t* Image::GetBuffer() const
{
	return buffer;
}

void Image::SetPixel(int i, int j, const Color& color)
{
	if (0 <= i && i < width &&
		0 <= j  && j < height)
		buffer[(height - 1 - j) * width + i] = color.GetInt();
}

Color Image::GetPixel(int i, int j) const
{
	return IntToColor(buffer[j * width + i]);
}

void Image::Clear(const Color& c)
{
	//Method1 : slow
	//uint32_t colorInt = c.GetInt();
	//for (int i = 0; i < width; ++i)
	//	for (int j = 0; j < height; ++j)
	//		buffer[i + j * width] = colorInt;

	//Method2: better, still slow
	//Method 2.5: precalculate colorInt :> way faster
	uint32_t colorInt = c.GetInt();
	for (uint32_t* p = buffer, *bufferEnd = buffer + width * height;
		p < bufferEnd; ++p)
		*p = colorInt;

	//Method3: way faster, but only per byte :> black/grey/white
	//memset(buffer, 128, width * height * sizeof(uint32_t));
}