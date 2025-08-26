#include "SimulationViewport.h"
#include "Robot.h"
#include <mujoco/mjvisualize.h>
#include <qnamespace.h>
#include <qpoint.h>
#include <iostream>

namespace spqr {

SimulationViewport::SimulationViewport(MujocoContext& mujContext)
    : model(mujContext.model), data(mujContext.data), cam(&mujContext.cam), opt(&mujContext.opt), scene(&mujContext.scene), robotManager(mujContext.robotManager) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&SimulationViewport::update));
    timer->start(16);
    mjv_defaultPerturb(&pert);
}

void SimulationViewport::initializeGL() {
    mjr_defaultContext(&context);
    mjr_makeContext(model, &context, mjFONTSCALE_100);   
}

void SimulationViewport::resizeGL(int w, int h) {  
    const float frameBufferFactor = devicePixelRatioF();
    logicalWidth = w;
    logicalHeight = h;
    width = int(w * frameBufferFactor);
    height = int(h * frameBufferFactor);
}

void SimulationViewport::paintGL() {
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mjv_updateScene(model, data, opt, &pert, cam, mjCAT_ALL, scene);

    if (highlightedBody >= 0) {
        robotManager.highlightRobot(highlightedBody, scene);
    }

    mjrRect viewport = {0, 0, width, height};
    mjr_setBuffer(mjFB_WINDOW, &context);
    mjr_render(viewport, scene, &context);
}

void SimulationViewport::wheelEvent(QWheelEvent* event) {
    mjv_moveCamera(model, mjMOUSE_ZOOM, 0, -0.0005 * event->angleDelta().y(), scene, cam);
}

void SimulationViewport::mousePressEvent(QMouseEvent* event) {
    lastMousePosition = event->position();
    highlightedBody = -1;
    
    if (event->button() == Qt::LeftButton) {

        float relx = event->position().x() / logicalWidth;
        float rely = 1.0 - event->position().y() / logicalHeight; // Flip Y for MuJoCo
        highlightedBody = selectBody(relx, rely);

        if (highlightedBody >= 0) {
            pert.select = highlightedBody;
            mjv_initPerturb(model, data, scene, &pert);
            
            if (event->modifiers() & Qt::ShiftModifier) {
                pert.active = mjPERT_ROTATE;
                mouseAction = mjMOUSE_ROTATE_V; // use rotate mouse action when moving
            } else {
                pert.active = mjPERT_TRANSLATE;
                mouseAction = mjMOUSE_MOVE_H; // use horizontal-plane move when moving
            }
        }

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
    highlightedBody = -1;
}

void SimulationViewport::mouseMoveEvent(QMouseEvent* event) {
    if (mouseAction == mjMOUSE_NONE)
        return;

    const QPointF delta = event->position() - lastMousePosition;

    if (pert.select > 0 && pert.active) {
        mjtNum reldx = (mjtNum)(delta.x() / (float)logicalHeight);
        mjtNum reldy = (mjtNum)(delta.y() / (float)logicalHeight); // note sign
        mjv_movePerturb(model, data, mouseAction, reldx, reldy, scene, &pert);
        mjv_applyPerturbPose(model, data, &pert, /*flg_paused=*/1);
    } else {
        mjv_moveCamera(model, mouseAction, 0.003*delta.x(), 0.003*delta.y(), scene, cam);
    }

    lastMousePosition = event->position();
}

int SimulationViewport::selectBody(float relx, float rely) const {
    if (!model || !data || !opt || !scene || !cam || height == 0)
        return -1;

    mjtNum selpnt[3];
    int geomid=-1, flexid=-1, skinid=-1;
    mjtNum aspect = (mjtNum)width / (mjtNum)height;
    int bodyid = mjv_select(
        model,
        data,
        opt,
        aspect,
        (mjtNum)relx,
        (mjtNum)rely,
        scene,
        selpnt,
        &geomid,
        &flexid,
        &skinid
    );

    std::cout << "Ray cast results:" << std::endl;
    std::cout << "  relx=" << relx << ", rely=" << rely << std::endl;
    
    if (bodyid >= 0) {
        const char* bodyName = mj_id2name(model, mjOBJ_BODY, bodyid);
        const char* geomName = (geomid >= 0) ? mj_id2name(model, mjOBJ_GEOM, geomid) : nullptr;
        
        std::cout << "  HIT: bodyid=" << bodyid << " (" << (bodyName ? bodyName : "unnamed") << ")" << std::endl;
        std::cout << "       geomid=" << geomid << " (" << (geomName ? geomName : "unnamed") << ")" << std::endl;
        std::cout << "       world hit point=(" << selpnt[0] << ", " << selpnt[1] << ", " << selpnt[2] << ")" << std::endl;
        
    } else {
        std::cout << "  MISS: no geometry hit" << std::endl;
    }

    return bodyid;
}

}
