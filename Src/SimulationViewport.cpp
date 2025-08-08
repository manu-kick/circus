#include "SimulationViewport.h"
#include <memory>
#include <mujoco/mjvisualize.h>
#include <qpoint.h>

namespace spqr {

SimulationViewport::SimulationViewport(MujocoContext& mujContext)
    : model(mujContext.model), data(mujContext.data), cam(&mujContext.cam), opt(&mujContext.opt), scene(&mujContext.scene) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&SimulationViewport::update));
    timer->start(16);
}

void SimulationViewport::initializeGL() {
    mjr_defaultContext(&context);
    mjr_makeContext(model, &context, mjFONTSCALE_100);   
}

void SimulationViewport::resizeGL(int w, int h) {  
    const float frameBufferFactor = devicePixelRatioF();
    width = int(w * frameBufferFactor);
    height = int(h * frameBufferFactor);
}

void SimulationViewport::paintGL() {
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mjv_updateScene(model, data, opt, nullptr, cam, mjCAT_ALL, scene);
    mjrRect viewport = {0, 0, width, height};
    mjr_setBuffer(mjFB_WINDOW, &context);
    mjr_render(viewport, scene, &context);
}

void SimulationViewport::wheelEvent(QWheelEvent* event) {
    mjv_moveCamera(model, mjMOUSE_ZOOM, 0, -0.0005 * event->angleDelta().y(), scene, cam);
}

void SimulationViewport::mousePressEvent(QMouseEvent* event) {
    lastMousePosition = event->position();

    if (event->button() == Qt::LeftButton) {
        if (event->modifiers() & Qt::ShiftModifier) {
            mouseAction = mjMOUSE_ROTATE_V;
        } else {
            mouseAction = mjMOUSE_MOVE_H;
        }
    }
}

void SimulationViewport::mouseReleaseEvent(QMouseEvent* event) {
    Q_UNUSED(event);
    mouseAction = mjMOUSE_NONE;
}

void SimulationViewport::mouseMoveEvent(QMouseEvent* event) {
    if (mouseAction == mjMOUSE_NONE)
        return;

    const QPointF delta = event->position() - lastMousePosition;

    mjv_moveCamera(model, mouseAction, 0.003*delta.x(), 0.003*delta.y(), scene, cam);

    lastMousePosition = event->position();
    update();
}

}
