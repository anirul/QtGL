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
    , m_frame(0)
{
    vaoHandle = 0;
}

void TriangleWindow::initialize()
{
    GLuint vboHandles[2];
    m_program = new QOpenGLShaderProgram(this);
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
    glGenBuffers(2, vboHandles);
    GLuint positionBufferHandle = vboHandles[0];
    GLuint colorBufferHandle = vboHandles[1];

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colors, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
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

    glBindVertexArray(vaoHandle);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    m_program->release();

    ++m_frame;
}
