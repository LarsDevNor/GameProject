#version 400 core

#define POSITION	0
#define NORMAL		1
#define COLOR		2
#define TEXCOORD	3
#define FRAG_COLOR	0
#define FRAG_COLOR_AD0 1
#define FRAG_COLOR_AD1 2

//uniform sampler2D depthSampler;
uniform sampler2D colorSampler;
uniform sampler2D normalSampler;
uniform sampler2D depthSampler;

layout(location = FRAG_COLOR) out vec4 COLOR_OUT;

in vec2 fTexcoord;

void main()
{
	float depthValue	= texture(depthSampler, fTexcoord).r;
	vec3 colorValue		= texture(colorSampler, fTexcoord).rgb;
	vec3 normalValue	= texture(normalSampler, fTexcoord).rgb;

	vec4 color;
	color.r = ( texture(colorSampler, fTexcoord).r + texture(colorSampler, fTexcoord).g + texture(colorSampler, fTexcoord).b ) / 3;
	color.gb = vec2(1.0 - texture(depthSampler, fTexcoord).r);
	color.a = 1.0;
	
	if  ( depthValue > 0.99 ) 
		color = vec4(0.0, 1.0, 0.0, 1.0);
	else
		color = vec4(color.r, 0.0, 0.0, 1.0);

	color = vec4(vec3(depthValue), 1.0);

	color = vec4(colorValue, 1.0);

	COLOR_OUT = vec4( mix(colorValue, vec3(0.5, 0.7, 0.3), pow(depthValue, 10.0)), 1.0); 

	float depthLeft		= textureOffset(depthSampler, fTexcoord, ivec2(-1, 0)).r;
	float depthRight	= textureOffset(depthSampler, fTexcoord, ivec2(1, 0)).r;
	float depthBottom	= textureOffset(depthSampler, fTexcoord, ivec2(0, -1)).r;
	float depthTop		= textureOffset(depthSampler, fTexcoord, ivec2(0, 1)).r;

/*
	float delta = 0.005f;
	depthLeft	= texture(depthSampler, fTexcoord + vec2(-delta, 0.0)).r;
	depthRight	= texture(depthSampler, fTexcoord + vec2(delta, 0.0)).r;
	depthBottom	= texture(depthSampler, fTexcoord + vec2(0, -delta)).r;
	depthTop	= texture(depthSampler, fTexcoord + vec2(0, delta)).r;
*/
	float closest = min(min(depthLeft, min(depthRight, depthBottom)), depthTop);
	float zDist = max(0.0, depthValue - closest);

//	COLOR_OUT = vec4(vec3(zDist*100.0), 1.0);

	//COLOR_OUT = vec4(colorValue, 1.0);
	COLOR_OUT = vec4(normalValue, 1.0);
}
