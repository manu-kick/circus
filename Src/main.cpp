
#include <QSurfaceFormat>
#include "CircusApplication.h"
#include "AppWindow.h"
#include "Constants.h"

#include "SceneParser.h"
#include <iostream>

int main(int argc, char** argv) {    
    spqr::CircusApplication app(argc, argv);
    app.setApplicationName(spqr::appName);

    spqr::SceneParser p("Resources/exp_scene.yaml");
    std::cout << p.buildMuJoCoXml() << std::endl;

    spqr::AppWindow window(argc, argv);
    window.show();

    return app.exec();
}