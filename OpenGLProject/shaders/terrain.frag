#version 400 core

#define POSITION	0
#define NORMAL		3
#define COLOR		6
#define TEXCOORD	7
#define FRAG_COLOR	0

uniform vec3 camPos;

const float spacing = 1.0f;

layout(location = FRAG_COLOR) out vec4 color;

in vec2 fTexCoord;
in vec3 fNormal;
in vec3 fPos;
flat in vec3 fFlatPos;

void main()
{
	color = vec4(fTexCoord.xy, fNormal.z, 1.0);

	vec3 lightDir = vec3(0.0, -1.0, 0.0);
	float lDotN = dot(lightDir, normalize(fNormal));
	color = vec4(vec3(lDotN, lDotN, 0.3)*0.4, 1.0);

	vec2 closeness0 = vec2(1.0, 1.0) - vec2(fract(fPos.x), fract(fPos.z));
	vec2 closeness1 = vec2(fract(fPos.x), fract(fPos.z));

	closeness0.x = pow(closeness0.x, 8);
	closeness1.x = pow(closeness1.x, 8);
	closeness0.y = pow(closeness0.y, 8);
	closeness1.y = pow(closeness1.y, 8);
	
	const float maxDist = 10.0f;
	float distToCam = length(camPos.xz - fPos.xz);
	distToCam =clamp(distToCam, 0.0, maxDist);

	color.rg += (maxDist-distToCam)*(1.0/maxDist) * (closeness0.x + closeness1.x)*2.0;
	color.rg += (maxDist-distToCam)*(1.0/maxDist) * (closeness0.y + closeness1.y)*2.0;

//	color = vec4(fFlatPos, 1.0);

}
