#include "stdafx.h"

#include "Shader.h"

Shader::Shader()
{

}

void Shader::addStage(const GLchar* filePath, const GLchar* preprocessor, GLenum type)
{
	shaderStage s; s.filePath = filePath; s.preprocessor = preprocessor; s.type = type;
	shaderStages.push_back(s);
}

bool Shader::install()
{
	printf("--------------------------------\n");
	printf("Building shader...\n");

	program = glCreateProgram();

	bool allStagesCompiled = true;
	for ( unsigned int i = 0; i < shaderStages.size(); ++i )
	{
		GLint compiled;
		const GLchar* source = LoadShaderText(shaderStages[i].filePath);

		const GLchar* fullSource[2];
		fullSource[0] = shaderStages[i].preprocessor;
		fullSource[1] = source;

		GLuint shader = glCreateShader(shaderStages[i].type);
		glShaderSource(shader, 2, &fullSource[0], NULL);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			printf("error when compiling shader: %s \n\n", shaderStages[i].filePath);
			printShaderInfoLog(shader);
			allStagesCompiled = false;
		}
		else
			printf("Shader stage: %s compiled\n", shaderStages[i].filePath);
		glAttachShader(program, shader);
	}

	if ( !allStagesCompiled )
	{
		printf("*** Using fixed function pipeline ***\n");
		printf("--------------------------------\n");
        throw std::runtime_error("Shader compile failed\n");
		return false;
	}

	GLint linked;
//	glBindFragDataLocationEXT(program, 0, "basicFrag");
	
	if ( varyings.size() > 0 )
		glTransformFeedbackVaryings(program, varyings.size(), &varyings[0], GL_SEPARATE_ATTRIBS);

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		printProgramInfoLog();
		printf("linking of shader failed...\n");
		printf("*** Using fixed function pipeline ***\n");
		printf("--------------------------------\n");
        throw std::runtime_error("Shader link failed\n");
		return false;
	}
	glUseProgram(program); // turn on programmable pipeline
	printf("Link success...\n");
	printf("--------------------------------\n");

	return true;
}
#pragma warning(push)
#pragma warning(disable:4996)
const GLchar* Shader::LoadShaderText(const char *fileName)
{
    char* shaderText = NULL;
    GLint shaderLength = 0;
    FILE* fp;
    fp = fopen(fileName, "r");
    if (fp != NULL)
    {
        while (fgetc(fp) != EOF)
        {
            shaderLength++;
        }
        rewind(fp);
        shaderText = (char*)malloc(shaderLength+1);
        if (shaderText != NULL)
        {
			fread(shaderText, 1, shaderLength, fp);
        }
        shaderText[shaderLength] = '\0';
        fclose(fp);
    }
    return shaderText;
}
#pragma warning(pop)

void Shader::printShaderInfoLog(GLuint shader)
{
	int infologLen = 0;
	int charsWritten = 0;
	GLchar* infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);

	if (infologLen > 0)
	{
		infoLog = (GLchar*) malloc(infologLen);
		if (infoLog == NULL)
		{
			std::cout << "ERROR: Could not allocate infolog buffer" << std::endl;
			exit(1);
		}
		glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog);
		printf("InfoLog shader:\n%s\n", infoLog);
	}
}

void Shader::printProgramInfoLog()
{
	int infologLen = 0;
	int charsWritten = 0;
	GLchar* infoLog;

	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infologLen);

	if (infologLen > 0)
	{
		infoLog = (GLchar*) malloc(infologLen);
		if (infoLog == NULL)
		{
			std::cout << "ERROR: Could not allocate infolog buffer" << std::endl;
			exit(1);
		}
		glGetProgramInfoLog(program, infologLen, &charsWritten, infoLog);
		printf("InfoLog program:\n%s\n", infoLog);
	}
}
int Shader::getUniLoc(char* name)
{
	glUseProgram(program);
	int pos = glGetUniformLocation(program, name);
	if ( pos == -1 )
		printf("uniform: %s not found\n", name);
	return pos;
}