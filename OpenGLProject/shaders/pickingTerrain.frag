#version 400 core

#define POSITION	0
#define NORMAL		1
#define COLOR		2
#define TEXCOORD	3
#define FRAG_COLOR	0
#define FRAG_COLOR_AD0 1
#define FRAG_COLOR_AD1 2
#define FRAG_COLOR_AD2 3

layout(location = FRAG_COLOR) out vec4 color;

in vec2 fTexcoord;

void main()
{
	color = vec4(fTexcoord.x, fTexcoord.y, 0.0, 1.0);
}
