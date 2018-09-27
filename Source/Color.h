#pragma once
#include <stdint.h>

struct Color
{
	Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255);
	Color& operator*=(float f);
	Color& operator/=(float f);

	uint32_t GetInt() const;
	uint8_t r, g, b, a;
};

Color operator*(const Color& c, float f);
Color operator*(float f, const Color& c);
Color operator/(const Color& c, float f);

Color IntToColor(uint32_t c);

uint32_t ColorToInt(const Color& c);
