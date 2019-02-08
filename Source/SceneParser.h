#pragma once
#include <string>
#include "Scene.h"

/**
 * Class SceneParser
 * 
 * - Parse scene config files into actual scenes & objects
 * - Currently read from config.json
 * TODO: User defined file name
 */
class SceneParser
{
public:
	SceneParser();
	std::vector<Scene*> Parse(const std::string& file);
private:

};
