#version 400 core

#define POSITION	0
#define NORMAL		1
#define COLOR		2
#define TEXCOORD	3
#define FRAG_COLOR	0
#define FRAG_COLOR_AD0 1
#define FRAG_COLOR_AD1 2
#define FRAG_COLOR_AD2 3

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform sampler2D heightSampler;

layout(location = POSITION) in vec4 pos;
layout(location = TEXCOORD) in vec2 texcoord;

out vec2 fTexcoord;
out vec3 fPos;

void main()
{
    float height = texture(heightSampler, texcoord).r;
	mat4 mvp = proj * view;// * model;
	vec4 posMVP = mvp * vec4(pos.x, height, pos.z, 1.0);
	gl_Position = posMVP;
	
	fTexcoord = texcoord;
}