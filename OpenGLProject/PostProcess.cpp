#include "stdafx.h"

#include "GameManager.h"
#include "Shader.h"
#include "PostProcess.h"

PostProcess::PostProcess()
{
	resetGeometry();
}

void PostProcess::resetGeometry()
{ 
	std::vector<glm::vec2> quadVertices;

	glGenVertexArrays(1, &quadVAO);
	quadVertices.push_back(glm::vec2(-1.0f, -1.0f));
	quadVertices.push_back(glm::vec2(1.0f, -1.0f));
	quadVertices.push_back(glm::vec2(1.0f, 1.0f));
	quadVertices.push_back(glm::vec2(-1.0f, 1.0f));

	glGenBuffers(1, &quadBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quadBuffer);
	{
		glBufferData(GL_ARRAY_BUFFER, quadVertices.size() * sizeof(glm::vec2), &quadVertices[0], GL_STATIC_DRAW);
	}

	glBindVertexArray(quadVAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, quadBuffer);
		{
			glVertexAttribPointer(semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		}
		glEnableVertexAttribArray(semantic::attr::POSITION);
	}

	::flushGLError("PostProcess::initGeometry()");
}

////////////////// FOG 

PostProcessFog::PostProcessFog()
{
	initShaders();
	resetGeometry();
	initFBO();
}

void PostProcessFog::initFBO()
{
	glGenFramebuffers(1, &fbo);
	::flushGLError("PostProcessFog::initFBO()");
}

void PostProcessFog::initShaders()
{
	shader = new Shader();
	shader->addStage("./shaders/fog.vert", "", GL_VERTEX_SHADER);
	shader->addStage("./shaders/fog.frag", "", GL_FRAGMENT_SHADER);
	shader->install();

	::flushGLError("PostProcessFog::initShader()");
}

void PostProcessFog::run(GLuint colorTexIn, GLuint depthTexIn, GLuint rgbaTexOut)
{
	if(rgbaTexOut!=0) // attach rgba out if not rendering to screen 
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glBindTexture(GL_TEXTURE_2D, rgbaTexOut); // output to client color texture
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, rgbaTexOut, 0);
		if(!::checkFramebuffer(GL_FRAMEBUFFER, "PostProcessFog::run()"))
		{
			// throw or sth 
		}
	}
	// render to framebuffer with rgbaTexOut attached.. or something along those lines
	shader->begin();
	{
		{ 
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, colorTexIn);
			glUniform1i(shader->getUniLoc("colorSampler"), 0);
		}
		{ 
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, depthTexIn);
			glUniform1i(shader->getUniLoc("depthSampler"), 1);
		}
		glBindVertexArray(quadVAO);
		{
			glDrawArrays(GL_QUADS, 0, 4);
		} glBindVertexArray(0);
	} shader->end();

	::flushGLError("PostProcessFog::run()");
	
	if (rgbaTexOut!=0) // detach rgba out if not rendering to screen 
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

////////////////// SSAO 

PostProcessSSAO::PostProcessSSAO()
{
	initShaders();
	resetGeometry();
	initFBO();
}

void PostProcessSSAO::initFBO()
{
	glGenFramebuffers(1, &fbo);
	::flushGLError("PostProcessSSAO::initFBO()");
}

void PostProcessSSAO::initShaders()
{
	shader = new Shader();
	shader->addStage("./shaders/ssao.vert", "", GL_VERTEX_SHADER);
	shader->addStage("./shaders/ssao.frag", "", GL_FRAGMENT_SHADER);
	shader->install();

	::flushGLError("PostProcessSSAO::initShader()");
}

void PostProcessSSAO::run(GLuint colorTexIn, GLuint normalTexIn, GLuint posTexIn, GLuint depthTexIn, GLuint rgbaTexOut)
{
	if(rgbaTexOut!=0) // attach rgba out if not rendering to screen 
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glBindTexture(GL_TEXTURE_2D, rgbaTexOut); // output to client color texture
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, rgbaTexOut, 0);
		if(!::checkFramebuffer(GL_FRAMEBUFFER, "PostProcessSSAO::initFBO()"))
		{
			// throw or sth 
		}
	}
	// render to framebuffer with rgbaTexOut attached.. or something along those lines
	shader->begin();
	{
		{ 
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, colorTexIn);
			glUniform1i(shader->getUniLoc("colorSampler"), 0);
		}
		{ 
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, normalTexIn);
			glUniform1i(shader->getUniLoc("colorSampler"), 1);
		}
		{ 
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, posTexIn);
			glUniform1i(shader->getUniLoc("colorSampler"), 2);
		}
		{ 
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, depthTexIn);
			glUniform1i(shader->getUniLoc("depthSampler"), 3);
		}
		glBindVertexArray(quadVAO);
		{
			glDrawArrays(GL_QUADS, 0, 4);
		} glBindVertexArray(0);
	} shader->end();

	::flushGLError("PostProcessSSAO::run()");
	
	if (rgbaTexOut!=0) // detach rgba out if not rendering to screen 
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

