#pragma once

#include <QOpenGLWindow>
#include <mujoco/mujoco.h>
#include <QTimer>
#include <QWheelEvent>
#include "MujocoContext.h"

namespace spqr {

class SimulationViewport : public QOpenGLWindow {
public:
    SimulationViewport(MujocoContext& mujContext);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void wheelEvent(QWheelEvent* event) override;

private:
    mjModel* model;
    mjData* data;
    mjvCamera* cam;
    mjvOption* opt;
    mjvScene* scene;
    mjrContext context;
    QTimer* timer;
    int width = 640, height = 480;
};

}
