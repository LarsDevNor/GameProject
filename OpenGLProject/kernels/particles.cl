
typedef struct 
{
    float4 pos;
    float4 color;
} ParticleRender;

typedef struct
{
    float4 speed;
} ParticleSim;

const sampler_t sampler = CLK_FILTER_LINEAR | CLK_NORMALIZED_COORDS_TRUE; 


kernel void firstKernel(    global ParticleRender* renderPtr, global ParticleSim* simPtr, 
                            read_only image2d_t heightImg, float terrainWidth, float terrainHeight)
{
    const float3 g = (float3)(0.0, -0.0025, 0.0);

    unsigned int idx = get_global_id(0);
    unsigned int x = idx;
 
    /* get particle properties */
    float4 pos  = renderPtr[idx].pos;
    float3 v    = simPtr[idx].speed.xyz;
        
    /* process */ 

    // integrate 
    v += g;
    pos.xyz += v;

    float2 tex = (float2)(pos.x/terrainWidth, pos.z/terrainHeight);
    float h = read_imagef(heightImg, sampler, tex).x;

    // check for collision with terrain 
    if ( pos.y - h < 1e-5 ) 
    {
        pos.y = h + 1e-4;
        v.y *= -1.0;
    }

    float4 color = renderPtr[idx].color;

    /* write back */     
    simPtr[x].speed     = (float4)(v.xyz,0.0);
    renderPtr[x].pos    = pos;

    float4 col0 = (float4)(0.0, 1.0, 0.0, 1.0);
    float4 col1 = (float4)(1.0, 0.0, 0.0, 1.0);

    float alpha = min( fabs(v.y*10.0), 1.0);

    if ( v.y > 0.0)
        renderPtr[x].color = col0;
    else
        renderPtr[x].color = col1;

    //renderPtr[x].color = (1.0-alpha)*col0 + alpha*col1;
    //renderPtr[x].color = smoothstep(col0, col1, 1.0);
}
