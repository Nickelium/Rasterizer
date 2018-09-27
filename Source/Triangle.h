#pragma once

#include "Maths.h"

#include <vector>


struct Triangle
{
public:
	Triangle(const Vector3f& p0, const Vector3f& p1, const Vector3f& p2);
	void SetUVs(const Vector2f& uv0, const Vector2f& uv1, const Vector2f& uv2);
	void SetNormals(const Vector3f& n0, const Vector3f& n1, const Vector3f& n2);

	Vector3f operator[](size_t index) const;
	Vector3f& operator[](size_t index);
	std::vector<Vector3f> GetVertices() const;
	std::vector<Vector2f> GetUVs() const;
	std::vector<Vector3f> GetNormals() const;

	static void Barycentric(const Triangle& triangle, const Vector3f& P, float& u, float& v, float& w);
	static void Sort(Triangle& triangle);
private:
	std::vector<Vector3f> vertices;
	std::vector<Vector2f> uvs;
	std::vector<Vector3f> normals;
};

Triangle operator*(const Matrix4f& M, const Triangle& triangle);
