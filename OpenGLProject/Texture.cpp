#include "StdAfx.h"
#include "Texture.h"

#include "CLManager.h"

Texture::Texture()
{
    created = false;
}

Texture::Texture(glm::ivec2 dim, GLint internalFormat, GLenum inputFormat, GLenum inputType, GLint filter, GLint wrap, const void* data, bool clInterop) :
    dim(dim), clInterop(clInterop)
{
    assert(dim.x > 0 && dim.y > 0);
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);	
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, dim.x, dim.y, 0, inputFormat, inputType, data);
    ::flushGLError("::Texture()");

    if ( clInterop )
    {
        cl_int err;
        CLManager* clManager = CLManager::getInstance();
        clTexId = cl::Image2DGL(clManager->context(), CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, texId, &err);
        CLErr(err);
    }

    created = true;
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, texId);
}

void Texture::unBind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    //if (created)
    //    glDeleteTextures(1, &texId);
}
