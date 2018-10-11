#include "Triangle.h"

#pragma region Triangle

Triangle::Triangle(const Vector3f& p0, const Vector3f& p1, const Vector3f& p2)
{
	vertices.push_back(p0);
	vertices.push_back(p1);
	vertices.push_back(p2);
}

void Triangle::SetUVs(const Vector2f& uv0, const Vector2f& uv1, const Vector2f& uv2)
{
	uvs.push_back(uv0);
	uvs.push_back(uv1);
	uvs.push_back(uv2);
}

void Triangle::SetNormals(const Vector3f& n0, const Vector3f& n1, const Vector3f& n2)
{
	normals.push_back(n0);
	normals.push_back(n1);
	normals.push_back(n2);
}

Vector3f Triangle::operator[](size_t index) const
{
	return vertices[index];
}

Vector3f& Triangle::operator[](size_t index)
{
	return vertices[index];
}

std::vector<Vector3f> Triangle::GetVertices() const
{
	return vertices;
}

std::vector<Vector2f> Triangle::GetUVs() const
{
	return uvs;
}


std::vector<Vector3f> Triangle::GetNormals() const
{
	return normals;
}

int Triangle::Size() 
{
	return 3;
}

void Triangle::Barycentric(Vector3f* vertices, const Vector3f& P, float& u, float& v, float& w)
{
	// Dont know why without dot(n) not working
	Vector3f a = vertices[0];
	Vector3f b = vertices[1];
	Vector3f c = vertices[2];
	Vector3f normal = Cross(b - a, c - a);

	float areaABC = normal.LengthSquared();
	float areaPBC = Dot(normal, Cross((P - b), (P - c)));
	float areaPCA = Dot(normal, Cross((P - c), (P - a)));

	u = areaPBC / areaABC; // alpha
	v = areaPCA / areaABC; // beta
	w = 1.0f - u - v; // gamma
}

//Selection Sort on y value, because only 3 vertices
void Triangle::Sort(Triangle& triangle) 
{
	size_t nbVertices = 3, k = 0; float minY;
	for (size_t i = 0; i < nbVertices; ++i)
	{
		k = i;
		minY = triangle[i].y;
		for (size_t j = i + 1; j < nbVertices; ++j)
			if (minY > triangle[j].y)
			{
				minY = triangle[j].y;
				k = j;
			}
		std::swap(triangle[k], triangle[i]);
	}
}

//Multiply
//Triangle operator*(const Matrix4f& M, const Triangle& triangle) 
//{
//	Triangle transform = triangle;
//	transform[0] = (M * Vector4f(triangle[0], 1.0f)).DivideW();
//	transform[1] = (M * Vector4f(triangle[1], 1.0f)).DivideW();
//	transform[2] = (M * Vector4f(triangle[2], 1.0f)).DivideW();
//
//	return transform;
//}

#pragma endregion
