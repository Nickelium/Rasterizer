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

void Triangle::Barycentric(const Vector3f* const vertices, const Vector3f& P, float& u, float& v, float& w)
{
	Vector2f a = { vertices[0].x, vertices[0].y };
	Vector2f b = { vertices[1].x, vertices[1].y };
	Vector2f c = { vertices[2].x, vertices[2].y };
	Vector2f d = { P.x, P.y };
	

	auto edgeFunction = [](Vector2f A, Vector2f B, Vector2f P)
	{
		return Cross(B - A, P - A);
	};

	float areaABC = edgeFunction(a, b, c);
	float areaPBC = edgeFunction(b, c, d);
	float areaPCA = edgeFunction(c, a, d);
	float areaPAB = edgeFunction(a, b, d);

	//Vector3f normal = Cross(b - a, c - a);
	//float areaABC = normal.LengthSquared();
	//float areaPBC = Dot(normal, Cross((P - b), (P - c)));
	//float areaPCA = Dot(normal, Cross((P - c), (P - a)));

	u = areaPBC / areaABC; // alpha
	v = areaPCA / areaABC; // beta
	w = 1.0f - u - v; // gamma
}

#pragma endregion
