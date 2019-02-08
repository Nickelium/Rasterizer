#pragma once

//Vec2<float> should be compatible with the framework

#include <cassert>
#include <cmath>
#include <iostream>

//struct Point2f;
//Friend function cause a lot of trouble

/*
 * Template class Vec2
 * 
 * - Represent vector of size 2 
 * - Provides typical math vector operations
 */
template <class T>
struct Vec2
{
	T x, y;

	Vec2();
	Vec2(T x);
	Vec2(T x, T  y);
	Vec2(const Vec2<T>& other);
	Vec2<T>& operator=(const Vec2<T>& other);
	//Vec2<T>(const Point2f& p);
	//Vec2<T>(const Point2f& from, const Point2f& to);

	//Point2f ToPoint2f() const;
	bool Equals(const Vec2<T>& other, float epsilon = 0.001f) const;
	float DotProduct(const Vec2<T>& other) const;
	float CrossProduct(const Vec2<T>& other) const;

	Vec2<T> operator-() const;
	Vec2<T> operator+() const;

	bool operator==(const Vec2<T>& other) const;
	bool operator!=(const Vec2<T>& other) const;

	Vec2<T>& operator+=(const Vec2<T>& other);
	Vec2<T>& operator-=(const Vec2<T>& other);
	Vec2<T>& operator*=(float value);
	Vec2<T>& operator/=(float value);

	float LengthSquared() const;
	float SquaredLength() const;
	float Length() const;
	Vec2<T> Normalize() const;
	Vec2<T> Normalized() const;

	Vec2<T> Orthogonal() const;
};

template <class T>
Vec2<T>::Vec2()
	:Vec2(0,0)
{
}

template <class T>
Vec2<T>::Vec2(T x)
	:Vec2(x, x)
{
}

template <class T>
Vec2<T>::Vec2(T x, T y)
{
	this->x = x;
	this->y = y;
}

template <class T>
Vec2<T>::Vec2(const Vec2<T>& other)
	:Vec2(other.x, other.y)
{
}

template <class T>
Vec2<T>& Vec2<T>::operator=(const Vec2<T>& other)
{
	x = other.x;
	y = other.y;
	return *this;
}

//template <class T>
//Vec2<T>::Vec2(const Point2f& p)
//	:Vec2(p.x, p.y)
//{
//}
//
//template <class T>
//Vec2<T>::Vec2(const Point2f& from, const Point2f& to)
//	:Vec2(to.x - from.x, to.y - from.y)
//{
//}
//
//template <class T>
//Point2f Vec2<T>::ToPoint2f() const
//{
//	return Point2f(x, y);
//}

template <class T>
bool Vec2<T>::Equals(const Vec2<T>& other, float epsilon) const
{
	return abs(x - other.x) < epsilon &&
		abs(y - other.y) < epsilon;
}

template <class T>
Vec2<T> Vec2<T>::operator-() const
{
	return Vec2<T>(-x, -y);
}

template <class T>
Vec2<T> Vec2<T>::operator+() const
{
	return *this;
}

template <class T>
bool Vec2<T>::operator==(const Vec2<T>& other) const
{
	return Equals(other);
}

template <class T>
bool Vec2<T>::operator!=(const Vec2<T>& other) const
{
	return !(*this == other);
}

//template <class T>
//Point2f operator+(const Vec2<T> v, const Point2f& p)
//{
//	return p + v;
//}

template <class T>
Vec2<T> operator+(const Vec2<T>& left, const Vec2<T>& right)
{
	return Vec2<T>(left.x + right.x, left.y + right.y);
}

template <class T>
Vec2<T> operator-(const Vec2<T>& left, const Vec2<T>& right)
{
	return left + (-right);
}

template <class T>
Vec2<T> operator*(const Vec2<T>& v, float f)
{
	return Vec2<T>(T(v.x * f), T(v.y * f));
}

template <class T>
Vec2<T> operator/(const Vec2<T>& v, float f)
{
	assert(f != 0.0f);
	return v * (1.0f / f);
}

template <class T>
Vec2<T> operator*(float f, const Vec2<T>& v)
{
	return v * f;
}

template <class T>
Vec2<T> operator/(float f, const Vec2<T>& v)
{
	assert(f != 0.0f);
	return v / f;
}

template <class T>
Vec2<T>& Vec2<T>::operator+=(const Vec2<T>& other)
{
	x += other.x;
	y += other.y;
	return *this;
}

template <class T>
Vec2<T>& Vec2<T>::operator-=(const Vec2<T>& other)
{
	return *this += (-other);
}

template <class T>
Vec2<T>& Vec2<T>::operator*=(float value)
{
	x *= value;
	y *= value;
	return *this;
}

template <class T>
Vec2<T>& Vec2<T>::operator/=(float value)
{
	assert(value != 0);
	return *this *= 1.0f / value;
}

template <class T>
float Vec2<T>::LengthSquared() const
{
	return Dot(*this, *this);
}

template <class T>
float Vec2<T>::SquaredLength() const
{
	return LengthSquared();
}

template <class T>
float Vec2<T>::Length() const
{
	return std::sqrt(LengthSquared());
}

template <class T>
Vec2<T> Vec2<T>::Normalize() const
{
	return *this / Length();
}

template <class T>
Vec2<T> Vec2<T>::Normalized() const
{
	return Normalize();
}

template <class T>
float Dot(const Vec2<T>& left, const Vec2<T>& right) 
{
	return left.x * right.x + left.y * right.y;
}

template <class T>
float Cross(const Vec2<T>& left, const Vec2<T>& right) 
{
	return left.x * right.y - left.y * right.x;
}

template <class T>
float Vec2<T>::DotProduct(const Vec2<T>& other) const
{
	return Dot(*this, other);
}

template <class T>
float Vec2<T>::CrossProduct(const Vec2<T>& other) const
{
	return Cross(*this, other);
}

template <class T>
Vec2<T> Vec2<T>::Orthogonal() const
{
	return Vec2<T>(-y, x);
}

template <class T>
std::ostream& operator<<(std::ostream& stream, const Vec2<T>& vector)
{
	return stream << "[" << vector.x << ", " << vector.y << "]";
}

typedef Vec2<int> Vector2i;
typedef Vec2<float> Vector2f;