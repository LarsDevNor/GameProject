/*
	Simple shader class. One instance - one complete set of shader stages. 
	Lars Hofsøy Breivik
*/
#pragma once 

#include <vector>

#include <GL/glew.h>
#include "stdio.h"
#include <string>
#include <iostream>

struct shaderStage
{
	const GLchar* filePath;
	const GLchar* preprocessor;
	GLenum type;
};

class Shader
{
public:
	Shader();

	bool install();
	const GLchar* LoadShaderText(const char *fileName);
	void printShaderInfoLog(GLuint shader);
	void printProgramInfoLog();
	int getUniLoc(char* name);
//	void addStage(std::pair<GLenum, const GLchar*> shaderInfo);
	void addStage(const GLchar* filePath, const GLchar* preprocessor, GLenum type);

	unsigned int getProgram() { return program; }
	void begin() { glUseProgram(program); }
	void end() { glUseProgram(0); }
	void addVarying(const GLchar* varying) { varyings.push_back(varying); }

private:
	/*void compileShader(GLuint shader, const GLchar* source);*/
	unsigned int program;

	std::vector<const GLchar*> varyings;

	const char* VSPath; 
	const char* TCPath;
	const char* TEPath;
	const char* FSPath;


	//std::vector<std::pair<GLenum, const GLchar*>> shaderStages;
	std::vector<shaderStage> shaderStages;
};
