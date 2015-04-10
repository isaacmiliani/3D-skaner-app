#version 330 core

// Light information
uniform vec4 lightPosition;
uniform vec3 lightIntensity;

// Material information
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float shininess;

in vec3 normal;
in vec3 position;

layout (location = 0) out vec4 fragColor;

vec3 adsModel(const in vec3 norm)
{
    // Calculate light direction
    vec3 s = normalize(lightPosition.xyz - position);

    // Calculate the vector from the fragment to eye position
    vec3 v = normalize(-position.xyz);

    // Reflect the light using the normal
    vec3 r = reflect(-s, norm);

    // Calculate the diffuse contribution
    vec3 diffuseIntensity = vec3(max(dot(s, norm), 0.0));

    // Calculate specular contribution
    vec3 specularIntensity = vec3(0.0);
    if (dot(s, norm) > 0.0)
        specularIntensity = vec3(pow(max(dot(r, v), 0.0), shininess));

    // Calculate final color
    return lightIntensity * (Ka + Kd * diffuseIntensity + Ks * specularIntensity);
}

void main()
{
    fragColor = vec4(adsModel(normalize(normal)), 1.0);
}

