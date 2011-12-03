#include "stdafx.h"

#include "GameManager.h"
#include "Camera.h"
#include "Shader.h"
#include "Terrain.h"

Terrain::Terrain() 
{
	gm = GameManager::getInstance();

	initGeometry();
	initShader();
}

void Terrain::initGeometry()
{
	nVertsHeight = 10;
	nVertsWidth = 10;
	width = 100.0f;
	height = 100.0f;
	{ // create geometry 
		for ( int i = 0; i < nVertsHeight; ++i ) 
		for ( int j = 0; j < nVertsWidth; ++j )
		{
			glm::vec2 normPos((float)j/(nVertsWidth-1), (float)i/(nVertsHeight-1));
			glm::vec2 pos = glm::vec2(normPos.x * width, normPos.x * height);

			vertices.push_back(PTVertex(glm::vec3(pos.x, 0.0f, pos.y), normPos));
		}
	}
	{ // bind geometry to buffers 
		glGenBuffers(1, &vbo);
		glGenVertexArrays(1, &vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		{
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(PTVertex), &vertices[0], GL_STATIC_DRAW);
		} glBindBuffer(GL_ARRAY_BUFFER, 0);
	
		glBindVertexArray(vao);
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			{
				glVertexAttribPointer(semantic::attr::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(PTVertex), BUFFER_OFFSET(0));
				glVertexAttribPointer(semantic::attr::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(PTVertex), BUFFER_OFFSET(sizeof(glm::vec3)));
			}
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
	shaderDefault->begin();
	{
		int viewLoc = shaderDefault->getUniLoc("view");
		int projLoc = shaderDefault->getUniLoc("proj");
		int modelLoc = shaderDefault->getUniLoc("model");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(gm->getActiveCamera()->getViewMatrix()));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(gm->getActiveCamera()->getProjMatrix()));

		//glBindVertexArray(vao);
		//glDrawArrays(GL_POINTS, 0, vertices.size());
	} shaderDefault->end();
	// glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}
