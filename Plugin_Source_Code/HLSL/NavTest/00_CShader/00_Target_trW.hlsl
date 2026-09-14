#include "../../UtilityCS.hlsl"

//struct Mat
//{
//    float4x4 value;
//};

struct TargetComputeInfo
{
    float4 t1_t0;
    float4x4 T;
    float4 countInfo;
};

cbuffer cbTargetComputeInfo
{
    TargetComputeInfo info : register(b0);
};

//StructuredBuffer<Mat> trM_Buffer : register(t0);
StructuredBuffer<float4x4> trM_Buffer : register(t0);
Texture2D<float4> alphaTex : register(t1);

//RWStructuredBuffer<Mat> trW_Buffer : register(u0);
RWStructuredBuffer<float4x4> trW_Buffer : register(u0);
RWStructuredBuffer<float4> circle_Buffer : register(u1);
RWStructuredBuffer<float4> terrainArea_Buffer : register(u2);

[numthreads(64, 1, 1)] //4 * 64 = 256 (4, 1, 1)
void CS_TrW(uint3 dtid : SV_DispatchThreadID, uint3 gid : SV_GroupID, uint3 gtid : SV_GroupThreadID, uint gidx : SV_GroupIndex)
{
    uint id = gid.x * 64 + gtid.x;
    
    //float4x4 M = trM_Buffer[id].value;
    float4x4 M = trM_Buffer[id];
    float4 circle = circle_Buffer[id];
     
    float3 pos = M.v3c0;
    float4 rot = M.v4c1;
    float3 sca = M.v3c2;
    
    //float3 pos = M.v3r0;
    //float4 rot = M.v4r1;
    //float3 sca = M.v3r2;
    
    circle.w = M.v3c3.x;
       
    {
        float4x4 W = Transform::GetW(pos, rot, sca);
        
        //trW_Buffer[id].value = W;
        trW_Buffer[id] = W;
    }
    
    {
        float3 c = pos;
        float r = circle.w;
        
        circle_Buffer[id] = float4(c, r);
    }
    
    {
        float4x4 T = info.T;
        float4 t1_t0 = info.t1_t0;
        uint2 posT = Terrain::GetHMapPos(pos, T, t1_t0.xy);
        float4 tArea = Terrain::GetTerrainArea(posT, alphaTex);
        
        terrainArea_Buffer[id] = tArea;
    }
    
}
