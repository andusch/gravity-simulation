#version 330 core
out vec4 FragColor;

in vec3 fragPos;
in vec3 Normal;

uniform vec3 objectColor;
uniform vec3 lightPos;
uniform bool isEmitter;

void main() {
    
    if (isEmitter) {
        FragColor = vec4(objectColor, 1.0);
    } else {
        // ambient lightning settings
        float ambientStrength = 0.15;
        vec3 ambient = ambientStrength * objectColor;

        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - fragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * objectColor;

        FragColor = vec4(ambient + diffuse, 1.0);
    }

}