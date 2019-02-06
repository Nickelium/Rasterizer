#pragma once

#include <stdint.h>
#include <cassert>
struct Color;

template <typename T>
class Buffer
{
public:
	Buffer::Buffer(int width, int height)
	:
		width(width),
		height(height),
		buffer(new T[width * height])
	{}
	~Buffer()
	{ delete buffer; }

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	int GetSize() const { return width * height * sizeof(T);  }
	T* GetBuffer() const { return buffer; }

	T& operator()(int i, int j)
	{
		return GetElement(i, j);
	}

	void SetElement(int i, int j, T value)
	{
		if(!(0 <= i && i < width)) return;
		if(!(0 <= j && j < height)) return;

		buffer[(height - 1 - j) * width + i] = value;
	}

	T GetElement(int i, int j) const
	{
		assert(0 <= i && i < width && "Out of buffer range width");
		assert(0 <= j && j < height && "Out of buffer range height");

		return buffer[(height - 1 - j)  * width + i];
	}

	T& GetElement(int i, int j) 
	{
		assert(0 <= i && i < width && "Out of buffer range width");
		assert(0 <= j && j < height && "Out of buffer range height");

		return buffer[(height - 1 - j)  * width + i];
	}

	void Clear(T value)
	{
		// Method1 : slow
		//uint32_t colorInt = c.GetInt();
		//for (int i = 0; i < width; ++i)
		//	for (int j = 0; j < height; ++j)
		//		buffer[i + j * width] = colorInt;

		//Method 2: pointer based :> way faster
		for (auto p = buffer, bufferEnd = buffer + width * height;
			p < bufferEnd; ++p)
			*p = value;

		//Method3: way faster, but only per byte :> black/grey/white
		//memset(buffer, 128, width * height * sizeof(uint32_t));
	}

private:
	const int width;
	const int height;
	T* buffer;
};
