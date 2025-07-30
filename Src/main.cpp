
#include <QSurfaceFormat>
#include "CircusApplication.h"
#include "AppWindow.h"

int main(int argc, char** argv) {    
    spqr::CircusApplication app(argc, argv);
    app.setApplicationName("Circus Simulator");

    spqr::AppWindow window(argc, argv);
    window.show();

    return app.exec();
}