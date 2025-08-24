#include "SimulationThread.h"
#include <QThread>

namespace spqr {

SimulationThread::SimulationThread(MujocoContext& context) : context_(context), running_(true) {}

void SimulationThread::run() {
    while (running_) {
        if (context_.isRunning()) {
            // Step the physics simulation
            mj_step(context_.model, context_.data);
        } else {
            // Sleep briefly when simulation is paused to avoid busy waiting
            QThread::msleep(16); // ~60 FPS refresh rate
        }
    }
}

void SimulationThread::stop() {
    running_ = false;
    wait();
}

}