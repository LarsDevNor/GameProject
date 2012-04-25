
typedef struct 
{
    float4 pos;
    float4 color;
} Particle;

const sampler_t sampler = CLK_FILTER_LINEAR | CLK_NORMALIZED_COORDS_TRUE; 

#if 1
kernel void firstKernel(global Particle* particles, read_only image2d_t heightImg, float terrainWidth, float terrainHeight)
{
    unsigned int idx = get_global_id(0);
    unsigned int x = idx;
    
    float4 pos = particles[idx].pos;
    float4 v = (float4)(0.1, 0.0, 0.3, 0.0);
    //pos += v;

    float2 tex = (float2)(pos.x/terrainWidth, pos.z/terrainHeight);
    
    float heightValue = read_imagef(heightImg, sampler, tex).x;
    pos.y = heightValue + 2.0;
    particles[x].pos = pos;

    float4 color = particles[idx].color;
    particles[x].color = (float4)(0.0, 1.0, 0.0, 0.0);
}
#else
kernel void firstKernel(global float* vertices, read_only image2d_t heightImg, float terrainWidth, float terrainHeight)
{
    unsigned int idx = get_global_id(0);
    unsigned int x   = idx*7;
    Particle* particlePtr = (Particle*)&vertices[idx*7];

    float3 pos = (float3)(vertices[x+0], vertices[x+1], vertices[x+2]);
    pos += (float3)(0.1, 0.0, 0.3);
    float3 normPos = (float3)(pos.x/terrainWidth, pos.y, pos.z/terrainHeight);
    //pos.x += 0.1;
    
    const sampler_t sampler = CLK_FILTER_LINEAR | CLK_NORMALIZED_COORDS_TRUE; 
    float heightValue = read_imagef(heightImg, sampler, normPos.xz).x;
    
    vertices[x+0] = pos.x;
    vertices[x+1] = heightValue * 1.0 + 1.0;
    vertices[x+2] = pos.z;

    float4 color = (float4)(0.0, 1.0, 0.0, 1.0);
#if 0
    float4* float4Ptr = (float4*)&vertices[x+3];
    *float4Ptr = color;
#else
    vertices[x+3] = color.x;
    vertices[x+4] = color.y;
    vertices[x+5] = color.z;
    vertices[x+6] = color.w;
#endif 
    /*
    particlePtr->pos = pos;
    particlePtr->color = color;
    */
}
#endif 