#include "stdafx.h"

#include "GameManager.h"
#include "PerlinNoise.h"
#include "Camera.h"
#include "Shader.h"
#include "Terrain.h"

Terrain::Terrain() 
{
	gm = GameManager::getInstance();
	pn = new PerlinNoise(0.5, 0.1, 20.0, 8, 5);

	initGeometry();
	initShader();
}

void Terrain::initGeometry()
{
	nVertsHeight = 128;
	nVertsWidth = 128;
	width = 200.0f;
	height = 200.0f;
	{ // create geometry 
		for ( size_t i = 0; i < nVertsHeight; ++i ) 
		for ( size_t j = 0; j < nVertsWidth; ++j )
		{
			glm::vec2 normPos((float)j/(nVertsWidth-1), (float)i/(nVertsHeight-1));
			glm::vec2 pos = glm::vec2(normPos.x * width, normPos.y * height);

			vertices.push_back(PTVertex(glm::vec3(pos.x, -10.0f + pn->GetHeight(j,i), pos.y), normPos));
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
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(PTVertex), &vertices[0], GL_STATIC_DRAW);
		} glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		} glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(vao);
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			{
				glVertexAttribPointer(semantic::attr::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(PTVertex), BUFFER_OFFSET(0));
				glVertexAttribPointer(semantic::attr::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(PTVertex), BUFFER_OFFSET(sizeof(glm::vec3)));
			}
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glEnableVertexAttribArray(semantic::attr::POSITION);
			glEnableVertexAttribArray(semantic::attr::TEXCOORD);
		} glBindVertexArray(0);
	}
}

void Terrain::initShader()
{
	shaderDefault = new Shader();
	shaderDefault->addStage("./shaders/terrain.vert", "", GL_VERTEX_SHADER);
	shaderDefault->addStage("./shaders/terrain.frag", "", GL_FRAGMENT_SHADER);
	shaderDefault->install();
}

void Terrain::render()
{
	glPointSize(5.0f);

	shaderDefault->begin();
	{
		int viewLoc = shaderDefault->getUniLoc("view");
		int projLoc = shaderDefault->getUniLoc("proj");
		int modelLoc = shaderDefault->getUniLoc("model");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(gm->getActiveCamera()->getViewMatrix()));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(gm->getActiveCamera()->getProjMatrix()));

		glBindVertexArray(vao);
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		}

	} shaderDefault->end();
	// glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}
