#include "Matrix4.h"

#include "Maths.h"

#include <iomanip>
#include <cassert>

Matrix4f::Matrix4f(
	float m0, float m1, float m2, float m3,
	float m4, float m5, float m6, float m7,
	float m8, float m9, float m10, float m11,
	float m12, float m13, float m14, float m15)
{
	float list[] = { m0, m1, m2, m3,
					m4,	m5, m6, m7,
					m8, m9, m10, m11,
					m12, m13, m14, m15 };
	int i = 0;
	for (float& f : list)
		M[i++] = f;
}

Matrix4f::Matrix4f(const Vector4f& v1, const Vector4f& v2, const Vector4f& v3, const Vector4f& o)
	:Matrix4f(v1.x, v1.y, v1.z, v1.w,
			v2.x, v2.y, v2.z, v2.w, 
			v3.x, v3.y, v3.z, v3.w,
			o.x, o.y, o.z, o.w)
{
}

Matrix4f& Matrix4f::operator=(const Matrix4f& other)
{
	std::memcpy(M, other.M, sizeof(other.M));
	return *this;
}

Matrix4f::Matrix4f(float diag)
{
	for (int i = 0; i < N * N; ++i)
	{
		if (i % N == i / N)
			M[i] = diag;
		else
			M[i] = 0.0f;
	}
}

Vector4f Matrix4f::operator*(const Vector4f& v) const
{
	float x = M[0] * v.x + M[1] * v.y + M[2] * v.z + M[3] * v.w;
	float y = M[4] * v.x + M[5] * v.y + M[6] * v.z + M[7] * v.w;
	float z = M[8] * v.x + M[9] * v.y + M[10] * v.z + M[11] * v.w;
	float w = M[12] * v.x + M[13] * v.y + M[14] * v.z + M[15] * v.w;
	return Vector4f(x, y, z, w);
}

