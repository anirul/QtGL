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
    , m_position_buffer(0)
    , m_color_buffer(0)
    , m_vertex_array(0)
    , m_frame(0)
{
}

TriangleWindow::~TriangleWindow() {
    if (m_program) delete m_program;
    if (m_position_buffer) delete m_position_buffer;
    if (m_color_buffer) delete m_color_buffer;
    if (m_vertex_array) delete m_vertex_array;
}

void TriangleWindow::initialize()
{
    m_program = new QOpenGLShaderProgram(this);
    m_position_buffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_color_buffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_vertex_array = new QOpenGLVertexArrayObject(this);
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
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };
    float colors[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
    };

    m_position_buffer->create();
    m_position_buffer->bind();
    m_position_buffer->allocate(points, 9 * sizeof(float));
    
    m_color_buffer->create();
    m_color_buffer->bind();
    m_color_buffer->allocate(colors, 9 * sizeof(float));

    m_vertex_array->create();
    m_vertex_array->bind();

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, m_position_buffer->bufferId());
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_color_buffer->bufferId());
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

}

void TriangleWindow::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // for retina display
    float pixel_ratio = this->devicePixelRatio();
    glViewport(0, 0, width() * pixel_ratio, height() * pixel_ratio);

    if (!m_program->bind()) {
        std::cerr << m_program->log().toStdString() << std::endl;
        throw std::runtime_error("Error could not bind shader!");
    }
    m_vertex_array->bind();

    glDrawArrays(GL_TRIANGLES, 0, 3);

    m_vertex_array->release();
    m_program->release();

    ++m_frame;
}
