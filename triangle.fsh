#version 410

in vec3 colorOut;
out vec4 fragColor;

void main(void)
{
    fragColor = vec4(colorOut, 1.0);
}
