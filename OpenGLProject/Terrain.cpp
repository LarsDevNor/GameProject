#include "stdafx.h"

#include "GameManager.h"
#include "PerlinNoise.h"
#include "Camera.h"
#include "Shader.h"
#include "Terrain.h"

/*
	ITEMS:

	1. SSAO: 
	2. generate terrain as we move 
	3. Clamp player to terrain in shader
	4. weapons
	5. Destructible terrain
	6. deferred renderer


*/

Terrain::Terrain() 
{
	gm = GameManager::getInstance();

	initShader();
	initGeometry();
	initTextures();
	initSamplers();
	initFBO();
}

void Terrain::initGeometry()
{
	nVertsHeight = 256;
	nVertsWidth = 256;
	width = 128.0f;
	height = 128.0f;
	pn = new PerlinNoise(0.5, 1.25f, 2500/(nVertsHeight+nVertsWidth), 8, 5);

	glm::vec2 spacing((float)width / nVertsWidth, (float)height / nVertsHeight);
	{ // create geometry 
		for ( size_t i = 0; i < nVertsHeight; ++i ) 
		for ( size_t j = 0; j < nVertsWidth; ++j )
		{
			float vertexHeight = -10.0f + (float)pn->GetHeight(j*0.1f, i*0.1f);
			heights.push_back(vertexHeight);
		}
		
		normals = std::vector<glm::vec3>(nVertsWidth*nVertsHeight);
		std::fill(normals.begin(), normals.end(), glm::vec3(0.0f, 0.0f, 0.0f));

		
#if 1 // calculate heightmap based normals 
		for ( size_t i = 1; i < nVertsHeight-1; ++i )
		for ( size_t j = 1; j < nVertsWidth-1; ++j )
		{
			glm::vec3 vecs[8];
			float h0 = heights[ j + i*nVertsWidth];
			vecs[0] = glm::vec3(  0.0f,      heights[j     + (i+1)*nVertsWidth]-h0,  spacing.y );
			vecs[1] = glm::vec3( -spacing.x, heights[(j-1) + (i+1)*nVertsWidth]-h0,  spacing.y );
			vecs[2] = glm::vec3( -spacing.x, heights[(j-1) +  i  *nVertsWidth]-h0,   0.0f	   );
			vecs[3] = glm::vec3( -spacing.x, heights[(j-1) + (i-1)*nVertsWidth]-h0, -spacing.y );
			vecs[4] = glm::vec3(  0.0f,      heights[j     + (i-1)*nVertsWidth]-h0, -spacing.y );
			vecs[5] = glm::vec3(  spacing.x, heights[(j+1) + (i-1)*nVertsWidth]-h0, -spacing.y );
			vecs[6] = glm::vec3(  spacing.x, heights[(j+1) +  i   *nVertsWidth]-h0,  0.0f      );
			vecs[7] = glm::vec3(  spacing.x, heights[(j+1) + (i+1)*nVertsWidth]-h0,  spacing.y );

			glm::vec3 normalsAcc(0.0f, 0.0,0.0f);
			for ( int k = 1; k < 8; ++k )
			{
				normalsAcc += glm::normalize(glm::cross(vecs[k-1], vecs[k]));
			}
			normals[j+i*nVertsWidth] = glm::normalize(normalsAcc / 8.0f);
		}
#else // calculate normals using heightmap gen function 
		glm::vec2 delta = 1.0f * spacing;
		for ( size_t i = 0; i < nVertsHeight; ++i )
		for ( size_t j = 0; j < nVertsWidth; ++j )
		{
			glm::vec3 hor = glm::vec3(delta.x*2.0, (float)pn->GetHeight((double)(j+delta.x), i) - (float)pn->GetHeight(j-delta.x, i), 0.0f);
			glm::vec3 vert = glm::vec3(0.0f, (float)pn->GetHeight(j, (double)(i-delta.y)) - (float)pn->GetHeight(j, i+delta.y), delta.y*2.0f);
			normals[j+i*nVertsWidth] = glm::normalize(glm::cross(hor, vert));
		}
#endif 

		const size_t smoothingIterations = 4;
		for ( size_t iteration = 0; iteration < smoothingIterations; ++iteration)
		for ( size_t i = 1; i < nVertsHeight-1; ++i )
		for ( size_t j = 1; j < nVertsWidth-1; ++j )
		{
			glm::vec3 adjNormals[8];
			adjNormals[0] = normals[j     + (i+1)*nVertsWidth];
			adjNormals[1] = normals[(j-1) + (i+1)*nVertsWidth];
			adjNormals[2] = normals[(j-1) +  i   *nVertsWidth];
			adjNormals[3] = normals[(j-1) + (i-1)*nVertsWidth];
			adjNormals[4] = normals[j     + (i-1)*nVertsWidth];
			adjNormals[5] = normals[(j+1) + (i-1)*nVertsWidth];
			adjNormals[6] = normals[(j+1) +  i   *nVertsWidth];
			adjNormals[7] = normals[(j+1) + (i+1)*nVertsWidth];

			glm::vec3 normalsAcc(0.0f, 0.0f, 0.0f);
			for ( int k = 1; k < 8; ++k )
			{
				normalsAcc += adjNormals[k];
			}
			normals[j+i*nVertsWidth] = glm::normalize( glm::normalize(normalsAcc) * 0.5f + normals[j+i*nVertsWidth] * 0.5f );
		}

		for ( size_t i = 0; i < nVertsHeight; ++i )
		for ( size_t j = 0; j < nVertsWidth; ++j )
		{
			glm::vec2 normPos((float)j/(nVertsWidth-1), (float)i/(nVertsHeight-1));
			glm::vec2 pos = glm::vec2(normPos.x * width, normPos.y * height);

			vertices.push_back(PNTVertex(glm::vec3(pos.x, heights[j+i*nVertsWidth], pos.y), normals[j+i*nVertsWidth], normPos));
		}
		
		for ( size_t i = 0; i < nVertsHeight-1; ++i )
		for ( size_t j = 0; j < nVertsWidth-1; ++j )
		{
			indices.push_back((j+1)	+ (i+1)*nVertsWidth);
			indices.push_back((j+1) + i*nVertsWidth);
			indices.push_back(j		+ i*nVertsWidth);

			indices.push_back((j)	+ (i+1)*nVertsWidth);
			indices.push_back((j+1) + (i+1)*nVertsWidth);
			indices.push_back(j		+ i*nVertsWidth);
		}
	}

	{ // bind geometry to buffers 
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ibo);
		glGenVertexArrays(1, &vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		{
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(PNTVertex), &vertices[0], GL_STATIC_DRAW);
		} glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		} glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(vao);
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			{
				unsigned int offset = 0;
				glVertexAttribPointer(semantic::attr::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(PNTVertex), BUFFER_OFFSET(offset));
				offset += sizeof(glm::vec3);
				glVertexAttribPointer(semantic::attr::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(PNTVertex), BUFFER_OFFSET(offset));
				offset += sizeof(glm::vec3);
				glVertexAttribPointer(semantic::attr::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(PNTVertex), BUFFER_OFFSET(offset));
			}
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glEnableVertexAttribArray(semantic::attr::POSITION);
			glEnableVertexAttribArray(semantic::attr::NORMAL);
			glEnableVertexAttribArray(semantic::attr::TEXCOORD);
		} glBindVertexArray(0);
	} 

	{ // quad fullscreen geometry 
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
	}

	::flushGLError("Terrain::initGeometry()");
}

