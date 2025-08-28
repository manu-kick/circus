#pragma once

#include <mujoco/mujoco.h>
#include <string>
namespace spqr {

struct MujocoContext {
    mjModel* model = nullptr;
    mjData* data = nullptr;
    mjvCamera camField{};
    mjvOption opt{};
    mjvScene scene{};

    bool isK1; // temporary -> just to be consistent with T1 and K1 cameras 

    // robot camera
    mjvCamera leftCam{}; // this is going to be the only one used for the T1
    mjvCamera rightCam{};

    MujocoContext(const std::string& xmlString);
    ~MujocoContext();

    // Copying could potentially lead to freeing the model or data twice.
    // Deleting the copy constructors prevents this.
    MujocoContext(const MujocoContext&) = delete;
    MujocoContext& operator=(const MujocoContext&) = delete;

    MujocoContext& operator=(MujocoContext&& other) noexcept;
};
}