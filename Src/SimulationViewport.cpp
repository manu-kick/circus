#include "SimulationViewport.h"
#include <memory>
#include <mujoco/mjvisualize.h>
#include <qpoint.h>

namespace spqr {

SimulationViewport::SimulationViewport(MujocoContext& mujContext)
    : model(mujContext.model), data(mujContext.data), camField(&mujContext.camField), camRobot(&mujContext.camRobot), opt(&mujContext.opt), scene(&mujContext.scene) {
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

/*
    camera:
        type: "realsense" # realsense or zed
        intrin:
            fx: 643.898
            fy: 643.216
            cx: 649.038
            cy: 357.21
            distortion_coeffs: [-0.0553056,0.065975,-0.000994232,2.98548e-05,-0.0216579]
            distortion_model: 2 # 0: none, 1: opencv format, 2: for rs d455 only
        extrin:
            - [ 0.05659255, 0.03014561, 0.99794215, 0.04217854]
            - [-0.99839673, 0.00283376, 0.05653273, 0.01405556]
            - [-0.00112372,-0.9995415 , 0.03025765,-0.01538294]
            - [ 0.        , 0.        , 0.        , 1.        ]
        pitch_compensation: 0.0 # in degree
        yaw_compensation: 0.0
        z_compensation: 0.0

    camera_type_ = node["camera"]["type"].as<std::string>();
        intr_ = Intrinsics(node["camera"]["intrin"]);
        p_eye2head_ = as_or<Pose>(node["camera"]["extrin"], Pose());

        float pitch_comp = as_or<float>(node["camera"]["pitch_compensation"], 0.0);
        float yaw_comp = as_or<float>(node["camera"]["yaw_compensation"], 0.0);

        p_headprime2head_ = Pose(0, 0, 0, 0, pitch_comp * M_PI / 180, yaw_comp * M_PI / 180);
*/

void SimulationViewport::paintGL() {
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mjv_updateScene(model, data, opt, nullptr, camField, mjCAT_ALL, scene);
    mjrRect viewport = {0, 0, width, height};
    mjr_setBuffer(mjFB_WINDOW, &context);
    mjr_render(viewport, scene, &context);

    // picture in picture for the robot camera
    int pipWidth = int(0.28 * width);
    int pipHeight = int(pipWidth * 9.0 / 16.0); // 16:9 aspect ratio
    pipHeight = std::min(pipHeight, height/2);
    mjv_updateScene(model, data, opt, nullptr, camRobot, mjCAT_ALL, scene);
    mjrRect pip{width - pipWidth, height - pipHeight, pipWidth, pipHeight}; // top-right
    mjr_render(pip, scene, &context);
}

void SimulationViewport::wheelEvent(QWheelEvent* event) {
    mjv_moveCamera(model, mjMOUSE_ZOOM, 0, -0.0005 * event->angleDelta().y(), scene, camField);
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

    mjv_moveCamera(model, mouseAction, 0.003*delta.x(), 0.003*delta.y(), scene, camField);

    lastMousePosition = event->position();
    update();
}

}
