#pragma once

#include <mujoco/mujoco.h>
#include <qpoint.h>

#include <QOpenGLWindow>
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
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

   private:
    QPointF lastMousePosition;
    mjtMouse mouseAction = mjMOUSE_NONE;

    mjModel* model;
    mjData* data;
    mjvCamera* cam;
    mjvOption* opt;
    mjvScene* scene;
    mjrContext context;
    QTimer* timer;
    mjvPerturb pert;
    RobotManager& robotManager;

    int selectBody(float relx, float rely) const;

    int width = 640, height = 480;
    int logicalWidth = 640, logicalHeight = 480;

    int highlightedBody = -1;
};

}  // namespace spqr
