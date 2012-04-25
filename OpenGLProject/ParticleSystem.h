#pragma once

class CLManager;
class Shader;
class GameManager;

class ParticleSystem
{
public:
    ParticleSystem();
    ~ParticleSystem();
    void render();

private:
    void initVBO();
private:

    CLManager* clManager;
    GameManager* gm;

    cl::Program program;
    cl::Kernel particleKernel;

    cl::BufferGL clBuf;
    GLuint glBuf;
    GLuint vao;

    cl::Image2DGL clTex;

    unsigned int dim;

    Shader* shader;
};

