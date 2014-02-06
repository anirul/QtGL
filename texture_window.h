#ifndef TEXTURE_WINDOW_HEADER_DEFINED
#define TEXTURE_WINDOW_HEADER_DEFINED

#include <QOpenGLShaderProgram>

class TextureWindow : public OpenGLWindow
{
public :
    TextureWindow();
    void initialize();
    void render();

private :
    GLuint loadShader(GLenum type, const char* source);
    // attributes
    GLuint position_;
    GLuint source_color_;
    GLuint tex_coord_input_;
    // uniform
    GLuint projection_;
    GLuint model_view_;
    // varying
    GLuint destination_color_;
    GLuint tex_coord_output_;
    // shaders
    QOpenGLShaderProgram* program_;
    int frame_;
};

#endif // TEXTURE_WINDOW_HEADER_DEFINED
