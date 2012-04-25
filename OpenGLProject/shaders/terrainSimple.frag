#version 400 core

#define POSITION	0
#define NORMAL		1
#define COLOR		2
#define TEXCOORD	3
#define FRAG_COLOR	0
#define FRAG_COLOR_AD0 1
#define FRAG_COLOR_AD1 2
#define FRAG_COLOR_AD2 3

uniform sampler2D heightSampler;
uniform float spacing;

layout(location = FRAG_COLOR) out vec4 COLOR_OUT;

in vec2 fTexcoord;
in float fHeight;
in vec3 fPos;

void main()
{
	vec3 lightDir = normalize(vec3(0.0, -1.0, 0.0));

	//float lDotN = dot(lightDir, normalize(fNormal));
    
    float heightCentre = textureOffset(heightSampler, fTexcoord, ivec2(0.0, 0.0)).r; 
    float heightLeft = textureOffset(heightSampler, fTexcoord, ivec2(-1.0, 0.0)).r; 
    float heightRight = textureOffset(heightSampler, fTexcoord, ivec2(1.0, 0.0)).r; 
    float heightBottom = textureOffset(heightSampler, fTexcoord, ivec2(0.0, -1.0)).r; 
    float heightTop = textureOffset(heightSampler, fTexcoord, ivec2(0.0, 1.0)).r; 
    
    vec3 lr = vec3(spacing, heightRight - heightLeft, 0.0);
    vec3 bt = vec3(0.0, heightTop - heightBottom, spacing);
    vec3 normal = normalize(cross(lr, bt));
    float LDotN = max(0.0, dot(lightDir, normal));

    float heightAdjusted = max(0.0, (10.0 + fHeight)*0.015);

    COLOR_OUT = vec4(heightAdjusted, LDotN*LDotN, 0.0, 1.0);

    COLOR_OUT = vec4(vec3(LDotN), 1.0);
}
