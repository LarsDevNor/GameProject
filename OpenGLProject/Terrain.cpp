#include "stdafx.h"

#include "PostProcess.h"
#include "GameManager.h"
#include "PerlinNoise.h"
#include "Camera.h"
#include "Shader.h"
#include "Terrain.h"

/*

	PICKING TODO: First simple version. Take picket position as input to a shader handling heightmap..
	lift by cos, 1/distFromCentre^2 or something 

*/

Terrain::Terrain() : heightTextures(2), inputTex(0)
{
	gm = GameManager::getInstance();

	initShader();
	initGeometry();
	initPickingFBO();
	//initTextures();
	//initSamplers();
	//initFBO();

	editPP = new PostProcessEdit();
}

void Terrain::initPickingFBO()
{
    GLuint pickingRBDepth = 0;
	// create, bind, and establish storage for renderbuffer 
	glGenRenderbuffers(1, &pickingRB);
    glGenRenderbuffers(1, &pickingRBDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, pickingRB);
	glm::ivec2 windowDim = gm->getWindowDim();
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, windowDim.x, windowDim.y);
    glBindRenderbuffer(GL_RENDERBUFFER, pickingRBDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowDim.x, windowDim.y);

	// create and bind FBO, attach renderbuffer to it 
	glGenFramebuffers(1, &pickingFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, pickingFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, pickingRB);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, pickingRB);
    glBindRenderbuffer(GL_RENDERBUFFER, pickingRBDepth);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, pickingRBDepth);

	::checkFramebuffer(GL_FRAMEBUFFER, "Terrain::initPickingFBO()");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	::flushGLError("Terrain::initPickingFBO()");	
}

void Terrain::initGeometry()
{
	nVertsHeight = 1024;
	nVertsWidth = 1024;
	width = 1024.0f;
	height = 1024.0f;
	pn = new PerlinNoise(0.5, 1.0f, 2500/(nVertsHeight+nVertsWidth), 8, 5);

	double bump = 1.0f;
	glm::vec2 spacing((float)width / nVertsWidth, (float)height / nVertsHeight);
	{ // create geometry 
		for ( size_t i = 0; i < nVertsHeight; ++i ) 
		for ( size_t j = 0; j < nVertsWidth; ++j )
		{
			//float vertexHeight = -10.0f + (float)pn->GetHeight(j*0.1f, i*0.1f);
			//float vertexHeight = static_cast<float>(bump*((0.5+pn->GetHeight(j*0.1, i*0.1))-1.0));

            float vertexHeight = 100.0f*static_cast<float>(j)/(nVertsHeight-1);
            vertexHeight = -10.0f;
		//	vertexHeight = i*10.1f+j*0.1f;
		//	vertexHeight = i*1.0f/(nVertsHeight-1);
			heights.push_back(vertexHeight);
		//	printf("%g\n", vertexHeight);
		}
		//heightTex = createTexture2D(glm::ivec2(nVertsWidth, nVertsHeight), GL_R32F, GL_RED, GL_FLOAT, GL_LINEAR, GL_CLAMP_TO_EDGE, &heights[0]);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
		heightTextures[0] = createTexture2D(glm::ivec2(nVertsWidth, nVertsHeight), GL_R32F, GL_RED, GL_FLOAT, GL_LINEAR, GL_CLAMP_TO_EDGE, &heights[0]);
		heightTextures[1] = createTexture2D(glm::ivec2(nVertsWidth, nVertsHeight), GL_R32F, GL_RED, GL_FLOAT, GL_LINEAR, GL_CLAMP_TO_EDGE, &heights[0]);

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
			glm::vec2 normPos((float)(j)/(nVertsWidth-1), (float)(i)/(nVertsHeight-1));
			glm::vec2 pos = glm::vec2(normPos.x * width, normPos.y * height);

            // remap [0,max] -> [0.5,max-0.5]
            glm::vec2 texelPos = normPos + (glm::vec2(0.5f, 0.5f) / glm::vec2(glm::vec2(nVertsWidth-1.0f, nVertsHeight-1.0f)));
            texelPos *= glm::vec2(nVertsWidth-2.0f, nVertsHeight-2.0f)/glm::vec2(nVertsWidth-1.0f, nVertsHeight-1.0f); 

			vertices.push_back( PNTVertex(glm::vec3(pos.x, 0.0f, pos.y), normals[j+i*nVertsWidth], texelPos));
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
			glEnableVertexAttribArray(semantic::attr::POSITION);
			glEnableVertexAttribArray(semantic::attr::NORMAL);
			glEnableVertexAttribArray(semantic::attr::TEXCOORD);
		} glBindVertexArray(0);
	} 

	::flushGLError("Terrain::initGeometry()");
}

