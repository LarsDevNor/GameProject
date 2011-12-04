#version 400 core

#define POSITION	0
#define NORMAL		3
#define COLOR		6
#define TEXCOORD	7
#define FRAG_COLOR	0

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

layout(location = POSITION) in vec4 pos;
layout(location = NORMAL)	in vec3 normal;
layout(location = TEXCOORD) in vec2 texCoord;

out vec2 fTexCoord;
out vec3 fNormal;

void main()
{
	fNormal = normal;
	fTexCoord = texCoord;
	mat4 mvp = proj * view; //model;
	gl_Position = mvp * vec4(pos);
}