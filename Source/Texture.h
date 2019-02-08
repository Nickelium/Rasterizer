#pragma once

#include <stdint.h>
#include <string>

#include "Color.h"

/*
 * Class Texture
 * 
 * - Provides access to individual texture element
 * - Uses stbi to load images
 */
class Texture
{
public:
	Texture(const std::string& fileName);
	~Texture();

	Color GetColor(float u, float v) const;

private:
	uint8_t* image;
	int width;
	int height;
	int comp;
	
	static char* s_Location;
};

