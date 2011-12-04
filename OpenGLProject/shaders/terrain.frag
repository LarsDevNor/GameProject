#version 400 core

#define POSITION	0
#define NORMAL		3
#define COLOR		6
#define TEXCOORD	7
#define FRAG_COLOR	0

layout(location = FRAG_COLOR) out vec4 color;

in vec2 fTexCoord;
in vec3 fNormal;

void main()
{
	color = vec4(fTexCoord.xy, fNormal.z, 1.0);

	vec3 lightDir = vec3(0.0, -1.0, 0.0);
	float lDotN = dot(lightDir, normalize(fNormal));
	color = vec4(vec3(lDotN), 1.0);
}
