#pragma once
#include <string>
#include <mujoco/mujoco.h>

namespace spqr {
class Simulator {
public:
    static void init(const std::string& modelPath);
    static void step();
    static void shutdown();

    static mjModel* getModel();
    static mjData* getData();

private:
    static mjModel* model;
    static mjData* data;
};
}
