#version 400 core

#define POSITION	0
#define NORMAL		1
#define COLOR		2
#define TEXCOORD	3
#define FRAG_COLOR	0
#define FRAG_COLOR_AD0 1
#define FRAG_COLOR_AD1 2
#define FRAG_COLOR_AD2 3

//uniform sampler2D depthSampler;
uniform sampler2D colorSampler;
uniform sampler2D normalSampler;
uniform sampler2D positionSampler;
uniform sampler2D depthSampler;

layout(location = FRAG_COLOR) out vec4 COLOR_OUT;

in vec2 fTexcoord;

#define PI 3.14159265
// TODO: look here maybe for more ideas http://www.cmlab.csie.ntu.edu.tw/~fielia/research/ao-gpu/FINAL-paper121.pdf

/* TODO :::::::::
float LinearizeDepth(vec2 uv)
{
  float n = 1.0; // camera z near
  float f = 100.0; // camera z far
  float z = texture2D(depthSampler, uv).x;
  return (2.0 * n) / (f + n - z * (f - n));
}*/

void main()
{
	vec3 color 			= texture(colorSampler, fTexcoord).rgb;
	vec3 pixelNorm		= normalize(texture(normalSampler, fTexcoord).rgb);
	vec3 pixelPos 		= texture(positionSampler, fTexcoord).rgb;

	if ( pixelNorm.y < 0.0)
	{
		COLOR_OUT = vec4(1.0, 1.0, 0.0, 1.0);
		return;
	}

	float sampleRadius = 0.05 / pixelPos.z; // calc based on screensize ? make uniform.. 
	const float bias = 0.1;
	const float intensity = 30.0;
	const int numSamples = 64;
	float occlusion = 0.0;
	for ( int i = 0; i < numSamples; ++i)
	{
		vec2 offset = vec2(sin(i*PI*2.0/(numSamples-1)), cos(i*PI*2.0/(numSamples-1)));
		vec2 occluderSamplingPosition = fTexcoord + offset*sampleRadius;
		vec3 occluderPosition = texture(positionSampler, occluderSamplingPosition).rgb;
		vec3 v = occluderPosition - pixelPos;
		vec3 vn = normalize(v);
		float d = length(v);
		occlusion += max(0.0, dot(pixelNorm, vn)-bias)*(1.0/(1.0+d))*intensity;
	}
	occlusion /= numSamples*numSamples;

	COLOR_OUT = vec4(vec3(1.0-occlusion), 1.0);
}
