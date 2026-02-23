#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in int aPartID;

uniform mat4 uPartMatrices[6];

out vec2 TexCoord;

void main()
{
    gl_Position = uPartMatrices[aPartID] * vec4(aPos, 1.0);
    TexCoord = aUV;
}