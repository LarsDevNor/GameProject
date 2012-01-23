#pragma once 

/*
	ITEMS:

	- diff types of postprocessing filters - takes input and output textures needed for filter *** 100% 
	- ShaderManager - installs shaders, associates string names with shaders *** 0%
	- FrameBufferObject-, Texture-, other_GL_type- Wrappers - unng�r handles, kan legge mye brukt setup kode her etc *** 0%
	- Terrain editing - Drag up/down, apply diff textures etc *** 0%
	- Clamp player to terrain in shader *** 0%
	- generate terrain as we move *** 0%
	- weapons *** 0%
	- Destructible terrain *** 0%
	- Particle system *** 0% 
	- SSAO *** 0%
	- deferred renderer *** 0%
	- OpenCL processing (use for particle system maybe) *** 0%
	- Add shader variable position defines from c++ before compile *** 0%
*/

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

struct PNTVertex
{
	PNTVertex( glm::vec3 const & position, glm::vec3 normal, glm::vec2 const & texcoord ) :
	position(position), normal(normal), texcoord(texcoord) {}

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoord;
};

namespace semantic 
{
	namespace attr
	{
		enum type
		{
			POSITION = 0,
			NORMAL = 1,
			COLOR	 = 2,
			TEXCOORD = 3
		};
	};
};

inline void flushGLError(const char* whoIsChecking)
{
	GLenum error = glGetError();
	if ( error != GL_NO_ERROR )
	{
		while ( true )
		{
			printf("GL error\t%s\t", whoIsChecking);
			if ( error == GL_INVALID_ENUM )
				printf("Invalid enum\n");
			else if ( error == GL_INVALID_VALUE )
				printf("Invalid value\n");
			else if ( error == GL_INVALID_OPERATION )
				printf("Invalid operation\n");
			else if ( error == GL_STACK_OVERFLOW )
				printf("Stack overflow\n");
			else if ( error == GL_STACK_UNDERFLOW )
				printf("Stack underflow\n");
			else if ( error == GL_OUT_OF_MEMORY )
				printf("Out of memory\n");
			else if ( error == GL_TABLE_TOO_LARGE )
				printf("Table too large\n");
			error = glGetError();
			if ( error == GL_NO_ERROR )
				break;
		}
	}
}

inline bool checkFramebuffer(GLuint FramebufferName, const char* WhoIsChecking)
{
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if ( status == GL_FRAMEBUFFER_COMPLETE ) 
		return true;
	else 
		printf("FBO error\t%s\t", WhoIsChecking);
	switch(status)
	{
	case GL_FRAMEBUFFER_UNDEFINED:
		printf("%s\n", "GL_FRAMEBUFFER_UNDEFINED");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		printf("%s\n", "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		printf("%s\n", "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		printf("%s\n", "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		printf("%s\n", "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		printf("%s\n", "GL_FRAMEBUFFER_UNSUPPORTED");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		printf("%s\n", "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		printf("%s\n", "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS");
		break;
	}
	return false;
}