#version 330 core
out vec4 FragColor;

in vec3 fragPos;
in vec3 Normal;

uniform vec3 objectColor;
uniform vec3 sunPos = vec3(0.0, 0.0, 0.0);

void main() {
    
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * objectColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(sunPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * objectColor;

    FragColor = vec4(ambient + diffuse, 1.0);

}