#version 400 core

#define POSITION	0
#define NORMAL		1
#define COLOR		2
#define TEXCOORD	3
#define FRAG_COLOR	0
#define FRAG_COLOR_AD0 1

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

layout(location = POSITION) in vec4 pos;
layout(location = NORMAL)	in vec3 normal;
layout(location = TEXCOORD) in vec2 texCoord;

out vec2 fTexCoord;
out vec3 fNormal;
out vec3 fPos;
flat out vec3 fFlatPos;

void main()
{
	mat4 mvp = proj * view; //model;
	vec4 posMVP = mvp * vec4(pos);
	gl_Position = posMVP;
	
	fPos = pos.xyz;
	fFlatPos = pos.xyz;
	fNormal = (view * vec4(normal, 0.0)).xyz;
	fTexCoord = texCoord;
}