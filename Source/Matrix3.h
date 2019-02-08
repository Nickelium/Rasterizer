#pragma once

#include "Vector3.h"
#include "Vector2.h"

/*
 * Class Matrix3f
 * 
 * - Represents a matrix with row and column of length 3
 * - Elements stored as rows
 * - Column based matrix
 * - Contains corresponding matrix operations
 * - Implemented element access with proxy design pattern 
 * 
 */
class Matrix3f
{
public:
	Matrix3f(float diag = 1.0f);
	Matrix3f(float m0, float m1, float m2,
			 float m3, float m4, float m5, 
			 float m6, float m7, float m8);
	Matrix3f(const Vector3f& v1, const Vector3f& v2, const Vector3f& o);

	Vector3f operator*(const Vector3f& v) const;
	Matrix3f operator*(const Matrix3f& other) const;
	Matrix3f operator*(float scalar) const;
	friend Matrix3f operator*(float scalar, const Matrix3f& N);

	const float* GetPointer() const;

	static Matrix3f Identity();
	static Matrix3f Translate(float dx, float dy);
	static Matrix3f Translate(const Vector2f& dv);
	static Matrix3f Rotate(float degrees);
	static Matrix3f Scale(float dx, float dy);
	static Matrix3f Scale(const Vector2f& dv);
	
	static Matrix3f Transpose(const Matrix3f& M);	
	static float Determinant(const Matrix3f& M);
	static Matrix3f Inverse(const Matrix3f& M);

	class ProxyRow
	{
		friend class Matrix3f;
	public:
		float& operator[](unsigned int col);
	private:
		ProxyRow(Matrix3f& parent, unsigned int row);
		unsigned int row;
		Matrix3f& parentMatrix;
	};
	friend class ProxyRow;
	ProxyRow operator[](unsigned int row);
	
	class ProxyRowConst
	{
		friend class Matrix3f;
	public:
		float operator[](unsigned int col) const;
	private:
		ProxyRowConst(const Matrix3f& parent, unsigned int row);
		unsigned int row;
		const Matrix3f& parentMatrix;
	};
	friend class ProxyRowConst;
	ProxyRowConst operator[](unsigned int row) const;

	static const int N = 3;
private:
	static const int size = N * N;
	//row based storage
	//column based basis vectors
	float M[size];

	//Determinant 2x2
	static float Determinant(const Vector2f& v0, const Vector2f& v1);
};

std::ostream& operator<<(std::ostream& os, const Matrix3f& M);