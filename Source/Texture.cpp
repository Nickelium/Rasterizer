#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "..\Dependencies\stb_image.h"

#include <string>
#include <algorithm>

char* Texture::s_Location = "Resources/";

Texture::Texture(const std::string& fileName)
{
	image = stbi_load((s_Location + fileName).c_str(), &width, &height, &comp, STBI_rgb);
}

Texture::~Texture()
{
	stbi_image_free(image);
}

Color Texture::GetColor(float u, float v) const
{
	assert(0.0f <= u && u <= 1.0f &&
		0.0f <= v && v <= 1.0f && "Sample from invalid UV\n");
	int x = int(std::round(u * (width - 1)));
	int y = height - 1 - int (std::round(v * (height - 1)));
	float invMaxValue = 1.0f / 255.0f;
	return Color(
		invMaxValue * image[(x + y * width) * comp + 0],
		invMaxValue * image[(x + y * width) * comp + 1],
		invMaxValue * image[(x + y * width) * comp + 2]);
}

