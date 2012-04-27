#pragma once 

/*
	ITEMS:

	- diff types of postprocessing filters - takes input and output textures needed for filter *** 100% 
	- ShaderManager - installs shaders, associates string names with shaders *** 0%
	- FrameBufferObject-, Texture-, other_GL_type- Wrappers - unngår handles, kan legge mye brukt setup kode her etc *** Wrapped texture2d 
	- Terrain editing - Drag up/down, apply diff textures etc *** 50%
	- Clamp player to terrain in shader *** 0%
	- generate terrain as we move *** 0%
	- weapons *** 0%
	- Destructible terrain *** 20%
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

struct ParticleVertex
{
    ParticleVertex () : pos(glm::vec4(0.0, 0.0, 0.0, 0.0)), color(glm::vec4(0.0, 0.0, 0.0, 0.0)) {}
    ParticleVertex ( glm::vec4 pos, glm::vec4 color ) :
        pos(pos), color(color) {}

    glm::vec4 pos;
    glm::vec4 color;
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

        enum particle 
        {
            PARTICLE_POSITION = 0,
            PARTICLE_COLOR = 1
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

inline GLuint createTexture2D(glm::ivec2 dim, GLint internalFormat, GLenum inputFormat, GLenum inputType, GLint filter, GLint wrap, const void* data)
{
	GLuint textureHandle = 0;	
	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);	
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, dim.x, dim.y, 0, inputFormat, inputType, data);
	::flushGLError("::createTexture2D()");
	return textureHandle;
}

inline void CLErr(cl_int errorCode)
{
    std::string errorStr;
    switch(errorCode)
    {
    case    CL_SUCCESS:
        return;
    case    CL_DEVICE_NOT_FOUND:
        errorStr = "CL_DEVICE_NOT_FOUND";
        break;
    case    CL_DEVICE_NOT_AVAILABLE:
        errorStr = "CL_DEVICE_NOT_AVAILABLE";
        break;
    case    CL_COMPILER_NOT_AVAILABLE:
        errorStr = "CL_COMPILER_NOT_AVAILABLE";
        break;
    case    CL_OUT_OF_RESOURCES:
        errorStr = "CL_OUT_OF_RESOURCES";
        break;
    case    CL_OUT_OF_HOST_MEMORY:
        errorStr = "CL_OUT_OF_HOST_MEMORY";
        break;
    case    CL_PROFILING_INFO_NOT_AVAILABLE:
        errorStr = "CL_PROFILING_INFO_NOT_AVAILABLE";
        break;
    case    CL_MEM_COPY_OVERLAP:
        errorStr = "CL_MEM_COPY_OVERLAP";
        break;
    case    CL_IMAGE_FORMAT_MISMATCH:
        errorStr = "CL_IMAGE_FORMAT_MISMATCH";
        break;
    case    CL_IMAGE_FORMAT_NOT_SUPPORTED:
        errorStr = "CL_IMAGE_FORMAT_NOT_SUPPORTED";
        break;
    case    CL_BUILD_PROGRAM_FAILURE:
        errorStr = "CL_BUILD_PROGRAM_FAILURE";
        break;
    case    CL_MAP_FAILURE:
        errorStr = "CL_MAP_FAILURE";
        break;
    case    CL_INVALID_VALUE:
        errorStr = "CL_INVALID_VALUE";
        break;
    case    CL_INVALID_DEVICE_TYPE:
        errorStr = "CL_INVALID_DEVICE_TYPE";
        break;
    case    CL_INVALID_PLATFORM:
        errorStr = "CL_INVALID_PLATFORM";
        break;
    case    CL_INVALID_DEVICE:
        errorStr = "CL_INVALID_DEVICE";
        break;
    case    CL_INVALID_CONTEXT:
        errorStr = "CL_INVALID_CONTEXT";
        break;
    case    CL_INVALID_QUEUE_PROPERTIES:
        errorStr = "CL_INVALID_QUEUE_PROPERTIES";
        break;
    case    CL_INVALID_COMMAND_QUEUE:
        errorStr = "CL_INVALID_COMMAND_QUEUE";
        break;
    case    CL_INVALID_HOST_PTR:
        errorStr = "CL_INVALID_HOST_PTR";
        break;
    case    CL_INVALID_MEM_OBJECT:
        errorStr = "CL_INVALID_MEM_OBJECT";
        break;
    case    CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
        errorStr = "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
        break;
    case    CL_INVALID_IMAGE_SIZE:
        errorStr = "CL_INVALID_IMAGE_SIZE";
        break;
    case    CL_INVALID_SAMPLER:
        errorStr = "CL_INVALID_SAMPLER";
        break;
    case    CL_INVALID_BINARY:
        errorStr = "CL_INVALID_BINARY";
        break;
    case    CL_INVALID_BUILD_OPTIONS:
        errorStr = "CL_INVALID_BUILD_OPTIONS";
        break;
    case    CL_INVALID_PROGRAM:
        errorStr = "CL_INVALID_PROGRAM";
        break;
    case    CL_INVALID_PROGRAM_EXECUTABLE:
        errorStr = "CL_INVALID_PROGRAM_EXECUTABLE";
        break;
    case    CL_INVALID_KERNEL_NAME:
        errorStr = "CL_INVALID_KERNEL_NAME";
        break;
    case    CL_INVALID_KERNEL_DEFINITION:
        errorStr = "CL_INVALID_KERNEL_DEFINITION";
        break;
    case    CL_INVALID_KERNEL:
        errorStr = "CL_INVALID_KERNEL";
        break;
    case    CL_INVALID_ARG_INDEX:
        errorStr = "CL_INVALID_ARG_INDEX";
        break;
    case    CL_INVALID_ARG_VALUE:
        errorStr = "CL_INVALID_ARG_VALUE";
        break;
    case    CL_INVALID_ARG_SIZE:
        errorStr = "CL_INVALID_ARG_SIZE";
        break;
    case    CL_INVALID_KERNEL_ARGS:
        errorStr = "CL_INVALID_KERNEL_ARGS";
        break;
    case    CL_INVALID_WORK_DIMENSION:
        errorStr = "CL_INVALID_WORK_DIMENSION";
        break;
    case    CL_INVALID_WORK_GROUP_SIZE:
        errorStr = "CL_INVALID_WORK_GROUP_SIZE";
        break;
    case    CL_INVALID_WORK_ITEM_SIZE:
        errorStr = "CL_INVALID_WORK_ITEM_SIZE";
        break;
    case    CL_INVALID_GLOBAL_OFFSET:
        errorStr = "CL_INVALID_GLOBAL_OFFSET";
        break;
    case    CL_INVALID_EVENT_WAIT_LIST:
        errorStr = "CL_INVALID_EVENT_WAIT_LIST";
        break;
    case    CL_INVALID_EVENT:
        errorStr = "CL_INVALID_EVENT";
        break;
    case    CL_INVALID_OPERATION:
        errorStr = "CL_INVALID_OPERATION";
        break;
    case    CL_INVALID_GL_OBJECT:
        errorStr = "CL_INVALID_GL_OBJECT";
        break;
    case    CL_INVALID_BUFFER_SIZE:
        errorStr = "CL_INVALID_BUFFER_SIZE";
        break;
    case    CL_INVALID_MIP_LEVEL:
        errorStr = "CL_INVALID_MIP_LEVEL";
        break;
    case    CL_INVALID_GLOBAL_WORK_SIZE:
        errorStr = "CL_INVALID_GLOBAL_WORK_SIZE";
        break;
    default:
        errorStr = "Unknown CL error";
    }
    throw std::runtime_error("OpenCL error: " + errorStr + "\n");
}

inline float randNormNegPos() 
{
    return -1.0f + 2.0f * static_cast<float>(rand()) / RAND_MAX;
}

inline float randNormPos()
{
    return static_cast<float>(rand()) / RAND_MAX;
}
