#ifndef TRIANGLE_WINDOW_HEADER_DEFINED
#define TRIANGLE_WINDOW_HEADER_DEFINED

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class TriangleWindow : public OpenGLWindow
{
public:
    TriangleWindow();
    virtual ~TriangleWindow();
    void initialize();
    void render();

private:
    QOpenGLShaderProgram* m_program;
    QOpenGLBuffer* m_position_buffer;
    QOpenGLBuffer* m_color_buffer;
    QOpenGLVertexArrayObject* m_vertex_array;
    int m_frame;
};

#endif // TRIANGLE_WINDOW_HEADER_DEFINED
