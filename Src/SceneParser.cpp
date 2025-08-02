#include "SceneParser.h"
#include <yaml-cpp/yaml.h>
#include <sstream>

namespace spqr {

SceneParser::SceneParser(const std::string& yamlPath) {
    sceneRoot = YAML::LoadFile(yamlPath);
    sceneBaseDir = std::filesystem::path(yamlPath).parent_path();

    const auto& teams = sceneRoot["teams"];

    if (!teams || teams.size() > 2) {
        throw std::runtime_error("Scene must contain one or two teams.");
    }

    for (const auto& team : teams) {
        const auto& robots = team.second;

        if (!robots.IsSequence())
            throw std::runtime_error("Each team must be a sequence of robots.");

        for (const auto& robot : robots) {
            if (!robot["type"])
                throw std::runtime_error("Robot missing type field.");
            const std::string type = robot["type"].as<std::string>();
            robotTypes.insert(type);
        }
    }
}

std::string SceneParser::buildMuJoCoXml() {
    pugi::xml_document doc;

    auto mujoco = doc.append_child("mujoco");

    auto option = mujoco.append_child("option");
    option.append_attribute("timestep") = "0.01";
    option.append_attribute("iterations") = "50";
    option.append_attribute("tolerance") = "1e-10";
    option.append_attribute("solver") = "Newton";
    option.append_attribute("jacobian") = "dense";
    option.append_attribute("cone") = "pyramidal";

    auto include_node = mujoco.append_child("include");

    //TODO: parse the field type 
    include_node.append_attribute("file") = "includes/field.xml";

    for (const std::string& robotType : robotTypes) {
        std::filesystem::path commonPath = sceneBaseDir / "robots" / robotType / (robotType + "_common.xml");
        auto include_node = mujoco.append_child("include");
        include_node.append_attribute("file") = commonPath.string().c_str();
    }

    auto asset = mujoco.append_child("asset");

    auto texture = asset.append_child("texture");
    texture.append_attribute("type") = "skybox";
    texture.append_attribute("builtin") = "gradient";
    texture.append_attribute("rgb1") = "0.3 0.5 0.7";
    texture.append_attribute("rgb2") = "0 0 0";
    texture.append_attribute("width") = "512";
    texture.append_attribute("height") = "512";


    std::ostringstream oss;
    doc.save(oss, "  "); 

    return oss.str();
}

pugi::xml_node SceneParser::buildRobotInstance(const std::string& robotType) {
    // Replace this with actual XML mesh nodes for the robot type.

    std::filesystem::path instancePath = sceneBaseDir / "robots" / robotType / (robotType + "instance.xml");

}

}
