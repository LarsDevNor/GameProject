#version 400 core

#define POSITION	0
#define COLOR		3
#define TEXCOORD	4
#define FRAG_COLOR	0

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

layout(location = POSITION) in vec4 pos;
layout(location = TEXCOORD) in vec2 texCoord;

out vec2 fTexCoord;

void main()
{
	fTexCoord = texCoord;
	mat4 mvp = proj * view; //model;
	gl_Position = mvp * vec4(pos);
}