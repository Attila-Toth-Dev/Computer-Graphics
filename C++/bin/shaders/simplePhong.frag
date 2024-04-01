// a simple shader
#version 410

out vec4 FragColor;

in vec3 vNormal;

uniform vec3 LightDirection;

void main()
{
    vec3 N = normalize(vNormal);
    vec3 L = normalize(LightDirection);

    // Calculate the lambert term (this is the
    // negative light direction)
    float lambertTerm = max(0, min(1, dot(N, -L)));

    FragColor = vec4(lambertTerm, lambertTerm, lambertTerm, 1);
}