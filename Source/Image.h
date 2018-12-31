#pragma once

#include <stdint.h>
struct Color;

class Image
{
public:
	Image(int width, int height);
	~Image();

	int GetWidth() const;
	int GetHeight() const;
	uint32_t* GetBuffer() const;

	void SetPixel(int i, int j, const Color& color);
	Color GetPixel(int i, int j) const;
	void Clear(const Color& c);

public:
	const int width;
	const int height;
	uint32_t* buffer;
};
