#pragma once

#include <QOpenGLWindow>
#include <mujoco/mujoco.h>
#include <QTimer>
#include <QWheelEvent>

namespace spqr {

class SimulationViewport : public QOpenGLWindow {
public:
    SimulationViewport(mjModel* model, mjData* data, mjvCamera* cam,
             mjvOption* opt, mjvScene* scene)
        : model(model), data(data), cam(cam), opt(opt), scene(scene) {
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, QOverload<>::of(&SimulationViewport::update));
        timer->start(16);
    }

protected:
    void initializeGL() override {
        mjr_defaultContext(&context);
        mjr_makeContext(model, &context, mjFONTSCALE_100);
    }

    void resizeGL(int w, int h) override {
        width = w;
        height = h;
    }

    void paintGL() override {
        mjv_updateScene(model, data, opt, nullptr, cam, mjCAT_ALL, scene);
        mjrRect viewport = {0, 0, width, height};
        mjr_setBuffer(mjFB_WINDOW, &context);
        mjr_render(viewport, scene, &context);
    }

    void wheelEvent(QWheelEvent* event) override {
        mjv_moveCamera(model, mjMOUSE_ZOOM, 0, -0.0005 * event->angleDelta().y(), scene, cam);
    }

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
