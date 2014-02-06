#include <assert.h>

#include <iostream>
#include <string>
#include <algorithm>
#include <stdexcept>

#include "opengl_window.h"
#include "triangle_window.h"
#include "mola_data.hpp"

#include <QScreen>
#include <QApplication>
#include <iostream>

TextureWindow::TextureWindow() 
    : program_(0)
    , frame_(0)
{
}

GLuint TextureWindow::loadShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    return shader;
}

void TextureWindow::initialize() {
    program_ = new QOpenGLShaderProgram(this);
        std::cout << QCoreApplication::applicationDirPath().toStdString() << std::endl;
    std::cout << (const char*)glGetString(GL_VERSION) << std::endl;
    MOLA::data<float> north_data(
	(QCoreApplication::applicationDirPath() + "/../Resources/megr_n_512.img").toStdString(),
	12288);
    program_->addShaderFromSourceFile(
	QOpenGLShader::Vertex, 
	QCoreApplication::applicationDirPath() + "/../Resources/texture.vsh");
    program_->addShaderFromSourceFile(
	QOpenGLShader::Fragment, 
	QCoreApplication::applicationDirPath() + "/../Resources/texture.fsh");
    program_->link();
    position_ = m_program->attributeLocation("position");
    source_color_ = m_program->attributeLocation("source_color");
    tex_coord_input_ = m_program->attributeLocation("tex_coord_input");
    projection_ = program_->uniformLocation("projection");
    matrix_model_view_ = program_->uniformLocation("model_view");
    destination_color_ = program_->varyingLocation("destination_color");
    tex_coord_output_ = program_->varyingLocation("tex_coord_output");
}

void TextureWindow::render() {
    // for retina display
    float pixel_ratio = this->devicePixelRatio();
    glViewport(0, 0, width() * pixel_ratio, height() * pixel_ratio);
    glClear(GL_COLOR_BUFFER_BIT);
    program_->bind();
    QMatrix4x4 matrix_projection;
    matrix_projection.perspective(60, width() / height() , 0.1, 100.0);
    QMatrix4x4 matrix_model_view;
    matrix_model_view.setToIdentity();
    matrix_model_view.translate(0, 0, -2);
    matrix_model_view.rotate(100.0f * frame_ / screen()->refreshRate(), 0, 1, 0);
    program_->setUniformValue(projection_, matrix_projection);
    program_->setUniformValue(model_view_, matrix_model_view);
}
