#include "Core/MuJoCoWidget.h"
#include "Core/Renderer.h"


namespace spqr {

MuJoCoWidget::MuJoCoWidget(QWidget* parent) : QOpenGLWidget(parent) {}

void MuJoCoWidget::initializeGL() {}

void MuJoCoWidget::paintGL() {
    Renderer::render(nullptr);
}

void MuJoCoWidget::resizeGL(int w, int h) {}

}  // namespace spqr
