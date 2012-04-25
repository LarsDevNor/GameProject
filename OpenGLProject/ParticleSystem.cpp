#include "StdAfx.h"

#include "GameManager.h"
#include "Terrain.h"
#include "Camera.h"
#include "Shader.h"
#include "CLManager.h"
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() : dim(128<<1)
{
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glPointSize(5.0f);

    cl_int err = CL_SUCCESS;
    clManager = CLManager::getInstance();    
    gm = GameManager::getInstance();

    program = clManager->build("./kernels/particles.cl");

    particleKernel = cl::Kernel(program, "firstKernel", &err);
    CLErr(err);

    initVBO();

    shader = new Shader();
    shader->addStage("./shaders/particle.vert", "", GL_VERTEX_SHADER);
    shader->addStage("./shaders/particle.geom", "", GL_GEOMETRY_SHADER);
    shader->addStage("./shaders/particle.frag", "", GL_FRAGMENT_SHADER);
    shader->install();
}

void ParticleSystem::initVBO()
{
    cl_int err = CL_SUCCESS;

    size_t elementNumb = dim*dim;
    std::vector<ParticleVertex> particleBuf(elementNumb);

    float spacing = 0.3f;
    for (size_t i = 0; i < dim; ++i)
    for (size_t j = 0; j < dim; ++j)
    {
        particleBuf[j+i*dim].color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        particleBuf[j+i*dim].pos = glm::vec4((float)i*spacing, 0.0, (float)j*spacing, 1.0);
    }

    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, glBuf);
    glBufferData(GL_ARRAY_BUFFER, elementNumb * sizeof(ParticleVertex), &particleBuf[0] , GL_STATIC_DRAW);

    unsigned int offset = 0;
    glVertexAttribPointer(semantic::attr::PARTICLE_POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), BUFFER_OFFSET(offset));
    offset += sizeof(glm::vec4);
    glVertexAttribPointer(semantic::attr::PARTICLE_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), BUFFER_OFFSET(offset));

    glEnableVertexAttribArray(semantic::attr::PARTICLE_POSITION);
    glEnableVertexAttribArray(semantic::attr::PARTICLE_COLOR);

    clBuf = cl::BufferGL(clManager->context, CL_MEM_READ_WRITE, glBuf, &err);
    CLErr(err);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
} 

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::render() 
{
    // Create CL Texture Object from terrain heightmap texture, put it in the terrain class. pass it as an arg to kernel
    // first try adjusting height of particles relative to terrain height
    //clTex = cl::Image2DGL(clManager->context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, /*GL TEX OBJ*/0, &err);
    //CLErr(err);

    // SIMULATE 

    cl_int err = CL_SUCCESS;
    
    std::vector<cl::Memory>* glVec = new std::vector<cl::Memory>();
    glVec->push_back(clBuf);
    cl::Image2DGL heightTex = gm->getTerrain()->getInputTex().getCL();
    glVec->push_back(heightTex);

    glFinish();
    clManager->queue.enqueueAcquireGLObjects(glVec, nullptr, nullptr);

    float terrainWidth  = gm->getTerrain()->getWidth();
    float terrainHeight = gm->getTerrain()->getHeight();
    particleKernel.setArg(0, sizeof(&clBuf), &clBuf); 
    particleKernel.setArg(1, sizeof(&heightTex), &heightTex);
    particleKernel.setArg(2, sizeof(float), &terrainWidth);
    particleKernel.setArg(3, sizeof(float), &terrainHeight);
    
    clManager->queue.enqueueNDRangeKernel(particleKernel, cl::NullRange, cl::NDRange(dim*dim,1), cl::NDRange((dim*dim<128)?dim*dim:128,1)); 
    
    clManager->queue.enqueueReleaseGLObjects(glVec, nullptr, nullptr);
    clManager->queue.finish();

    // RENDER 

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    shader->begin(); 
    {
        int viewLoc = shader->getUniLoc("view");
        int projLoc = shader->getUniLoc("proj");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(gm->getActiveCamera()->getViewMatrix()));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(gm->getActiveCamera()->getProjMatrix()));

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, glBuf);
        glDrawArrays(GL_POINTS, 0, dim*dim);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    } shader->end();
    glDisable(GL_BLEND);
}