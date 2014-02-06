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

#include <QScreen>
#include <QApplication>
#include <iostream>

TriangleWindow::TriangleWindow()
    : m_program(0)
    , m_vertex_buffer(0)
    , m_frame(0)
{
}

void TriangleWindow::initialize()
{
    m_program = new QOpenGLShaderProgram(this);
    m_vertex_buffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    std::cout << QCoreApplication::applicationDirPath().toStdString() << std::endl;
    std::cout << (const char*)glGetString(GL_VERSION) << std::endl;
#ifdef Q_OS_MAC
    QString resourcePath = QCoreApplication::applicationDirPath() + "/../Resources/";
#else
    QString resourcePath = ":/";
#endif
    if (!m_program->addShaderFromSourceFile(
        QOpenGLShader::Vertex,
        resourcePath + "triangle.vsh"))
    {
        std::cerr << m_program->log().toStdString() << std::endl;
        throw std::runtime_error("Error in compiling vertex shader!");
    }
    if (!m_program->addShaderFromSourceFile(
        QOpenGLShader::Fragment,
        resourcePath + "triangle.fsh"))
    {
        std::cerr << m_program->log().toStdString() << std::endl;
        throw std::runtime_error("Error in compiling fragment shader!");
    }
    if (!m_program->link()) {
        std::cerr << m_program->log().toStdString() << std::endl;
        throw std::runtime_error("Error linking program shader!");
    }

    float points[] = {
        -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f,
        0.0f,  0.5f, 0.0f, 1.0f
    };
    m_vertex_buffer->create();
    m_vertex_buffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    if (!m_vertex_buffer->bind()) {
        throw std::runtime_error("Error could not bind vertex buffer to Context!");
    }
    m_vertex_buffer->allocate(points, 3 * 4 * sizeof(float));
    m_program->setAttributeBuffer("vertex", GL_FLOAT, 0, 4);
    m_program->enableAttributeArray("vertex");
}

void TriangleWindow::render()
{
    // for retina display
    float pixel_ratio = this->devicePixelRatio();
    glViewport(0, 0, width() * pixel_ratio, height() * pixel_ratio);

    if (!m_program->bind()) {
        std::cerr << m_program->log().toStdString() << std::endl;
        throw std::runtime_error("Error could not bind shader!");
    }

    glDrawArrays(GL_TRIANGLES, 0, 3);

    m_program->release();

    ++m_frame;
}