void Terrain::initShader()
{
	shaderDefault = new Shader();
	shaderDefault->addStage("./shaders/terrain.vert", "", GL_VERTEX_SHADER);
	shaderDefault->addStage("./shaders/terrain.frag", "", GL_FRAGMENT_SHADER);
	shaderDefault->install();

	shaderPostproc = new Shader();
	shaderPostproc->addStage("./shaders/postproc.vert", "", GL_VERTEX_SHADER);
	shaderPostproc->addStage("./shaders/postproc.frag", "", GL_FRAGMENT_SHADER);
	shaderPostproc->install();

	::flushGLError("Terrain::initShader()");
}

void Terrain::initTextures()
{
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &colorTex);
	glGenTextures(1, &depthTex);

	{ // color rtt target
		glBindTexture(GL_TEXTURE_2D, colorTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, gm->getWindowDim().x, gm->getWindowDim().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	}
	{ // depth rtt target 
		glBindTexture(GL_TEXTURE_2D, depthTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, gm->getWindowDim().x, gm->getWindowDim().y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	}

	::flushGLError("Terrain::initTextures()");
}

void Terrain::initSamplers()
{
	glGenSamplers(1, &defaultSampler);
	glSamplerParameteri(defaultSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glSamplerParameteri(defaultSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glSamplerParameteri(defaultSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(defaultSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	::flushGLError("Terrain::initSamplers()");
}


void Terrain::initFBO()
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindTexture(GL_TEXTURE_2D, colorTex);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorTex, 0);
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTex, 0);

	//GLenum drawBuffers[2] = { GL_COLOR_ATTACHMENT0 };
	//glDrawBuffers(2, drawBuffers);
	
	if(::checkFramebuffer(GL_FRAMEBUFFER, "Terrain::initFBO()"))
	{
		// throw or sth 
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	::flushGLError("Terrain::initFBO()");
}

void Terrain::renderTerrain()
{
	shaderDefault->begin();
	{
		int viewLoc = shaderDefault->getUniLoc("view");
		int projLoc = shaderDefault->getUniLoc("proj");
		//int modelLoc = shaderDefault->getUniLoc("model");
		//int camPosLoc = shaderDefault->getUniLoc("camPos");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(gm->getActiveCamera()->getViewMatrix()));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(gm->getActiveCamera()->getProjMatrix()));
		//glUniform3fv(camPosLoc, 1, glm::value_ptr(-gm->getActiveCamera()->getEye()));
		glBindVertexArray(vao);
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		} glBindVertexArray(0);
	} shaderDefault->end();

	::flushGLError("Terrain::renderTerrain()");
}

void Terrain::renderFullscreenQuad()
{
	glDisable(GL_DEPTH_TEST);

	shaderPostproc->begin();
	{
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, depthTex);
			glUniform1i(shaderPostproc->getUniLoc("depthSampler"), 0);
		}
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, colorTex);
			glUniform1i(shaderPostproc->getUniLoc("colorSampler"), 1);
		}
		/*glBindSampler(0, defaultSampler);*/

		glBindVertexArray(quadVAO);
		{
			glDrawArrays(GL_QUADS, 0, 4);
		} glBindVertexArray(0);
	} shaderPostproc->end();

	::flushGLError("Terrain::renderFullscreenQuad()");
}

void Terrain::render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	{
		glEnable(GL_DEPTH_TEST);
		const float depthClear = 1.0;
		glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));

		glClearBufferfv(GL_DEPTH, 0, &depthClear);

		renderTerrain();
	} glBindFramebuffer(GL_FRAMEBUFFER, 0);

	renderFullscreenQuad();

	::flushGLError("Terrain::render()");
}
