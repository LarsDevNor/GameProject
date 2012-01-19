#include "stdafx.h"

#include "GameManager.h"
#include "PerlinNoise.h"
#include "Camera.h"
#include "Shader.h"
#include "Terrain.h"



Terrain::Terrain() 
{
	gm = GameManager::getInstance();

	initShader();
	initGeometry();
	//initTextures();
	//initSamplers();
	//initFBO();
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

	::flushGLError("Terrain::initGeometry()");
}

void Terrain::initShader()
{
	shaderDefault = new Shader();
	shaderDefault->addStage("./shaders/terrain.vert", "", GL_VERTEX_SHADER);
	shaderDefault->addStage("./shaders/terrain.frag", "", GL_FRAGMENT_SHADER);
	shaderDefault->install();
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

void Terrain::render(GLuint fbo)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	{
		GLenum drawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, drawBuffers);

		glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
		glClearBufferfv(GL_COLOR, 1, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));

		const float depthClear = 1.0;
		glClearBufferfv(GL_DEPTH, 0, &depthClear);

		renderTerrain();
	} glBindFramebuffer(GL_FRAMEBUFFER, 0);

	::flushGLError("Terrain::render()");
}