PostProcessEdit::PostProcessEdit()
{
	resetGeometry();
	initShaders();
	initFBO();
}

//////////////////////////////// EDIT 

void PostProcessEdit::initFBO()
{
	glGenFramebuffers(1, &fbo);
	::flushGLError("PostProcessEdit::initFBO()");
}

void PostProcessEdit::initShaders()
{
	shader = new Shader();
	shader->addStage("./shaders/edit.vert", "", GL_VERTEX_SHADER);
	shader->addStage("./shaders/edit.frag", "", GL_FRAGMENT_SHADER);
	shader->install();

	::flushGLError("PostProcessEdit::initShader()");
}

void PostProcessEdit::run(glm::vec2 pos, EDIT_TYPE editType, GLuint texIn, GLuint texOut)
{
    

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glBindTexture(GL_TEXTURE_2D, texOut); 
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texOut, 0); // texture dimension determines size of fbo amirite ?
	if(!::checkFramebuffer(GL_FRAMEBUFFER, "PostProcessEdit::initFBO()"))
	{
	    					
	} 
    glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)));
    glViewport(0,0,1024,1024);
	shader->begin();
	{
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texIn);
            glUniform1i(shader->getUniLoc("textureSampler"), 0);
		}

        glUniform2fv(shader->getUniLoc("editPos"), 1, glm::value_ptr(pos));

		// TODO: push to parent
		glBindVertexArray(quadVAO);
		{
			glDrawArrays(GL_QUADS, 0, 4);
		} glBindVertexArray(0);
	} shader->end();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ::flushGLError("PostProcessEdit::run()");
    glm::ivec2 winSize = GameManager::getInstance()->getWindowDim();
    glViewport(0,0,winSize.x, winSize.y);
}

/************************************************************************/
/* Implement later                                                      */
/************************************************************************/

/* Implement 
class PostProcessSSAO : public PostProcess
{
public:

	PostProcessSSAO()
	{
		initShaders();
		resetGeometry();
		initFBO();
	}

	void initFBO()
	{
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		::flushGLError("PostProcessSSAO::initFBO()");
	}

	virtual void initShaders()
	{
		// TODO: install SSAO shader 
		//shader->addStage("./shaders/postproc.vert", "", GL_VERTEX_SHADER);
		//shader->addStage("./shaders/postproc.frag", "", GL_FRAGMENT_SHADER);
		//shader->install();

		::flushGLError("PostProcessSSAO::initShader()");
	}

	virtual void run(GLuint colorTexIn, GLuint depthTexIn, GLuint normalTexIn, GLuint rgbaTexOut)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		{
			glBindTexture(GL_TEXTURE_2D, rgbaTexOut); // output to client color texture
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, rgbaTexOut, 0);

			if(!::checkFramebuffer(GL_FRAMEBUFFER, "Terrain::initFBO()"))
			{
				// throw or sth 
			}
			// render to framebuffer with rgbaTexOut attached.. or something along those lines
			shader->begin();
			{
				{ 
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, colorTexIn);
					glUniform1i(shader->getUniLoc("colorSampler"), 0);
				}
				{ 
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, depthTexIn);
					glUniform1i(shader->getUniLoc("depthSampler"), 1);
				}
				{ 
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, normalTexIn);
					glUniform1i(shader->getUniLoc("normalSampler"), 1);
				}
				glBindVertexArray(quadVAO);
				{
					glDrawArrays(GL_QUADS, 0, 4);
				} glBindVertexArray(0);
			} shader->end();

			::flushGLError("PostProcessSSAO::run()");
		} glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	}

private:
	Shader* shader;
	GLuint colorTex;
	GLuint depthTex;
	GLuint normalTex;
	GLuint fbo;
};

class PostProcessSmooth : public PostProcess
{
public:
	virtual void initShaders()
	{
		shader = new Shader();
		shader->addStage("./shaders/postproc.vert", "", GL_VERTEX_SHADER);
		shader->addStage("./shaders/postproc.frag", "", GL_FRAGMENT_SHADER);
		shader->install();

		::flushGLError("PostProcess::initShader()");
	}
	virtual void run()
	{

	}
private:
	Shader* shader;
};
*/