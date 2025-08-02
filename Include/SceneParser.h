#pragma once

#include <string>
#include <yaml-cpp/yaml.h>
#include <unordered_set>
#include <pugixml.hpp>
#include <filesystem> 

namespace spqr {

class SceneParser {
public:
    SceneParser(const std::string& yamlPath);
    std::string buildMuJoCoXml();

private:

    pugi::xml_node buildRobotInstance(const std::string& robotType);
    

    std::unordered_set<std::string> robotTypes;
    YAML::Node sceneRoot;

    std::filesystem::path sceneBaseDir;
};

}
