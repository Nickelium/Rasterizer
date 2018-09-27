#include "Matrix3.h"

#include "Maths.h"

#include <iomanip>
#include <cassert>

Matrix3f::Matrix3f(float m0, float m1, float m2,
	float m3, float m4, float m5,
	float m6, float m7, float m8)
{
	float list[] = { m0, m1, m2, m3, m4, m5, m6, m7, m8 };
	int i = 0;
	for (float& f : list)
		M[i++] = f;
}

Matrix3f::Matrix3f(const Vector3f& v1, const Vector3f& v2, const Vector3f& o)
	:Matrix3f(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, o.x, o.y, o.z)
{
}

Matrix3f::Matrix3f(float diag)
{
	for (int i = 0; i < N * N; ++i)
	{
		if (i % N == i / N)
			M[i] = diag;
		else
			M[i] = 0.0f;
	}
}

Vector3f Matrix3f::operator*(const Vector3f& v) const
{
	float x = M[0] * v.x + M[1] * v.y + M[2] * v.z;
	float y = M[3] * v.x + M[4] * v.y + M[5] * v.z;
	float z = M[6] * v.x + M[7] * v.y + M[8] * v.z;
	return Vector3f(x, y, z);
}

Matrix3f Matrix3f::operator*(const Matrix3f& other) const
{
	Matrix3f m;
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
		{
			float sum = 0.0f;
			for (int k = 0; k < N; ++k)
				sum += (*this)[i][k] * other[k][j];
			m[i][j] = sum;
		}
	return m;
}

Matrix3f Matrix3f::operator*(float scalar) const 
{
	Matrix3f N;
	for (int i = 0; i < Matrix3f::size; ++i)
		N.M[i] = M[i] * scalar;
	return N;
}

Matrix3f operator*(float scalar, Matrix3f& N)
{
	return N * scalar;
}

const float* Matrix3f::GetPointer() const
{
	return M;
}

Matrix3f Matrix3f::Identity()
{
	return Matrix3f(1.0f);
}

Matrix3f Matrix3f::Translate(float dx, float dy)
{
	Matrix3f trans = Matrix3f::Identity();
	float list[] = { dx, dy };
	int i = 0;
	for (float f : list)
		trans[i++][N - 1] = f;
	return trans;
}

Matrix3f Matrix3f::Translate(const Vector2f& v)
{
	return Translate(v.x, v.y);
}

Matrix3f Matrix3f::Rotate(float degrees)
{
	Matrix3f m = Identity();
	float rad = Maths::Radians(degrees);
	m[0][0] = cos(rad);
	m[1][0] = sin(rad);
	m[0][1] = -sin(rad);
	m[1][1] = cos(rad);
	return m;
}

Matrix3f Matrix3f::Scale(float dx, float dy)
{
	Matrix3f scale = Matrix3f::Identity();
	float list[] = { dx, dy };
	int i = 0;
	for (float f : list)
	{
		scale[i][i] = f;
		++i;
	}
	return scale;
}

Matrix3f Matrix3f::Scale(const Vector2f& v)
{
	return Scale(v.x, v.y);
}

Matrix3f Matrix3f::Transpose(const Matrix3f& M)
{
	Matrix3f N = M;
	for(int i = 0; i < Matrix3f::N - 1; ++i)
		for (int j = i + 1; j < Matrix3f::N; ++j)
		{
			std::swap(N[i][j], N[j][i]);
		}
	return N;
}

//Developing from the first row
float Matrix3f::Determinant(const Matrix3f& M)
{
	return
		+M[0][0] * (Determinant(Vector2f{ M[1][1], M[2][1] }, Vector2f{ M[1][2], M[2][2] }))
		-M[0][1] * (Determinant(Vector2f{ M[1][0], M[2][0] }, Vector2f{ M[1][2], M[2][2] }))
		+M[0][2] * (Determinant(Vector2f{ M[1][0], M[2][0] }, Vector2f{ M[1][1], M[2][1] }));
}

//AdjunctMatrix / Determinant
//No (-1)^(i+j) => because determinant swap row & col => swap sign already
Matrix3f Matrix3f::Inverse(const Matrix3f& M)
{
	Matrix3f N;
	float det = Determinant(M);
	assert(abs(det) > 10e-4);

	float detInv = 1.0f / det;
	int sign = 1;
	for (int i = 0; i < Matrix3f::N; ++i)
		for (int j = 0; j < Matrix3f::N; ++j)
		{
			Vector2f v0 = { M[(j + 1) % Matrix3f::N][(i + 1) % Matrix3f::N],
							M[(j + 2) % Matrix3f::N][(i + 1) % Matrix3f::N] };
			Vector2f v1 = { M[(j + 1) % Matrix3f::N][(i + 2) % Matrix3f::N],
							M[(j + 2) % Matrix3f::N][(i + 2) % Matrix3f::N] };
			N[i][j] = detInv  * Determinant(v0, v1);
		
		}
	return N;
}

float& Matrix3f::ProxyRow::operator[](unsigned int col)
{
	return parentMatrix.M[row * N + col];
}

Matrix3f::ProxyRow::ProxyRow(Matrix3f& parent, unsigned int row)
	:parentMatrix(parent),
	row(row)
{
}

Matrix3f::ProxyRow Matrix3f::operator[](unsigned int row)
{
	return ProxyRow(*this, row);
}

float Matrix3f::ProxyRowConst::operator[](unsigned int col) const
{
	return parentMatrix.M[row * N + col];
}

Matrix3f::ProxyRowConst::ProxyRowConst(const Matrix3f& parent, unsigned int row)
	:parentMatrix(parent),
	row(row)
{
}

Matrix3f::ProxyRowConst Matrix3f::operator[](unsigned int row) const
{
	return ProxyRowConst(*this, row);
}

 float Matrix3f::Determinant(const Vector2f& v0, const Vector2f& v1)
{
	 return v0.x * v1.y - v0.y * v1.x;
}

std::ostream& operator<<(std::ostream& os, const Matrix3f& M)
{
	for (unsigned int i = 0; i != M.N; ++i)
	{
		for (unsigned int j = 0; j != M.N; ++j)
		{
			if (j != 0) os << " ";
			os << std::setw(5) << std::setfill(' ') << std::setprecision(3)
				<< M[i][j];
		}
		os << "\n";
	}
	return os;
}