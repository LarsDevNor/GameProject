#version 400 core

#define POSITION	0
#define NORMAL		1
#define COLOR		2
#define TEXCOORD	3
#define FRAG_COLOR	0
#define FRAG_COLOR_AD0 1
#define FRAG_COLOR_AD1 2

uniform sampler2D colorSampler;
uniform sampler2D depthSampler;

layout(location = FRAG_COLOR) out vec4 COLOR_OUT;

in vec2 fTexcoord;

void main()
{
	float depthValue	= texture(depthSampler, fTexcoord).r;
	vec3 colorValue		= texture(colorSampler, fTexcoord).rgb;

	vec4 color;
	color.r = ( texture(colorSampler, fTexcoord).r + texture(colorSampler, fTexcoord).g + texture(colorSampler, fTexcoord).b ) / 3;
	color.gb = vec2(1.0 - texture(depthSampler, fTexcoord).r);
	color.a = 1.0;

	color = vec4(colorValue, 1.0);

	COLOR_OUT = vec4( mix(colorValue, vec3(0.5, 0.7, 0.3), pow(depthValue, 10.0)), 1.0); 
}
