#pragma once
#include <QApplication>
#include <QMainWindow>
#include <QOpenGLWidget>

namespace spqr {

class MuJoCoWidget : public QOpenGLWidget {
    Q_OBJECT
public:
    MuJoCoWidget(QWidget* parent = nullptr);
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
};

}