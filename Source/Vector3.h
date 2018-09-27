#pragma once

#include <cassert>
#include <cmath>
#include <iostream>

//Friend function cause a lot of trouble

template <class T>
struct Vec3
{
	T x, y, z;

	Vec3();
	Vec3(T x);
	Vec3(T x, T  y, T z);
	Vec3(const Vec3<T>& other);
	Vec3<T>& operator=(const Vec3<T>& other);

	Vec3<T> operator-() const;

	bool operator==(const Vec3<T>& other) const;
	bool operator!=(const Vec3<T>& other) const;

	Vec3<T>& operator+=(const Vec3<T>& other);
	Vec3<T>& operator-=(const Vec3<T>& other);
	Vec3<T>& operator*=(float value);
	Vec3<T>& operator/=(float value);

	float LengthSquared() const;
	float Length() const;
	Vec3<T> Normalize() const;
};

template <class T>
Vec3<T>::Vec3()
	:Vec3(0, 0, 0)
{
}

template <class T>
Vec3<T>::Vec3(T x)
	: Vec3(x, x, x)
{
}

template <class T>
Vec3<T>::Vec3(T x, T y, T z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

template <class T>
Vec3<T>::Vec3(const Vec3<T>& other)
	:Vec3(other.x, other.y, other.z)
{
}

template <class T>
Vec3<T>& Vec3<T>::operator=(const Vec3<T>& other)
{
	x = other.x;
	y = other.y;
	z = other.z;

	return *this;
}

template <class T>
Vec3<T> Vec3<T>::operator-() const
{
	return Vec3<T>(-x, -y, -z);
}

template <class T>
bool Vec3<T>::operator==(const Vec3<T>& other) const
{
	return x == other.x && y == other.y && z == other.z;
}

template <class T>
bool Vec3<T>::operator!=(const Vec3<T>& other) const
{
	return !(*this == other);
}

template <class T>
Vec3<T> operator+(const Vec3<T>& left, const Vec3<T>& right)
{
	return Vec3<T>(left.x + right.x, left.y + right.y, left.z + right.z);
}

template <class T>
Vec3<T> operator-(const Vec3<T>& left, const Vec3<T>& right)
{
	return left + (-right);
}

template <class T>
Vec3<T> operator*(const Vec3<T>& v, float f)
{
	return Vec3<T>(v.x * f, v.y * f, v.z * f);
}

template <class T>
Vec3<T> operator/(const Vec3<T>& v, float f)
{
	assert(f != 0.0f);
	return v * (1.0f / f);
}

template <class T>
Vec3<T> operator*(float f, const Vec3<T>& v)
{
	return v * f;
}

template <class T>
Vec3<T> operator/(float f, const Vec3<T>& v)
{
	assert(f != 0.0f);
	return v / f;
}

template <class T>
Vec3<T>& Vec3<T>::operator+=(const Vec3<T>& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

template <class T>
Vec3<T>& Vec3<T>::operator-=(const Vec3<T>& other)
{
	return *this += (-other);
}

template <class T>
Vec3<T>& Vec3<T>::operator*=(float value)
{
	x *= value;
	y *= value;
	z *= value;
	return *this;
}

template <class T>
Vec3<T>& Vec3<T>::operator/=(float value)
{
	return *this *= (1.0f / value);
}

template <class T>
float Vec3<T>::LengthSquared() const
{
	return Dot(*this, *this);
}

template <class T>
float Vec3<T>::Length() const
{
	return std::sqrt(LengthSquared());
}

template <class T>
Vec3<T> Vec3<T>::Normalize() const
{
	return *this / Length();
}

template <class T>
float Dot(const Vec3<T>& left, const Vec3<T>& right)
{
	return left.x * right.x + left.y * right.y + left.z * right.z;
}

template <class T>
Vec3<T> Cross(const Vec3<T>& left, const Vec3<T>& right)
{
	return Vec3<T>
		(
			left.y * right.z - right.y * left.z,
			-(left.x * right.z - right.x * left.z),
			left.x * right.y - right.x * left.y
		);
}

template <class T>
std::ostream& operator<<(std::ostream& stream, const Vec3<T>& vector)
{
	return stream << "[" << vector.x << ", " << vector.y << ", " << vector.z <<  "]";
}

typedef Vec3<int> Vector3i;
typedef Vec3<float> Vector3f;