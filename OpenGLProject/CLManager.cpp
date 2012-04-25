#include "StdAfx.h"

#include "CLManager.h"

CLManager* CLManager::instance = nullptr;
CLManager* CLManager::getInstance()
{
    if ( !instance )
    {
        instance = new CLManager();
    }
    return instance;
}

CLManager::CLManager()
{
    init();
}

CLManager::~CLManager()
{
}

void CLManager::init()
{
    cl_int err = CL_SUCCESS;
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    if (platforms.size() == 0) 
    {
        std::cout << "Platform size 0\n";
        return;
    }

    // assuming win32 and OpenGL interop support 
    cl_context_properties props[] = 
    {
        CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(), 
        CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(), 
        CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[0])(), 
        0
    };
    context = cl::Context(CL_DEVICE_TYPE_GPU, props, nullptr, nullptr, &err);
    CLErr(err);

    devices = context.getInfo<CL_CONTEXT_DEVICES>();
}

cl::Program CLManager::build(std::string path)
{
    std::fstream fileIn(path.c_str(), std::fstream::in | std::fstream::binary);
    if (fileIn.fail())
        throw std::runtime_error("Unable to open kernel file\n");
    fileIn.seekg(0, std::ios::end);
    size_t len = (size_t)fileIn.tellg();
    fileIn.seekg(0, std::ios::beg);
    char* sourceStr = new char[len];
    fileIn.read(sourceStr, len);
    if  (fileIn.fail())
        throw std::runtime_error("Error when reading kernel file\n");
    fileIn.close();
    cl_int err = CL_SUCCESS;
    cl::Program::Sources source(1, std::make_pair(sourceStr,len));
    cl::Program program = cl::Program(context, source);
    cl_int buildRes = program.build(devices);
    if ( buildRes == CL_BUILD_PROGRAM_FAILURE )
        throw std::runtime_error(  path + " failed:\n" + program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]).c_str());    
    else
        CLErr(buildRes);
    queue = cl::CommandQueue(context, devices[0], 0, &err);
    CLErr(err);
    return program;
}
