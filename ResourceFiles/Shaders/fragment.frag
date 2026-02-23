#version 330 core
out vec4 FragColor;
in vec2 texCoord;
uniform sampler2D tex0;

void main()
{
    vec4 texColor = texture(tex0, texCoord);
    FragColor = texColor;  // Make sure you output ALL 4 components (RGBA)
    
    // Optional: discard fully transparent pixels
    if (texColor.a < 0.1)
        discard;
}