Matrix4f Matrix4f::operator*(const Matrix4f& other) const
{
	Matrix4f m;
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

Matrix4f Matrix4f::operator*(float scalar) const
{
	Matrix4f N;
	for (int i = 0; i < Matrix4f::size; ++i)
		N.M[i] = M[i] * scalar;
	return N;
}

Matrix4f operator*(float scalar, const Matrix4f& N)
{
	return N * scalar;
}

const float* Matrix4f::GetPointer() const
{
	return M;
}

Matrix4f Matrix4f::Identity()
{
	return Matrix4f(1.0f);
}

Matrix4f Matrix4f::Translate(float dx, float dy, float dz)
{
	Matrix4f trans = Matrix4f::Identity();
	float list[] = { dx, dy, dz };
	int i = 0;
	for (float f : list)
		trans[i++][N - 1] = f;
	return trans;
}

Matrix4f Matrix4f::Translate(const Vector3f& v)
{
	return Translate(v.x, v.y, v.z);
}

//Rodrigues Rotation in matrix-form
//ref: Mathematics for 3D Game Programming and Computer Graphics
//Chapter 3, p.62
/*
	@param axis: Axis of rotation, assumed to be normalized
		 angleDegrees: Angle of rotation given in degrees
	@return Rotation matrix
*/
Matrix4f Matrix4f::Rotate(const Vector3f& axis, float angleDegrees)
{
	Matrix4f M = Matrix4f::Identity();
	float angle = Maths::Radians(angleDegrees);
	float C = cos(angle);
	float S = sin(angle);
	M[0][0] = C + axis.x * axis.x * (1 - C);
	M[0][1] = axis.x * axis.y * (1 - C) - axis.z * S;
	M[0][2] = axis.x * axis.z * (1 - C) + axis.y * S;

	M[1][0] = axis.x * axis.y * (1 - C) + axis.z * S;
	M[1][1] = C + axis.y * axis.y * (1 - C);
	M[1][2] = axis.y * axis.z * (1 - C) - axis.x * S;

	M[2][0] = axis.x * axis.z * (1 - C) - axis.y * S;
	M[2][1] = axis.y * axis.z * (1 - C) + axis.x * S;
	M[2][2] = C + axis.z * axis.z * (1 - C);
	return M;
}

Matrix4f Matrix4f::Scale(float dx, float dy, float dz)
{
	Matrix4f scale = Matrix4f::Identity();
	float list[] = { dx, dy, dz };
	int i = 0;
	for (float f : list)
	{
		scale[i][i] = f;
		++i;
	}
	return scale;
}

Matrix4f Matrix4f::Scale(const Vector3f& v)
{
	return Scale(v.x, v.y, v.z);
}

/*
	@param nearZ and farZZ: are defined as coordinates on z-axis, therefore should generally
		be negative. Remark this is not like glm
	@return Orthographic matrix which defines the view as cuboid
			This matrix maps to a coordinate-system with the center of cuboid as the origin
			This computation is equivalent to (TS)^-1 = S^-1 * T^-1 = Orthographic
				which is computationally more efficient
			Due to the direction of the change of basis, we should compute as inverse.
*/
Matrix4f Matrix4f::Orthographic(float left, float right, float bottom, float top,
	float nearZ, float farZ)
{
	Matrix4f M = Matrix4f::Identity();
	M[0][0] = 2.0f / (right - left);
	M[1][1] = 2.0f / (top - bottom);
	M[2][2] = 2.0f / (farZ - nearZ);

	M[0][3] = -(right + left) / (right - left);
	M[1][3] = -(top + bottom) / (top - bottom);
	M[2][3] = -(farZ + nearZ) / (farZ - nearZ);

	return M;
}

Matrix4f Matrix4f::Orthographic(const Vector2f& x, const Vector2f& y, const Vector2f& z)
{
	return Matrix4f::Orthographic(x.x, x.y, y.x, y.y, z.x, z.y);
}

Matrix4f Matrix4f::Perspective(float left, float right, float bottom, float top,
	float nearZ, float farZ)
{
	Matrix4f M = Matrix4f::Identity();
	M[3][3] = 0.0f;
	M[3][2] = -1.0f;

	M[0][0] = 2.0f * nearZ / (right - left);
	M[0][2]= (right + left) / (right - left);

	M[1][1] = 2.0f * nearZ / (top - bottom);
	M[1][2] = (top + bottom) / (top - bottom);

	M[2][2] = -(farZ + nearZ) / (farZ - nearZ);
	M[2][3] = -2.0f * nearZ * farZ / (farZ - nearZ);

	return M;
}

Matrix4f Matrix4f::Perspective(float fov, float aspectRatio, float nearZ, float farZ)
{
	Matrix4f M = Matrix4f::Identity();
	fov = Maths::Radians(fov);
	M[0][0] = 1.0f / tan(fov * 0.5f);
	M[1][1] = (1.0f / tan(fov * 0.5f)) * aspectRatio;
	M[2][2] = -(farZ + nearZ) / (farZ - nearZ);
	M[2][3] = -(2.0f * farZ * nearZ) / (farZ - nearZ);
	M[3][2] = -1.0f;
	M[3][3] = 0.0f;
	return M;
}

Matrix4f Matrix4f::Perspective(const Vector2f& x, const Vector2f& y, const Vector2f& z)
{
	return Matrix4f::Perspective(x.x, x.y, y.x, y.y, z.x, z.y);
}

Matrix4f Matrix4f::View(const Vector3f& eye, const Vector3f& center, const Vector3f& up)
{
	//(TRS)^-1 = (RS)^-1 * T^-1
	//transpose(RS) * T^-1
	Vector3f z = (eye - center).Normalize();
	Vector3f x = Cross(up, z).Normalize();
	Vector3f y = Cross(z, x).Normalize();
	Matrix4f RS = Matrix4f(Vector4f(x, 0.0f), Vector4f(y, 0.0f), Vector4f(z, 0.0f), Vector4f(0.0f));
	Matrix4f Tinv = Matrix4f::Translate(-eye);
	return Matrix4f::Transpose(RS) * Tinv;
}

Matrix4f Matrix4f::Viewport(int width, int height)
{
	Matrix4f M;
	float halfWidth		= float(width >> 1);
	float halfHeight	= float(height >> 1);
	float halfDepth		= 255.0f * 0.5f;
	//map depth [0,255] : debug color purpose
	M[0][0] = halfWidth;
	M[0][3] = halfWidth;
	M[1][1] = halfHeight;
	M[1][3] = halfHeight;
	//M[2][2] = halfDepth;
	//M[2][3] = halfDepth;
	return M;
}

Matrix4f Matrix4f::Transpose(const Matrix4f& M)
{
	Matrix4f N = M;
	for (int i = 0; i < Matrix4f::N - 1; ++i)
		for (int j = i + 1; j < Matrix4f::N; ++j)
		{
			std::swap(N[i][j], N[j][i]);
		}
	return N;
}

//Developing from the first row
float Matrix4f::Determinant(const Matrix4f& M)
{
	return
		+M[0][0] *
		Matrix3f::Determinant(
			Matrix3f(
				Vector3f{ M[1][1], M[2][1], M[3][1] },
				Vector3f{ M[1][2], M[2][2], M[3][2] },
				Vector3f{ M[1][3], M[2][3], M[3][3] }))
		- M[0][1] *
		Matrix3f::Determinant(
			Matrix3f(
				Vector3f{ M[1][0], M[2][0], M[3][0] },
				Vector3f{ M[1][2], M[2][2], M[3][2] },
				Vector3f{ M[1][3], M[2][3], M[3][3] }))
		+ M[0][2] *
		Matrix3f::Determinant(
			Matrix3f(
				Vector3f{ M[1][0], M[2][0], M[3][0] },
				Vector3f{ M[1][1], M[2][1], M[3][1] },
				Vector3f{ M[1][3], M[2][3], M[3][3] }))
		- M[0][3] *
		Matrix3f::Determinant(
			Matrix3f(
				Vector3f{ M[1][0], M[2][0], M[3][0] },
				Vector3f{ M[1][1], M[2][1], M[3][1] },
				Vector3f{ M[1][2], M[2][2], M[3][2] }));
}

//AdjunctMatrix / Determinant
Matrix4f Matrix4f::Inverse(const Matrix4f& M)
{
	Matrix4f N;
	float det = Determinant(M);
	assert(abs(det) > 10e-4);
	float detInv = 1.0f / det;
	int sign;
	for (int i = 0; i < Matrix4f::N; ++i)
		for (int j = 0; j < Matrix4f::N; ++j)
		{
			Vector3f v0 =
			{ 
				M[(j + 1) % Matrix4f::N][(i + 1) % Matrix4f::N],
				M[(j + 2) % Matrix4f::N][(i + 1) % Matrix4f::N],
				M[(j + 3) % Matrix4f::N][(i + 1) % Matrix4f::N]
			};
			Vector3f v1 = 
			{ 
				M[(j + 1) % Matrix4f::N][(i + 2) % Matrix4f::N],
				M[(j + 2) % Matrix4f::N][(i + 2) % Matrix4f::N],
				M[(j + 3) % Matrix4f::N][(i + 2) % Matrix4f::N]
			};
			Vector3f v2 =
			{ 
				M[(j + 1) % Matrix4f::N][(i + 3) % Matrix4f::N],
				M[(j + 2) % Matrix4f::N][(i + 3) % Matrix4f::N],
				M[(j + 3) % Matrix4f::N][(i + 3) % Matrix4f::N]
			};
			sign = (i + j) % 2 == 0 ? 1 : -1; // More efficient than (-1)^(i+j)
			N[i][j] = sign * detInv * Matrix3f::Determinant(Matrix3f(v0, v1, v2));
		}
	return N;
}

float& Matrix4f::ProxyRow::operator[](unsigned int col)
{
	return parentMatrix.M[row * N + col];
}

Matrix4f::ProxyRow::ProxyRow(Matrix4f& parent, unsigned int row)
	:parentMatrix(parent),
	row(row)
{
}

Matrix4f::ProxyRow Matrix4f::operator[](unsigned int row)
{
	return ProxyRow(*this, row);
}

float Matrix4f::ProxyRowConst::operator[](unsigned int col) const
{
	return parentMatrix.M[row * N + col];
}

Matrix4f::ProxyRowConst::ProxyRowConst(const Matrix4f& parent, unsigned int row)
	:parentMatrix(parent),
	row(row)
{
}

Matrix4f::ProxyRowConst Matrix4f::operator[](unsigned int row) const
{
	return ProxyRowConst(*this, row);
}

std::ostream& operator<<(std::ostream& os, const Matrix4f& M)
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