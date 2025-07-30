#include "SimulationViewport.h"
#include <memory>

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
    width = w;
    height = h;
}

void SimulationViewport::paintGL() {
    mjv_updateScene(model, data, opt, nullptr, cam, mjCAT_ALL, scene);
    mjrRect viewport = {0, 0, width, height};
    mjr_setBuffer(mjFB_WINDOW, &context);
    mjr_render(viewport, scene, &context);
}

void SimulationViewport::wheelEvent(QWheelEvent* event) {
    mjv_moveCamera(model, mjMOUSE_ZOOM, 0, -0.0005 * event->angleDelta().y(), scene, cam);
}

}
