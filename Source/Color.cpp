#include "Color.h"

#include <cassert>
#include <algorithm>
#include "IShader.h"

const float Color::MIN_VALUEF = 0.0f;
const float Color::MAX_VALUEF = 1.0f;

const int Color::MIN_VALUEI = 0;
const int Color::MAX_VALUEI = 255;


Color::Color(float r, float g, float b)
	:r(r),
	g(g),
	b(b)
{
}

Color& Color::operator*=(float f)
{
	r = ClampValue(r * f);
	g = ClampValue(g * f);
	b = ClampValue(b * f);
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
	float invMaxValue = 1.0f / 255.0f;
	return  Color
	(
		float(c >> 16) * invMaxValue,
		float(c >> 8) * invMaxValue,
		float(c >> 0) * invMaxValue
	);
}

uint32_t ColorToInt(const Color& c)
{
	float maxValue = 255.0f;
	return
		(uint32_t(c.r * maxValue) << 16) |
		(uint32_t(c.g * maxValue) << 8) |
		(uint32_t(c.b * maxValue) << 0);
}

Color operator*(const Color& c, const Color& d)
{
	return Color(c.r * d.r, c.g * d.g, c.b * d.b);
}

Color operator*(const Color& c, const Vector3f& v)
{
	return c * Vector4f(v, 1.0f);
}

Color operator*(const Color& c, const Vector4f& v)
{
	return Color(c.r * v.x, c.g * v.y, c.b * v.z);
}

Color operator*(const Vector3f& v, const Color& c)
{
	return c * v;
}

Color operator*(const Vector4f& v, const Color& c)
{
	return c * v;
}

Color operator+(const Color& c, const Color& d)
{
	return Color(c.r + d.r, c.g + d.g, c.b + d.b);
}

Color operator*(const Color& c, float f)
{
	return Color(c.r * f, c.g * f, c.b * f);
}

Color operator*(float f, const Color& c)
{
	return c * f;
}

Color operator/(const Color& c, float f)
{
	return c * (1.0f / f);
}

Color Color::Clamp(const Color& c)
{
	return Color(ClampValue(c.r), ClampValue(c.g), ClampValue(c.b));
}

float Color::ClampValue(float value)
{
	return Maths::Clamp(Color::MIN_VALUEF, Color::MAX_VALUEF, value);
}
