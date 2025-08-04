#pragma once

#include <string>
#include <yaml-cpp/yaml.h>
#include <unordered_set>
#include <pugixml.hpp>
#include <filesystem> 
#include <Eigen/Eigen>

using namespace pugi;
using namespace std;
using namespace Eigen;

namespace spqr {

struct RobotSpec {
    string name;
    string type;
    Vector3d position;
    Vector3d orientation;  // Euler angles
};

struct TeamSpec {
    string name;
    std::vector<RobotSpec> robots;
};

struct SceneSpec {
    std::string field;
    std::vector<TeamSpec> teams;
};
class SceneParser {
public:
    SceneParser(const string& yamlPath);
    string buildMuJoCoXml();

private:
    void buildRobotCommon(const string& robotType, xml_node& mujoco);
    void buildRobotInstance(const RobotSpec& robotSpec, xml_node& worldbody, xml_node& actuator, xml_node& sensor);
    void prefixSubtree(xml_node& root, const std::string& robotName);

    unordered_set<string> robotTypes;
    YAML::Node sceneRoot;
    SceneSpec scene;
    filesystem::path sceneBaseDir;
};

}
