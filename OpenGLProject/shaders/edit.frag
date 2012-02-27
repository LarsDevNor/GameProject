#version 400 core

#define POSITION	0
#define NORMAL		1
#define COLOR		2
#define TEXCOORD	3
#define FRAG_COLOR	0
#define FRAG_COLOR_AD0 1
#define FRAG_COLOR_AD1 2
#define FRAG_COLOR_AD2 3

uniform sampler2D textureSampler;
uniform vec2 editPos;

layout(location = FRAG_COLOR) out float COLOR_OUT;

in vec2 fTexcoord;

void main()
{
	float value = texture(textureSampler, fTexcoord).r;

/*    if ( length(fTexcoord.xy - editPos) < 0.1 )
        COLOR_OUT = vec4(vec3(value), 1.0);
    else
    	COLOR_OUT = vec4(vec3(value), 1.0);
*/
    //COLOR_OUT = vec4(value, 0.0, 0.0, 0.0);
    float strength = 1.0;

    float threshold = 0.2;
    float proximity = 1.0/threshold * ( threshold - min(threshold,length(fTexcoord - editPos)) );
    COLOR_OUT = value+pow(strength*proximity, 4);

  //  COLOR_OUT = value + strength*sin(proximity * 3.14/2.0);

    //COLOR_OUT = 
    //COLOR_OUT = value;
    //COLOR_OUT = fTexcoord.x*100.0;

    //COLOR_OUT = fTexcoord.x * 100.0;

    //COLOR_OUT = vec4(value, 0.0, 0.0, 0.0);

   //COLOR_OUT = vec4(fTexcoord.x*5.0);

   //COLOR_OUT = vec4(1.0, 0.0, 0.0, 0.0);
    
  /*  if ( length(fTexcoord.xy - vec2(0.0, 0.0)) < 0.1 )
        COLOR_OUT = vec4(50.0, 0.0, 0.0, 0.0);
    else
        COLOR_OUT = vec4(0.0, 0.0, 0.0, 0.0);
        */
}
