#include "Crosshair.h"
#include <glad/glad.h>
#include <vector>

Crosshair::Crosshair(Texture& t)
    : UIPanel(t)
{
    vao.Bind();
    texture.Bind();
    // Screen center

    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;

    // Crosshair size on screen
    float size = 32.0f;  // 32x32 pixels on screen
    
    // UV coordinates - try the FULL icon (0,0) to (16,16)
    float atlasSize = 256.0f;
    float iconSize = 16.0f;
    
    float u1 = 0.0f / atlasSize;
    float v1 = 0.0f / atlasSize;
    float u2 = iconSize / atlasSize;
    float v2 = iconSize / atlasSize;

    vertices.insert(vertices.end(), {
        {{centerX - size/2, centerY - size/2}, {u1, v1}},
        {{centerX + size/2, centerY - size/2}, {u2, v1}},
        {{centerX - size/2, centerY + size/2}, {u1, v2}},
        {{centerX + size/2, centerY + size/2}, {u2, v2}}
    });

    indices.insert(indices.end(), {
        0, 1, 2,
        2, 1, 3
    });

    vbo.Bind();
    vbo.Upload(vertices.data(), vertices.size() * sizeof(UIVertex));

    ebo.Bind();
    ebo.Upload(indices.data(), indices.size() * sizeof(indices));
    
    vao.LinkAttrib(vbo, 0, 2, GL_FLOAT, 4*sizeof(float), (void*)0);
    vao.LinkAttrib(vbo, 1, 2, GL_FLOAT, 4*sizeof(float), (void*)(2*sizeof(float)));

    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();
}