void Terrain::initShader()
{
	defaultShader = new Shader();
    /*defaultShader->addStage("./shaders/terrain.vert", "", GL_VERTEX_SHADER);
    defaultShader->addStage("./shaders/terrain.frag", "", GL_FRAGMENT_SHADER);*/
	defaultShader->addStage("./shaders/terrainSimple.vert", "", GL_VERTEX_SHADER);
	defaultShader->addStage("./shaders/terrainSimple.frag", "", GL_FRAGMENT_SHADER);
	defaultShader->install();

	pickingShader = new Shader();
	pickingShader->addStage("./shaders/pickingTerrain.vert", "", GL_VERTEX_SHADER);
	pickingShader->addStage("./shaders/pickingTerrain.frag", "", GL_FRAGMENT_SHADER);
	pickingShader->install();
}

void Terrain::renderTerrain(Shader* renderShader)
{
	renderShader->begin();
	{
		int viewLoc = renderShader->getUniLoc("view");
		int projLoc = renderShader->getUniLoc("proj");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(gm->getActiveCamera()->getViewMatrix()));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(gm->getActiveCamera()->getProjMatrix()));

		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, getInputTex());
		glUniform1i(renderShader->getUniLoc("heightSampler"), 0);

		glBindVertexArray(vao);
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		} glBindVertexArray(0);
	} renderShader->end();
	::flushGLError("Terrain::renderTerrain()");
}

void Terrain::pick(const glm::ivec2& screenCoord)
{
    float pixelValue[4];
	glBindFramebuffer(GL_FRAMEBUFFER, pickingFBO);
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
	    glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)));
        const float depthClear = 1.0; // clear to max depth 
        glClearBufferfv(GL_DEPTH, 0, &depthClear);
	    renderTerrain(pickingShader);
	    glReadPixels( screenCoord.x, gm->getWindowDim().y - screenCoord.y, 1, 1, GL_RGBA, GL_FLOAT, &pixelValue);
    } glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glm::vec2 normPickPos = glm::vec2(pixelValue[0], pixelValue[1]);
    //std::printf("pixel: %g %g\n", normPickPos.x, normPickPos.y );
    //std::printf("in: %i out: %i\n", getInputTex(), getOutputTex());

	editPP->run(normPickPos, PostProcessEdit::UP, getInputTex(), getOutputTex());
	swapTex();


	::flushGLError("Terrain::pick()");
}

// TODO: use a diff shader when not render to FBOs
void Terrain::render(GLuint fbo)
{
	if( fbo != 0 )
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		GLenum drawBuffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, drawBuffers);

		glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
		glClearBufferfv(GL_COLOR, 1, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
		glClearBufferfv(GL_COLOR, 2, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)));
		glClearBufferfv(GL_COLOR, 3, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)));

		const float depthClear = 1.0; // clear to max depth 
		glClearBufferfv(GL_DEPTH, 0, &depthClear);
	}
    //glDisable(GL_CULL_FACE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    defaultShader->begin();
    glUniform1f(defaultShader->getUniLoc("spacing"), (float)width / nVertsWidth);
    defaultShader->end();
	renderTerrain(defaultShader);
    //renderTerrain(pickingShader);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if ( fbo != 0)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	::flushGLError("Terrain::render()");
}
