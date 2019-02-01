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
			normals.emplace_back(x, y, z);
		}
		else if (line.substr(0, 2) == "f ")
		{
			std::stringstream ss(line.substr(2));

			std::vector<std::string> splitStrings;
			std::string x, y, z;
			ss >> x >> y >> z;
			auto processString = [&splitStrings](auto&& param, char delimiter)->decltype(auto)
			{
				std::stringstream stream(std::forward<decltype(param)>(param));
				std::string str;
				while(std::getline(stream, str, delimiter))
				{
					if (str.empty())
						str = "0";
					splitStrings.push_back(std::move(str));
				}
			};
			char delimeter = '/';
			processString(x, delimeter);
			processString(y, delimeter);
			processString(z, delimeter);
			auto handleNegativeIndex = [](int index, int maxIndex) -> decltype(auto)
			{
				return index > 0 ? index : maxIndex - abs(index) + 1;
			};
			faces.push_back(std::vector<int>
			{
				handleNegativeIndex(std::stoi(splitStrings[0 * 3 + 0]), vertices.size()) - 1,
				handleNegativeIndex(std::stoi(splitStrings[1 * 3 + 0]), vertices.size()) - 1,
				handleNegativeIndex(std::stoi(splitStrings[2 * 3 + 0]), vertices.size()) - 1
			});
			faceTexCoords.push_back(std::vector<int>
			{
				handleNegativeIndex(std::stoi(splitStrings[0 * 3 + 1]), texCoords.size()) - 1,
				handleNegativeIndex(std::stoi(splitStrings[1 * 3 + 1]), texCoords.size()) - 1,
				handleNegativeIndex(std::stoi(splitStrings[2 * 3 + 1]), texCoords.size()) - 1
			});
			faceNormals.push_back(std::vector<int>
			{
				handleNegativeIndex(std::stoi(splitStrings[0 * 3 + 2]), normals.size()) - 1,
				handleNegativeIndex(std::stoi(splitStrings[1 * 3 + 2]), normals.size()) - 1,
				handleNegativeIndex(std::stoi(splitStrings[2 * 3 + 2]), normals.size()) - 1
			});
		}
	}
}

void Mesh::ProcessTriangles()
{
	int index = 0;
	for (auto it = faces.begin(); it != faces.end(); ++it, ++index)
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
