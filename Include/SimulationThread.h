#pragma once

#include <mujoco/mujoco.h>
#include <QThread>
#include "MujocoContext.h"

namespace spqr {

class SimulationThread : public QThread {
    Q_OBJECT
public:
    SimulationThread(MujocoContext& context);
    void run() override;
    void stop();
private:
    MujocoContext& context_;
    std::atomic<bool> running_;
};

}
