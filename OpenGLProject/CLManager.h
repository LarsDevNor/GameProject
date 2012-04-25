#pragma once

class CLManager
{
public:
    static CLManager* getInstance();
public:
    virtual ~CLManager();
    void init();
    cl::Program build(std::string path);
public:
    cl::Context context;
    cl::CommandQueue queue;

private:
    static CLManager* instance;
private:
    CLManager();
    std::vector<cl::Device> devices;
};

