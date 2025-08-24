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

    // robot camera
    mjvCamera leftCam{};
    mjvCamera rightCam{};

private:
    std::vector<double> target_positions; // Store target positions for stiffness control
    bool isSimulationRunning = false; // Simulation starts paused

public:

    MujocoContext(const std::string& xmlString);
    ~MujocoContext();

    // Copying could potentially lead to freeing the model or data twice.
    // Deleting the copy constructors prevents this.
    MujocoContext(const MujocoContext&) = delete;
    MujocoContext& operator=(const MujocoContext&) = delete;

    MujocoContext& operator=(MujocoContext&& other) noexcept;

    // Simulation control methods
    void playSimulation();
    void pauseSimulation();
    void toggleSimulation();
    bool isRunning() const;
};
}