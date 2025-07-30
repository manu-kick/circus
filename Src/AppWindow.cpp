
#include "AppWindow.h"
#include "MujocoContext.h"
#include <QVBoxLayout>
#include <memory>

namespace spqr {

    AppWindow::AppWindow(int& argc, char** argv) {
        const std::string tmpFile = std::string(PROJECT_ROOT) + "/Resources/scene_real.xml";
        mujContext = std::make_unique<MujocoContext>(tmpFile);

        viewport = std::make_unique<SimulationViewport>(*mujContext);
        QWidget* container = QWidget::createWindowContainer(viewport.get());

        QVBoxLayout* layout = new QVBoxLayout;
        layout->addWidget(container);

        QWidget* central = new QWidget;
        central->setLayout(layout);
        setCentralWidget(central);
        resize(800, 600);

        sim = std::make_unique<SimulationThread>(mujContext->model, mujContext->data);
        sim->start();
    };

    AppWindow::~AppWindow(){
        sim->stop();
    }
}
