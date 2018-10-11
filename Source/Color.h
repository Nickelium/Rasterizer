#pragma once
#include <stdint.h>
#include "Vector3.h"
#include "Vector4.h"


struct Color
{
	Color(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
	Color& operator*=(float f);
	Color& operator/=(float f);

	uint32_t GetInt() const;
	float r, g, b, a;

	static const float MIN_VALUEF;
	static const float MAX_VALUEF;

	static const int MIN_VALUEI;
	static const int MAX_VALUEI;
private:
	static float ClampValue(float value);
};

Color operator+(const Color& c, const Color& d);
Color operator*(const Color& c, const Color& d);
Color operator*(const Color& c, const Vector3f& v);
Color operator*(const Color& c, const Vector4f& v);
Color operator*(const Vector3f& v, const Color& c);
Color operator*(const Vector4f& v, const Color& c);
Color operator*(const Color& c, float f);
Color operator*(float f, const Color& c);
Color operator/(const Color& c, float f);

Color IntToColor(uint32_t c);

uint32_t ColorToInt(const Color& c);
