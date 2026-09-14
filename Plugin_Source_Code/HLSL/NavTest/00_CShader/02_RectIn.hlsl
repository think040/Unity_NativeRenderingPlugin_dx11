#include "../../UtilityCS.hlsl"

struct RectInInfo
{
    float4 rect;
    float4 data;
};

cbuffer cRectInInfo
{
    RectInInfo rectInInfo : register(b0);
};

StructuredBuffer<float4x4> SCV_Buffer : register(t0);
StructuredBuffer<float4> pos_Buffer : register(t1);

RWStructuredBuffer<int> inRect_Buffer : register(u0);

[numthreads(64, 1, 1)]
void CS_RectIn(uint3 dtid : SV_DispatchThreadID, uint3 gid : SV_GroupID, uint3 gtid : SV_GroupThreadID, uint gidx : SV_GroupIndex)
{
    uint idx = gid.x * 64 + gtid.x;
    
    RectInInfo info = rectInInfo;
        
    float4 rect = info.rect;
    int idxStart = info.data.x;
    int idxEnd = info.data.y;
    
    int inRect = 0;
       
    {
        if (idxStart <= idx && idx <= idxEnd)
        {
            float3 posS;
            float3 posW = pos_Buffer[idx].xyz;
        
            float4x4 S = SCV_Buffer[0];
            float4x4 CV = SCV_Buffer[1];
        
            float4 vec = mul(CV, float4(posW, 1.0f));
            vec = (1.0f / vec.w) * vec;
            posS = mul(S, vec).xyz;
        
        
            {
                float x = rect.x;
                float y = rect.y;
                float w = rect.z;
                float h = rect.w;
            
                if (x <= posS.x && posS.x <= x + w)
                {
                    if (y <= posS.y && posS.y <= y + h)
                    {
                        inRect = 1;
                    }
                }
            }
            
            //inRect = 850407;
        }
    }
    
    //inRect = 850407;
           
    inRect_Buffer[idx] = inRect;
}
