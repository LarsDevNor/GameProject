#version 400 core

#define PARTICLE_POSITION   0           
#define PARTICLE_COLOR      1       
#define COMMON              1             
#define FRAG_COLOR	        0

layout(location = FRAG_COLOR) out vec4 COLOR_OUT;

in vec4 fColor;
in vec2 fOffset;

void main()
{
    COLOR_OUT = vec4(0.0, 1.0, 0.0, 1.0);

    float dist = pow( 1.0/0.2 * length(fOffset), 4.0);
    float inter = smoothstep(0.0, 1.0, dist);
    COLOR_OUT = mix(vec4(1.0, 0.2, 0.0, 1.0), vec4(1.0, 1.0, 1.0, 0.0), inter);

    //COLOR_OUT = vec4(fColor.rgb, 1.0);
}
