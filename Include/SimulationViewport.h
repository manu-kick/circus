#pragma once

#include <QOpenGLWindow>
#include <mujoco/mujoco.h>
#include <QTimer>
#include <QWheelEvent>
#include <qpoint.h>
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
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    QPointF lastMousePosition;
    mjtMouse mouseAction = mjMOUSE_NONE;

    mjModel* model;
    mjData* data;
    mjvCamera* camField;
    mjvCamera* leftCam;
    mjvCamera* rightCam;
    mjvOption* opt;
    mjvScene* scene;
    mjrContext context;
    QTimer* timer;
    
    bool isK1;
    int width = 640, height = 480;
};

}
