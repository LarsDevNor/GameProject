#pragma once 

class Shader;

class PostProcess
{
public:
	PostProcess();
	virtual void resetGeometry();
	virtual void initShaders() = 0;
protected:
	virtual void initFBO() = 0;
protected:
	GLuint quadBuffer;
	GLuint quadVAO;
};

class PostProcessFog : public PostProcess
{
public:
	PostProcessFog ();
	virtual void initShaders();
	virtual void run(GLuint colorTexIn, GLuint depthTexIn, GLuint rgbaTexOut=0);
protected:
	virtual void initFBO();
private:
	Shader* shader;
	GLuint depthTex;
	GLuint fbo;
};
