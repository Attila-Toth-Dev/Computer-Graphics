// a simple shader
#version 410

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
uniform mat4 ProjectionViewModel;

out vec4 vPosition;
out vec3 vNormal;

uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;

void main()
{
    vPosition = ModelMatrix * Position;
    vNormal = NormalMatrix * Normal.xyz;
    gl_Position = ProjectionViewModel * Position;
}