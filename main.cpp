#include <assert.h>

#include <iostream>
#include <string>
#include <algorithm>
#include <stdexcept>
#ifdef WIN32
#include <cstdint>
#endif

#include "opengl_window.h"
#include "triangle_window.h"
#include "mola_data.hpp"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QSurfaceFormat format;
    format.setSamples(4);
    // OpenGL desktop (may not work under Windows)
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setMajorVersion(3);
    format.setMinorVersion(2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    
    TriangleWindow window;
    window.setFormat(format);
    window.resize(640, 480);
    window.show();
    window.setAnimating(true);
    return app.exec();
}
