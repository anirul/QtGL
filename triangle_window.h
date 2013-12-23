#ifndef TRIANGLE_WINDOW_HEADER_DEFINED
#define TRIANGLE_WINDOW_HEADER_DEFINED

#include <QOpenGLShaderProgram>

class TriangleWindow : public OpenGLWindow
{
public:
    TriangleWindow();

    void initialize();
    void render();

private:
    GLuint loadShader(GLenum type, const char *source);

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

    QOpenGLShaderProgram *m_program;
    int m_frame;
};

#endif // TRIANGLE_WINDOW_HEADER_DEFINED
