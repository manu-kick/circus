#include "MujocoContext.h"
#include <stdexcept>
#include <memory>
#include <functional>
#include <filesystem>

namespace spqr {
    MujocoContext::MujocoContext(const std::string& xmlString){
        char error[1024] = {0};
        std::filesystem::current_path(PROJECT_ROOT);

        std::unique_ptr<mjSpec, std::function<void(mjSpec*)> > spec(
            mj_parseXMLString(xmlString.c_str(), nullptr, error, sizeof(error)),
            [](mjSpec* s) {
                mj_deleteSpec(s);
            }
        );

        if (!spec) {
            throw std::runtime_error(std::string("Failed to parse the generated XML. ")+error);
        }            
        
        model = mj_compile(spec.get(), nullptr);
        if (!model) {
            throw std::runtime_error(std::string("Failed to compile mujoco specs. ")+mjs_getError(spec.get()));
        }

        data = mj_makeData(model);

        // harcoded only for one robot
        camRobot.type = mjCAMERA_FIXED;
        camRobot.fixedcamid = mj_name2id(model, mjOBJ_CAMERA, "red_Booster-T1_0_depth_cam");

        mjv_defaultOption(&opt);
        mjv_defaultCamera(&camField);
        mjv_makeScene(model, &scene, 10000);
    }

    MujocoContext::~MujocoContext(){
        if (data) mj_deleteData(data);
        if (model) mj_deleteModel(model);
        mjv_freeScene(&scene);
    }

    MujocoContext& MujocoContext::operator=(MujocoContext&& other) noexcept {
        if (this != &other) {
            if (data) mj_deleteData(data);
            if (model) mj_deleteModel(model);
            mjv_freeScene(&scene);

            model = other.model;
            data = other.data;
            camField = other.camField;
            opt = other.opt;
            scene = other.scene;

            other.model = nullptr;
            other.data = nullptr;
        }
        return *this;
    }
}