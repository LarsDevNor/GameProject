#version 400 core

#define PARTICLE_POSITION   0           
#define PARTICLE_COLOR      1       
#define COMMON              1             
#define FRAG_COLOR	        0

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

//in vec4 gColor;

out vec4 fColor;
out vec2 fOffset;

void main()
{
    mat4 mvp = proj * view; //model;
    float size = 0.2;
    vec2 offset = vec2(0.0, 0.0);
    vec4 eyePos; 

    for ( int i = 0; i < gl_in.length(); ++i )
    {
        eyePos = mvp * gl_in[i].gl_Position; 
        offset = vec2(size, -size);
        gl_Position = eyePos + vec4(offset.xy, 0.0, 0.0);
        fOffset = offset;
		fColor = vec4(1.0, 0.0, 0.0, 1.0);
		EmitVertex();

        eyePos = mvp * gl_in[i].gl_Position; 
        offset = vec2(size, size);
        gl_Position = eyePos + vec4(offset.xy, 0.0, 0.0);
        fOffset = offset;
		fColor = vec4(0.0, 1.0, 0.0, 1.0);
		EmitVertex();

        eyePos = mvp * gl_in[i].gl_Position; 
        offset = vec2(-size, -size);
        gl_Position = eyePos + vec4(offset.xy, 0.0, 0.0);
        fOffset = offset;
		fColor = vec4(0.0, 0.0, 1.0, 1.0);
		EmitVertex();

        eyePos = mvp * gl_in[i].gl_Position; 
        offset = vec2(-size, size);
        gl_Position = eyePos + vec4(offset.xy, 0.0, 0.0);
        fOffset = offset;
		fColor = vec4(0.0, 0.0, 1.0, 1.0);
		EmitVertex();
    }
}