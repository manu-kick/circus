#pragma once

#include <string>
#include <yaml-cpp/yaml.h>
#include <unordered_set>
#include <pugixml.hpp>
#include <Eigen/Eigen>

using namespace pugi;
using namespace std;
using namespace Eigen;

namespace spqr {

struct RobotInfo {
    string name;
    string team;
    string number;
    string type;
    Vector3d position;
    Vector3d orientation;  // Euler angles
};

struct TeamInfo {
    string name;
    std::vector<RobotInfo> robots;
};

struct SceneInfo {
    std::string field;
    std::vector<TeamInfo> teams;
};

class SceneParser {
public:
    SceneParser(const string& yamlPath);
    string buildMuJoCoXml();
    const SceneInfo& getSceneInfo() const { return scene; }

private:
    void buildRobotCommon(const string& robotType, xml_node& mujoco);
    void buildRobotInstance(const RobotInfo& robotInfo, xml_node& worldbody, xml_node& actuator, xml_node& sensor);
    void prefixSubtree(xml_node& root, const std::string& robotName);

    unordered_set<string> robotTypes;
    YAML::Node sceneRoot;
    SceneInfo scene;
};

}
