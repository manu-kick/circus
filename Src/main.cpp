
#include <QSurfaceFormat>
#include "CircusApplication.h"
#include "AppWindow.h"
#include "Constants.h"

#include "SceneParser.h"
#include <iostream>
#include <mujoco/mujoco.h>


int main(int argc, char** argv) {    
    spqr::CircusApplication app(argc, argv);
    app.setApplicationName(spqr::appName);

    spqr::AppWindow window(argc, argv);
    window.show();

    return app.exec();
}