#pragma once

#include <mujoco/mujoco.h>
#include <string>
#include <stdexcept>

namespace spqr {

struct MujocoContext {
    mjModel* model = nullptr;
    mjData* data = nullptr;
    mjvCamera cam{};
    mjvOption opt{};
    mjvScene scene{};

    MujocoContext(const std::string& xmlFile) {
        char error[1024] = {0};
        model = mj_loadXML(xmlFile.c_str(), nullptr, error, sizeof(error));
        if (!model) throw std::runtime_error(error);

        data = mj_makeData(model);

        mjv_defaultOption(&opt);
        mjv_defaultCamera(&cam);
        mjv_makeScene(model, &scene, 10000);
    }

    ~MujocoContext() {
        if (data) mj_deleteData(data);
        if (model) mj_deleteModel(model);
        mjv_freeScene(&scene);
    }

    // Copying could potentially lead to freeing the model or data twice.
    // Deleting the copy constructors prevents this.
    MujocoContext(const MujocoContext&) = delete;
    MujocoContext& operator=(const MujocoContext&) = delete;

    MujocoContext& operator=(MujocoContext&& other) noexcept {
        if (this != &other) {
            // free existing resources
            if (data) mj_deleteData(data);
            if (model) mj_deleteModel(model);
            mjv_freeScene(&scene);

            model = other.model;
            data = other.data;
            cam = other.cam;
            opt = other.opt;
            scene = other.scene;

            other.model = nullptr;
            other.data = nullptr;
        }
        return *this;
    }
};
}