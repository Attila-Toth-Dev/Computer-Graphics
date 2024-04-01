// a simple colourable shader
#version 410

uniform vec3 Kd;
out vec4 FragColor;

void main()
{
    FragColor = vec4(Kd, 1);
}