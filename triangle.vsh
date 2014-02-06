#version 140
in vec2 posAttr;
in vec3 colAttr;
uniform mat4 matrix;
out vec3 col;

void main(void)
{
    col = colAttr;
    gl_Position = matrix * vec4(posAttr, 0.0, 1.0);
}
