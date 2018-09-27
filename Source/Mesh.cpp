#include "Mesh.h"
#include <fstream>
#include <string>
#include <sstream>

char* Mesh::s_Location = "Resources/";

Mesh::Mesh(const char* fileName)
{
	ParseFile((std::string(s_Location) + fileName).c_str());
	ProcessTriangles();
}

void Mesh::ParseFile(const char* fileName)
{
	std::ifstream in(fileName);
	if (!in)
	{
		std::cerr << "Failed to read obj file\n";
		return;
	}
	std::string line;
	while (std::getline(in, line))
	{
		if (line.substr(0, 2) == "v ")
		{
			std::stringstream ss(line.substr(2));
			float x, y, z;
			ss >> x >> y >> z;
			vertices.emplace_back(x, y, z);

		}
		else if (line.substr(0, 3) == "vt ")
		{
			std::stringstream ss(line.substr(3));
			float u, v;
			ss >> u >> v;
			texCoords.emplace_back(u, v);
		}
		else if (line.substr(0, 3) == "vn ")
		{
			std::stringstream ss(line.substr(3));
			float x, y, z;
			ss >> x >> y >> z;
			normals.emplace_back(x, y, -z);
		}
		else if (line.substr(0, 2) == "f ")
		{
			std::stringstream ss(line.substr(2));
			int v0, v1, v2;
			if (ss.str().find('/') != std::string::npos)
			{
				int uv0, uv1, uv2;
				int n0, n1, n2;

				char discardch;
	
				ss >> v0 >> discardch >> uv0 >> discardch >> n0;
				ss >> v1 >> discardch >> uv1 >> discardch >> n1;
				ss >> v2 >> discardch >> uv2 >> discardch >> n2;
				faceTexCoords.push_back(std::vector<int>{uv0 - 1, uv1 - 1, uv2 - 1});
				faceNormals.push_back(std::vector<int>{n0 - 1, n1 - 1, n2 - 1});
			}
			else
			{
				ss >> v0
					>> v1
					>> v2;
			}
			faces.push_back(std::vector<int>{v0 - 1, v1 - 1, v2 - 1});
		}
	}
}

void Mesh::ProcessTriangles()
{
	int index = 0;
	for (std::vector<std::vector<int>>::iterator it = faces.begin(); it != faces.end(); ++it, ++index)
	{
		Triangle triangle(vertices[(*it)[0]], vertices[(*it)[1]], vertices[(*it)[2]]);
		if (!texCoords.empty())
			triangle.SetUVs(
				texCoords[faceTexCoords[index][0]],
				texCoords[faceTexCoords[index][1]],
				texCoords[faceTexCoords[index][2]]);
		if (!normals.empty())
		{
			triangle.SetNormals(
				normals[faceNormals[index][0]],
				normals[faceNormals[index][1]],
				normals[faceNormals[index][2]]);
		}
		triangles.emplace_back(triangle);
	}
}

int Mesh::NbVertices() const
{
	return vertices.size();
}

int Mesh::NbFaces() const
{
	return faces.size();
}

std::vector<Triangle> Mesh::GetTriangles() const
{
	return triangles;
}
