#version 400 core

#define POSITION	0
#define NORMAL		1
#define COLOR		2
#define TEXCOORD	3
#define FRAG_COLOR	0
#define FRAG_COLOR_AD0 1
#define FRAG_COLOR_AD1 2
#define FRAG_COLOR_AD2 3

uniform sampler2D textureSampler;
uniform vec2 editPos;

uniform float threshold;
uniform float strength;

layout(location = FRAG_COLOR) out float COLOR_OUT;

in vec2 fTexcoord;

void main()
{
	float value = texture(textureSampler, fTexcoord).r;

    float proximity = 1.0/threshold * ( threshold - min(threshold,length(fTexcoord - editPos)) );
    COLOR_OUT = value+strength*pow(proximity, 4);
  //  COLOR_OUT = value + strength*sin(proximity * 3.14/2.0);
}
