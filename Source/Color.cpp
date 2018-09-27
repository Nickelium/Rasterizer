#include "Color.h"
#include <cassert>
#include <algorithm>

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	:r(r),
	g(g), 
	b(b),
	a(a)
{
}

Color& Color::operator*=(float f)
{
	r = int(r * f);
	g = int(g * f);
	b = int(b * f);
	return *this;
}

Color& Color::operator/=(float f)
{
	return *this *= (1.0f / f);
}

uint32_t Color::GetInt() const
{
	return ColorToInt(*this);
}

Color IntToColor(uint32_t c)
{
	return  Color
	(
		c >> 24,
		c >> 16,
		c >> 8,
		c
	);
}

uint32_t ColorToInt(const Color& c)
{
	return	
		(c.r << 24) |
		(c.g << 16) |
		(c.b << 8) |
		(c.a << 0);
}

Color operator*(const Color& c, float f)
{
	return Color(int(c.r * f), int(c.g * f), int(c.b * f), c.a);
}

Color operator*(float f, const Color& c)
{
	return c * f;
}

Color operator/(const Color& c, float f)
{
	assert(abs(f) > 10e-5);
	return c * (1.0f / f);
}