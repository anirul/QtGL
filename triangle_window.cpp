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
}

GLuint TriangleWindow::loadShader(GLenum type, const char *source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    return shader;
}

void TriangleWindow::initialize()
{
    m_program = new QOpenGLShaderProgram(this);
    std::cout << QCoreApplication::applicationDirPath().toStdString() << std::endl;
    std::cout << (const char*)glGetString(GL_VERSION) << std::endl;
    m_program->addShaderFromSourceFile(
	QOpenGLShader::Vertex, 
	QCoreApplication::applicationDirPath() + "/../Resources/triangle.vsh");
    m_program->addShaderFromSourceFile(
	QOpenGLShader::Fragment, 
	QCoreApplication::applicationDirPath() + "/../Resources/triangle.fsh");
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");
}

void TriangleWindow::render()
{
    // for retina display
    float pixel_ratio = this->devicePixelRatio();
    glViewport(0, 0, width() * pixel_ratio, height() * pixel_ratio);

    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60, 4.0/3.0, 0.1, 100.0);
    matrix.translate(0, 0, -2);
    matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

    m_program->setUniformValue(m_matrixUniform, matrix);

    GLfloat vertices[] = {
        0.0f, 0.707f,
        -0.5f, -0.5f,
        0.5f, -0.5f
    };

    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    m_program->release();

    ++m_frame;
}
