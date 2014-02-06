#ifndef TRIANGLE_WINDOW_HEADER_DEFINED
#define TRIANGLE_WINDOW_HEADER_DEFINED

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class TriangleWindow : public OpenGLWindow
{
public:
    TriangleWindow();

    void initialize();
    void render();

private:
    QOpenGLShaderProgram* m_program;
    GLuint vaoHandle;
    int m_frame;
};

#endif // TRIANGLE_WINDOW_HEADER_DEFINED
