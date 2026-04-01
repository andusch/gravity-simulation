#version 330 core
out vec4 FragColor;

in vec3 fragPos;
in vec3 Normal;

uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool isEmitter;

void main() {
    
    if (isEmitter) {
        FragColor = vec4(objectColor, 1.0);
    } else {
        // 1. Ambient
        // ambient lightning settings
        float ambientStrength = 0.2;
        vec3 ambient = ambientStrength * objectColor;

        // 2. Diffuse
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - fragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * objectColor;

        // 3. Specular
        float specularStrength = 0.7;;
        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * vec3(1.0, 1.0, 1.0);

        FragColor = vec4(ambient + diffuse + specular, 1.0);
    }

}