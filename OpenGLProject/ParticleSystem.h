#pragma once

class CLManager;
class Shader;
class GameManager;

struct ParticleSim
{
    glm::vec4 speed;
};

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
    cl::BufferGL renderBuffer;
    cl::Buffer simBuffer;
    GLuint glBuf;
    GLuint vao;
    cl::Image2DGL clTex;

    unsigned int dim;

    Shader* shader;
};

