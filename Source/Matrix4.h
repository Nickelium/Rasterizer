#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"


/*
 * Class Matrix4f
 *
 * - Represents a matrix with row and column of length 4
 * - Elements stored as rows
 * - Column based matrix
 * - Contains corresponding matrix operations
 * - Implemented element access with proxy design pattern
 *
 */
class Matrix4f
{
public:
	Matrix4f(float diag = 1.0f);
	Matrix4f(float m0, float m1, float m2, float m3,
			float m4, float m5, float m6, float m7,
			float m8, float m9, float m10, float m11,
			float m12, float m13, float m14, float m15);
	Matrix4f(const Vector4f& v1, const Vector4f& v2, const Vector4f& v3, const Vector4f& o);
	Matrix4f& operator=(const Matrix4f& other);

	Vector4f operator*(const Vector4f& v) const;
	Matrix4f operator*(const Matrix4f& other) const;
	Matrix4f operator*(float scalar) const;
	friend Matrix4f operator*(float scalar, const Matrix4f& N);

	const float* GetPointer() const;

	static Matrix4f Identity();
	static Matrix4f Translate(float dx, float dy, float dz);
	static Matrix4f Translate(const Vector3f& dv);
	static Matrix4f Rotate(const Vector3f& axis, float angleDegrees);
	static Matrix4f Scale(float dx, float dy, float dz);
	static Matrix4f Scale(const Vector3f& dv);
	static Matrix4f Orthographic(float left, float right, float bottom, float top,
		float nearZ, float farZ);
	static Matrix4f Orthographic(const Vector2f& x, const Vector2f& y, const Vector2f& z);
	static Matrix4f Perspective(float left, float right, float bottom, float top,
		float nearZ, float farZ);
	static Matrix4f Perspective(const Vector2f& x, const Vector2f& y, const Vector2f& z);
	static Matrix4f Perspective(float fov, float aspectRatio, float near, float far);
	static Matrix4f View(const Vector3f& eye, const Vector3f& center, const Vector3f& up);
	static Matrix4f Viewport(int width, int height);
		
	static Matrix4f Transpose(const Matrix4f& M);
	static float Determinant(const Matrix4f& M);
	static Matrix4f Inverse(const Matrix4f& M);

	class ProxyRow
	{
		friend class Matrix4f;
	public:
		float& operator[](unsigned int col);
	private:
		ProxyRow(Matrix4f& parent, unsigned int row);
		unsigned int row;
		Matrix4f& parentMatrix;
	};
	friend class ProxyRow;
	ProxyRow operator[](unsigned int row);

	class ProxyRowConst
	{
		friend class Matrix4f;
	public:
		float operator[](unsigned int col) const;
	private:
		ProxyRowConst(const Matrix4f& parent, unsigned int row);
		unsigned int row;
		const Matrix4f& parentMatrix;
	};
	friend class ProxyRowConst;
	ProxyRowConst operator[](unsigned int row) const;

	static const int N = 4;
private:
	static const int size = N * N;
	//row based storage
	//column based basis vectors
	float M[size];
};

std::ostream& operator<<(std::ostream& os, const Matrix4f& M);