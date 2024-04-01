// a normal map phong style shader
#version 410

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
layout(location = 2) in vec2 TexCoord;
layout(location = 3) in vec4 Tangent;
uniform mat4 ProjectionViewModel;

out vec4 vPosition;
out vec3 vNormal;
out vec2 vTexCoord;
out vec3 vTangent;
out vec3 vBiTangent;

// We wan the model's matrix seperately
uniform mat4 ModelMatrix;

// This will provide the normals of the mesh
uniform mat3 NormalMatrix;

void main()
{
    vPosition = ModelMatrix * Position;
    vNormal = (ModelMatrix * Normal).xyz;
    vTexCoord = TexCoord;
    vTangent = (ModelMatrix * Tangent).xyz;
    vBiTangent = cross(vNormal, vTangent) * Tangent.w;

    gl_Position = ProjectionViewModel * Position;
}