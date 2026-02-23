#include "ItemDecals.h"

ItemDecals::ItemDecals(Texture& itemAtlas)
    : UIPanel(itemAtlas)
{   
    vao.Bind();
    vbo.Bind();
    ebo.Bind();
    
    vao.LinkAttrib(vbo, 0, 2, GL_FLOAT, 4*sizeof(float), (void*)0);
    vao.LinkAttrib(vbo, 1, 2, GL_FLOAT, 4*sizeof(float), (void*)(2*sizeof(float)));

    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();
}

void ItemDecals::UploadDecalData() 
{
    vao.Bind();
    vbo.Bind();
    ebo.Bind();

    vbo.Upload(vertices.data(), vertices.size() * sizeof(UIVertex));
    ebo.Upload(indices.data(), indices.size() * sizeof(GLuint));

    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();

}