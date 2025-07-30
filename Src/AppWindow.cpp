
#include "AppWindow.h"
#include <QVBoxLayout>

namespace spqr {

    AppWindow::AppWindow(int& argc, char** argv) {
        char error[1024];
        const std::string tmpFile = std::string(PROJECT_ROOT) + "/Resources/scene_real.xml";
        model = mj_loadXML(tmpFile.c_str(), nullptr, error, 1024);
        if (!model) throw std::runtime_error(error);

        data = mj_makeData(model);
        mjv_defaultOption(&opt);
        mjv_defaultCamera(&cam);
        mjv_makeScene(model, &scene, 10000);
        scene.flags[mjRND_SHADOW] = false;
        scene.flags[mjRND_REFLECTION] = false;

        viewport = new SimulationViewport(model, data, &cam, &opt, &scene);
        QWidget* container = QWidget::createWindowContainer(viewport);

        QVBoxLayout* layout = new QVBoxLayout;
        layout->addWidget(container);

        QWidget* central = new QWidget;
        central->setLayout(layout);
        setCentralWidget(central);
        resize(800, 600);

        sim = new SimulationThread(model, data);
        sim->start();
    };

}
