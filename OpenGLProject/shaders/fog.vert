#version 400 core

#define POSITION	0
#define NORMAL		1
#define COLOR		2
#define TEXCOORD	3
#define FRAG_COLOR	0
#define FRAG_COLOR_AD0 1

layout(location = POSITION) in vec2 pos;

out vec2 fTexcoord;

void main()
{
	fTexcoord = vec2(0.5, 0.5) + 0.5 * pos.xy;
	gl_Position = vec4(pos.xy, 0.0, 1.0);
}