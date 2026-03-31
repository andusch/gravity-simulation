#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 fragPos;
out vec3 Normal;

uniform mat4 model;     
uniform mat4 view;
uniform mat4 projection;

void main() {
    
    // calculate the position of the vertex in "World Space"
    fragPos = vec3(model * vec4(aPos, 1.0));

    // pass the normal to the fragment shader
    Normal = mat3(model) * aNormal;

    gl_Position = projection * view * vec4(fragPos, 1.0);

}