#include "CircusApplication.h"

#include <QSurfaceFormat>

namespace spqr {
CircusApplication::CircusApplication(int& argc, char** argv) : QApplication(argc, argv) {
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setSamples(4);
    format.setSwapInterval(1);
    format.setVersion(2, 0);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setRenderableType(QSurfaceFormat::RenderableType::OpenGL);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    QSurfaceFormat::setDefaultFormat(format);
}
}  // namespace spqr
