// a simple shader
#version 410

out vec4 FragColor;

in vec4 vPosition;
in vec3 vNormal;

// For the light source
uniform vec3 ambientLight;  // Also seen as Ia
uniform vec3 diffuseLight;  // Also seen as Id
uniform vec3 specularLight; // Also seen as Is

uniform vec3 LightDirection;

// Model Material colours
uniform vec3  Ka; // ambient material colour
uniform vec3  Kd; // diffuse material colour
uniform vec3  Ks; // specular material colour
uniform float Ns; // Specular power

uniform vec4 CameraPosition;

void main()
{
    vec3 N = normalize(vNormal);
    vec3 L = normalize(LightDirection);

    // Calculate the lambert term (this is the
    // negative light direction)
    float lambertTerm = max(0, min(1, dot(N, -L)));

    // Calculate the view vector
    vec3 V = normalize(CameraPosition.xyz - vPosition.xyz);
    
    // Calculate the reflection vector
    vec3 R = reflect(L, N);

    float specularTerm = pow(max(0, dot(R, V)), Ns);

    vec3 ambient = ambientLight * Ka;
    vec3 diffuse = diffuseLight * Kd * lambertTerm;
    vec3 specular = specularLight * Ks * specularTerm;

    FragColor = vec4(ambient + diffuse + specular, 1);
}