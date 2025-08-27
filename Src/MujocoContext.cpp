#include "MujocoContext.h"
#include <stdexcept>
#include <memory>
#include <functional>
#include <filesystem>
#include <iostream>

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

        // Initialize all objects to their default positions
        // This ensures robots, field, and other objects are properly positioned when loaded
        mj_resetData(model, data);
        
        // Forward simulate a few steps to settle the initial configuration
        // but without actually advancing time (just for stability)
        for (int i = 0; i < 10; i++) {
            mj_forward(model, data);
        }

        // harcoded only for one robot
        isK1 = false;

        if (isK1) {
            std::string robotName("red_Booster-K1_0");
            leftCam.type = mjCAMERA_FIXED;
            leftCam.fixedcamid = mj_name2id(model, mjOBJ_CAMERA, (robotName + "_left_cam").c_str());
    
            rightCam.type = mjCAMERA_FIXED;
            rightCam.fixedcamid = mj_name2id(model, mjOBJ_CAMERA, (robotName + "_right_cam").c_str());
        }
        else {
            std::string robotName("red_Booster-T1_0");
            leftCam.type = mjCAMERA_FIXED;
            leftCam.fixedcamid = mj_name2id(model, mjOBJ_CAMERA, (robotName + "_depth_cam").c_str());
        }

        mjv_defaultOption(&opt);
        mjv_defaultCamera(&camField);
        mjv_makeScene(model, &scene, 10000);
        
        // Set camera to a good initial position to view the scene
        camField.azimuth = 90;    // Side view
        camField.elevation = -15; // Slightly from above
        camField.distance = 8;    // Good distance to see the field
        camField.lookat[0] = 0;   // Center of field
        camField.lookat[1] = 0;
        camField.lookat[2] = 0.5; // Slightly above ground
        
        // Ensure simulation starts paused
        isSimulationRunning = false;
        
        std::cout << "Scene loaded with " << model->nbody << " bodies. Simulation is paused." << std::endl;
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

    void MujocoContext::playSimulation() {
        isSimulationRunning = true;
        std::cout << "Simulation started" << std::endl;
    }

    void MujocoContext::pauseSimulation() {
        isSimulationRunning = false;
        std::cout << "Simulation paused" << std::endl;
    }

    void MujocoContext::toggleSimulation() {
        if (isSimulationRunning) {
            pauseSimulation();
        } else {
            playSimulation();
        }
    }

    bool MujocoContext::isRunning() const {
        return isSimulationRunning;
    }
}