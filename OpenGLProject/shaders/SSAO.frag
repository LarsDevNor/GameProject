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
uniform sampler2D positionSampler;
uniform sampler2D depthSampler;

layout(location = FRAG_COLOR) out vec4 COLOR_OUT;

in vec2 fTexcoord;

#define PI 3.14159265

void main()
{
	vec3 color 			= texture(colorSampler, fTexcoord).rgb;
	vec3 pixelNorm		= 2.0*normalize(texture(normalSampler, fTexcoord).rgb) - 1.0;
	vec3 pixelPos 		= texture(positionSampler, fTexcoord).rgb;

	float sampleRadius = 0.05 / pixelPos.z; // calc based on screensize ? make uniform.. 
	const float bias = 0.1;
	const float intensity = 1.0;
	const int numSamples = 16;
	float occlusion = 0.0;
	for ( int i = 0; i < numSamples; ++i)
	for ( int j = 0; j < numSamples; ++j)
	{
		vec2 offset = vec2(sin(i*PI*2.0/(numSamples-1)), cos(j*PI*2.0/(numSamples-1)));
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
