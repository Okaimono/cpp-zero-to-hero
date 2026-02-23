#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 camMatrix;  // Camera matrix (uploaded once)
uniform vec3 chunkOffset;     // Chunk position (uploaded per chunk)

out vec2 TexCoord;

void main() {
    // Translate vertex from local chunk space to world space
    vec3 worldPos = aPos + chunkOffset;
    
    // Transform to screen space
    gl_Position = camMatrix * vec4(worldPos, 1.0);
    
    TexCoord = aTexCoord;
}