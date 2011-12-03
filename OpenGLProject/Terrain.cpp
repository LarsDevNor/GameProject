#include "stdafx.h"

#include "Shader.h"
#include "Terrain.h"

Terrain::Terrain() 
{
	initGeometry();
	initShader();
}

void Terrain::initGeometry()
{
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
	shaderDefault->addStage("./shaders/clTerrainInst.vert", "", GL_VERTEX_SHADER);
	shaderDefault->addStage("./shaders/clTerrainInst.frag", "", GL_FRAGMENT_SHADER);
	shaderDefault->install();
}

void Terrain::render()
{
	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, vertices.size());
	// glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}
