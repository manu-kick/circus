#pragma once
#include <QMainWindow>
#include <mujoco/mujoco.h>
#include "SimulationViewport.h"
#include "SimulationThread.h"

namespace spqr {

class AppWindow : public QMainWindow
{
public:
  AppWindow(int& argc, char** argv);

private:
    mjModel* model;
    mjData* data;
    mjvCamera cam;
    mjvOption opt;
    mjvScene scene;
    SimulationViewport* viewport;
    SimulationThread* sim;
};

} 