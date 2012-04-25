#pragma once

class Texture
{
public:
    Texture();
    Texture(glm::ivec2 dim, GLint internalFormat, GLenum inputFormat, GLenum inputType, GLint filter, GLint wrap, const void* data, bool clInterop = false);
    virtual ~Texture();

    void bind () const;
    void unBind () const;
    glm::ivec2 getDim() const { return dim; }    
    GLuint get() const { return texId; }
    const cl::Image2DGL& getCL() const { return clTexId; } 
private:
    glm::ivec2 dim;
    GLuint texId;
    bool created;
    bool clInterop;
    cl::Image2DGL clTexId;
};

