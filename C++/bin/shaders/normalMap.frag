// a normal map phong style shader
#version 410

out vec4 FragColor;

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vTangent;
in vec3 vBiTangent;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

// For the light source
uniform vec3 ambientLight;  // Also seen as Ia
uniform vec3 diffuseLight;  // Also seen as Id
//uniform vec3 specularLight; // Also seen as Is

uniform vec3 LightDirection;

// Model Material Colours
uniform vec3  Ka; // ambient material colour
uniform vec3  Kd; // diffuse material colour
uniform vec3  Ks; // specular material colour
uniform float Ns; // Specular Power

uniform vec4 CameraPosition;

const int MAX_LIGHTS = 4;
uniform int NumberOfLights;
uniform vec3 PointLightColours[MAX_LIGHTS];
uniform vec3 PointLightPositions[MAX_LIGHTS];

vec3 Diffuse(vec3 direction, vec3 color, vec3 normal)
{
    return color * max(0, dot(normal, -direction));
}

vec3 Specular(vec3 direction, vec3 color, vec3 normal, vec3 view)
{
    // Calculate the Reflection vector
    vec3 R = reflect(direction, normal);
    float specularTerm = pow(max(0, dot(R, view)), Ns);
    return specularTerm * color;
}

void main()
{
    vec3 N = normalize(vNormal);
    vec3 T = normalize(vTangent);
    vec3 B = normalize(vBiTangent);
    vec3 L = normalize(LightDirection);

    mat3 TBN = mat3(T, B, N);

    vec3 texDiffuse = texture(diffuseTexture, vTexCoord).rgb;
    vec3 texSpecular = texture(specularTexture, vTexCoord).rgb;
    vec3 texNormal = texture(normalTexture, vTexCoord).rgb;

    N = normalize(TBN * (texNormal * 2 - 1));

    // Calculate the lambert term (this is the
    // negative light direction)
    float lambertTerm = max(0, min(1, dot(N, -L)));

    // Calculate the relfection vector
    vec3 V = normalize(CameraPosition.xyz - vPosition.xyz);

    // Calculate the reflection vector 
    vec3 R = reflect(L, N);

    // float specularTerm = pow(max(0, dot(R, V)), Ns);

    vec3 specularTotal = Specular(L, diffuseLight, N, V);
    vec3 diffuseTotal = Diffuse(L, diffuseLight, N);

    for(int i = 0; i < NumberOfLights; i++)
    {
        vec3 direction = vPosition.xyz - PointLightPositions[i];
        float lightDistance = length(direction);
        direction = direction / lightDistance;
    
        // Use the inverse square law to set the intensity of the lights
        vec3 color = PointLightColours[i] / (lightDistance * 2);

        diffuseTotal += Diffuse(direction, color, N);
        specularTotal += Specular(direction, color, N, V);
    }

    vec3 ambient = ambientLight * Ka * texDiffuse;
    vec3 diffuse = Kd * texDiffuse * diffuseTotal;
    vec3 specular = Ks * texSpecular * specularTotal;

    FragColor = vec4(ambient + diffuse + specular, 1);
}