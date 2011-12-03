#version 400 core

#define POSITION	0
#define COLOR		3
#define TEXCOORD	4
#define FRAG_COLOR	0

layout(location = FRAG_COLOR) out vec4 color;

in vec2 fTexCoord;

void main()
{
	color = vec4(1.0, 0.0, 0.0, 1.0);
}
