#pragma once

#include <cassert>
#include <cmath>
#include <iostream>
#include "Vector3.h"

//Friend function cause a lot of trouble

template <class T>
struct Vec4
{
	T x, y, z, w;

	Vec4();
	Vec4(T x);
	Vec4(T x, T  y, T z, T w);
	Vec4(const Vec3<T>& v, T w);
	Vec4(const Vec4<T>& other);
	Vec4<T>& operator=(const Vec4<T>& other);

	Vec3<T> GetVec3() const;
	Vec4<T> DivideW() const;

	Vec4<T> operator-() const;

	bool operator==(const Vec4<T>& other) const;
	bool operator!=(const Vec4<T>& other) const;

	Vec4<T>& operator+=(const Vec4<T>& other);
	Vec4<T>& operator-=(const Vec4<T>& other);

	float LengthSquared() const;
	float Length() const;
	Vec4<T> Normalize() const;
};

template <class T>
Vec4<T>::Vec4()
	:Vec4(0, 0, 0, 0)
{
}

template <class T>
Vec4<T>::Vec4(T x)
	: Vec4(x, x, x, x)
{
	
}

template <class T>
Vec4<T>::Vec4(T x, T y, T z, T w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

template <class T>
Vec4<T>::Vec4(const Vec3<T>& v, T w)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	this->w = w;
}

template <class T>
Vec4<T>::Vec4(const Vec4<T>& other)
	:Vec4(other.x, other.y, other.z, other.w)
{
}

template <class T>
Vec4<T>& Vec4<T>::operator=(const Vec4<T>& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;

	return *this;
}

template <class T>
Vec3<T> Vec4<T>::GetVec3() const
{
	return Vec3<T>(x, y, z);
}

template <class T>
Vec4<T> Vec4<T>::DivideW() const
{
	float invW = 1.0f / w;
	return Vec4<T>(x * invW, y * invW, z * invW, 1.0f);
}

template <class T>
Vec4<T> Vec4<T>::operator-() const
{
	return Vec4<T>(-x, -y, -z, -w);
}

template <class T>
bool Vec4<T>::operator==(const Vec4<T>& other) const
{
	return x == other.x && y == other.y && z == other.z && w == other.w;
}

template <class T>
bool Vec4<T>::operator!=(const Vec4<T>& other) const
{
	return !(*this == other);
}

template <class T>
Vec4<T> operator+(const Vec4<T>& left, const Vec4<T>& right)
{
	return Vec4<T>(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
}

template <class T>
Vec4<T> operator-(const Vec4<T>& left, const Vec4<T>& right)
{
	return left + (-right);
}

template <class T>
Vec4<T> operator*(const Vec4<T>& v, float f)
{
	return Vec4<T>(v.x * f, v.y * f, v.z * f, v.w * f);
}

template <class T>
Vec4<T> operator/(const Vec4<T>& v, float f)
{
	assert(f != 0.0f);
	return v * (1.0f / f);
}

template <class T>
Vec4<T> operator*(float f, const Vec4<T>& v)
{
	return v * f;
}

template <class T>
Vec4<T> operator/(float f, const Vec4<T>& v)
{
	assert(f != 0.0f);
	return v / f;
}

template <class T>
Vec4<T>& Vec4<T>::operator+=(const Vec4<T>& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}

template <class T>
Vec4<T>& Vec4<T>::operator-=(const Vec4<T>& other)
{
	return *this += (-other);
}

template <class T>
float Vec4<T>::LengthSquared() const
{
	return Dot(*this, *this);
}

template <class T>
float Vec4<T>::Length() const
{
	return std::sqrt(LengthSquared());
}

template <class T>
Vec4<T> Vec4<T>::Normalize() const
{
	return *this / Length();
}

template <class T>
float Dot(const Vec4<T>& left, const Vec4<T>& right)
{
	return left.x * right.x + left.y * right.y + left.z * right.z + left.w * right.w;
}

//Cross product only exist in (2D), 3D, 7D

template <class T>
std::ostream& operator<<(std::ostream& stream, const Vec4<T>& vector)
{
	return stream << "[" << vector.x << ", " << vector.y << ", " << vector.z << ', ' <<  vector.w "]";
}

typedef Vec4<int> Vector4i;
typedef Vec4<float> Vector4f;