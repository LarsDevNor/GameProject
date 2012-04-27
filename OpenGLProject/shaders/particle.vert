#version 400 core

#define PARTICLE_POSITION   0           
#define PARTICLE_COLOR      1       
#define COMMON              1             
#define FRAG_COLOR	        0

layout(location = PARTICLE_POSITION) in vec4 pos;
layout(location = PARTICLE_COLOR)	in vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

//out vec4 fColor;
out vec4 gPos;
out vec4 gColor;

void main()
{
#if 1 
    //gl_Position = pos;
    gPos = pos;
    gColor = color;	
#else
    mat4 mvp = proj * view; //model;
	vec4 posMVP = mvp * pos;
	gl_Position = posMVP;
    //gl_PointSize = 1.0 / posMVP.w;
    fColor = color;	
#endif 
}