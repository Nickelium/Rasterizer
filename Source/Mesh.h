#pragma once

#include <vector>

#include "Vector3.h"
#include "Triangle.h"

class Mesh
{
public:
	Mesh(const char* fileName);

	int NbVertices() const;
	int NbFaces() const;

	std::vector<Triangle> GetTriangles() const;
private:
	void ParseFile(const char* fileName);
	void ProcessTriangles();

	std::vector<Vector3f> vertices;
	std::vector<Vector2f> texCoords;
	std::vector<Vector3f> normals;

	std::vector<std::vector<int>> faces;
	std::vector<std::vector<int>> faceTexCoords;
	std::vector<std::vector<int>> faceNormals;

	std::vector<Triangle> triangles;

	static char* s_Location;
};