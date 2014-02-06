attribute vec4 posAttr;
attribute vec4 colAttr;
uniform mat4 matrix;
varying vec4 col;

void main(void)
{
    col = colAttr;
    gl_Position = matrix * posAttr;
}
