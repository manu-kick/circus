#pragma once

#include <mujoco/mujoco.h>
#include <QThread>

namespace spqr {

class SimulationThread : public QThread {
    Q_OBJECT
public:
    SimulationThread(const mjModel* model, mjData* data)
        : model_(model), data_(data), running_(true) {}
    void run() override {
        while (running_) mj_step(model_, data_);
    }
    void stop() {
        running_ = false;
        wait();
    }
private:
    const mjModel* model_;
    mjData* data_;
    std::atomic<bool> running_;
};

}
