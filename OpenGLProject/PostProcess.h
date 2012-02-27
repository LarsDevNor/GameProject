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

class PostProcessSSAO : public PostProcess
{
public:
	PostProcessSSAO ();
	virtual void initShaders();
	virtual void run(GLuint colorTexIn, GLuint normalTexIn, GLuint posTexIn, GLuint depthTexIn, GLuint rgbaTexOut=0);
protected:
	virtual void initFBO();
private:
	Shader* shader;
	GLuint depthTex;
	GLuint fbo;
};

// TODO: implement
class PostProcessEdit : PostProcess
{
public:
	enum EDIT_TYPE { UP, DOWN };
public:
	PostProcessEdit ();
	virtual void initShaders();
	virtual void run(glm::vec2 pos, EDIT_TYPE editType, GLuint texIn, GLuint texOut);
protected:
	virtual void initFBO();
private:
	EDIT_TYPE editType;
	Shader* shader;
	GLuint depthTex;
	GLuint fbo;
};
