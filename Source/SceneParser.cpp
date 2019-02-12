#include "SceneParser.h"
#include "json.hpp"
#include "OrderFifoMap.h"
#include <fstream>


SceneParser::SceneParser()
{

}

std::vector<Scene*> SceneParser::Parse(const std::string& file)
{
	using json = nlohmann::json;

	std::vector<Scene*> scenes;
	std::ifstream stream(file);
	order_json jsonMainObject;
	if(stream)
		stream >> jsonMainObject;
	for(auto& jScene : jsonMainObject)
	{	
		Scene* scene = new Scene;
		for(auto& jObject : jScene)
		{
			auto model = jObject["model"].get<std::string>();
			auto diffusemap = jObject["diffusemap"].get<std::string>();
			auto ambient = jObject["material"]["ambient"].get <std::vector<float>>();
			assert(ambient.size() == 3);
			auto diffuse = jObject["material"]["diffuse"].get <std::vector<float>>();
			assert(diffuse.size() == 3);
			auto specular = jObject["material"]["specular"].get <std::vector<float>>();
			assert(specular.size() == 3);
			auto exponent = jObject["material"]["exponent"].get<float>();

			auto position = jObject["transform"]["position"].get<std::vector<float>>();
			assert(position.size() == 3);
			auto rotation = jObject["transform"]["rotation"].get<std::vector<float>>();
			assert(rotation.size() == 3);
			auto scale = jObject["transform"]["scale"].get<std::vector<float>>();
			assert(scale.size() == 3);

			Object object(model.c_str(), diffusemap.c_str());
			object.SetMaterial(Material{
				Color{ambient[0], ambient[1], ambient[2]},
				Color{diffuse[0], diffuse[1], diffuse[2]},
				Color{specular[0], specular[1], specular[2]},
				exponent });
			object.SetTransform(Transform(
				Vector3f(position[0], position[1], position[2]),
				Vector3f(rotation[0], rotation[1], rotation[2]),
				Vector3f(scale[0], scale[1], scale[2])
			));
			
			scene->AddObject(std::move(object));
		}
		scenes.push_back(scene);
	}

	return scenes;
}
