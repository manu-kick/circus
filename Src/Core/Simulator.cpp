#include "Core/Simulator.h"
#include <stdexcept>

namespace spqr {

mjModel* Simulator::model = nullptr;
mjData* Simulator::data = nullptr;

void Simulator::init(const std::string& modelPath) {
    mj_activate(nullptr);
    char error[1000];
    model = mj_loadXML(modelPath.c_str(), 0, error, 1000);
    if (!model) throw std::runtime_error("MuJoCo load error: " + std::string(error));
    data = mj_makeData(model);
}

void Simulator::step() {
    if (model && data)
        mj_step(model, data);
}

void Simulator::shutdown() {
    if (data) mj_deleteData(data);
    if (model) mj_deleteModel(model);
    mj_deactivate();
}

mjModel* Simulator::getModel() { return model; }
mjData* Simulator::getData() { return data; }

}
