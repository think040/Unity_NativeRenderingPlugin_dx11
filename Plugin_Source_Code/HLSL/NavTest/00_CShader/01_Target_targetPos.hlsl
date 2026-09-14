#include "../../UtilityCS.hlsl"

#define UNIT_COUNT 1024
#define BINARY_COUNT 10

struct Mat
{
    float4x4 value;
};

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


//StructuredBuffer<Mat> trW_Buffer : register(t0);
StructuredBuffer<float4x4> trW_Buffer : register(t0);
StructuredBuffer<float4> circle_Buffer : register(t1);

StructuredBuffer<float4> block_Buffer : register(t2);
StructuredBuffer<int> random_Buffer : register(t3);

Texture2D<float4> nomHTex : register(t4);
Texture2D<float4> alphaTex : register(t5);
Texture2D<float4> holeTex : register(t6);

//StructuredBuffer<float4> block_Buffer : register(t0);
//StructuredBuffer<int> random_Buffer : register(t1);
//
//Texture2D<float4> nomHTex : register(t2);
//Texture2D<float4> alphaTex : register(t3);
//Texture2D<float4> holeTex : register(t4);
//
//
RWStructuredBuffer<float4> targetPos_Buffer : register(u0);
RWStructuredBuffer<Mat> debug_Buffer : register(u1);
//RWStructuredBuffer<float4> targetPosIn_Buffer : register(u2);

//RWStructuredBuffer<Mat> trW_Buffer : register(u2);
//RWStructuredBuffer<float4> circle_Buffer : register(u3);


groupshared bool shTest[UNIT_COUNT];



[numthreads(UNIT_COUNT, 1, 1)]
void CS_TargetPos(uint3 dtid : SV_DispatchThreadID, uint3 gid : SV_GroupID, uint3 gtid : SV_GroupThreadID, uint gIndex : SV_GroupIndex)
{
    uint id0 = gid.x;
    uint id1 = gtid.x;
    int i = 0;
          
    float3 tPos = float3(0.0f, 0.0f, 0.0f);
    float4x4 W;
    float r;
        
    shTest[id1] = false;
    
    int count = info.countInfo.x;
    
    GroupMemoryBarrierWithGroupSync();
    
    if (0 <= id1 && id1 < count)
    {
        tPos = targetPos_Buffer[id0].xyz;
        float4 c0 = circle_Buffer[id0];
        c0.xyz = tPos;
        c0.y = 0.0f;
        c0.w *= 1.15f;
    
        float4 c1 = circle_Buffer[id1];
        c1.y = 0.0f;
        c1.w *= 1.15f;
       
        bool bTest = false;
               
        //if ((id0 != id1) && (ba0 && ba1))
        if ((id0 != id1))
        {
            if (distance(c0.xyz, c1.xyz) <= (c0.w + c1.w))
            {
                bTest = true;
            }
        }
        
        shTest[id1] = bTest;
    }
        
    GroupMemoryBarrierWithGroupSync();

    if (id1 == 0)
    {
        bool bTest = false;
                
        //W = trW_Buffer[id0].value;
        W = trW_Buffer[id0];
        float4 c = circle_Buffer[id0];
        
        r = c.w;
        float3 posW = c.xyz;

        [loop]
        for (i = 0; i < count; i++)
        {
            if (shTest[i] == true)
            {
                bTest = true;
                break;
            }
        }
        
        if (bTest)
        {
            float k = 0.5f; // 0.2f
            k = 0.2f;
            
            float3 offsetVec = mul((float3x3) W, block_Buffer[random_Buffer[id0]].xyz * r * k);
           
            tPos = tPos + offsetVec;
        }
        
        float4x4 T = info.T;
        float4 t1_t0 = info.t1_t0;
        
        uint2 posT = Terrain::GetHMapPos(tPos, T, t1_t0.xy);
        //uint2 posT1 = Terrain::GetHMapPos(posW, T, t1_t0.xy);
        
        if (Terrain::isOutNV(posT, alphaTex) || Terrain::isOutNV_Hole(posT, holeTex))
        {
            //float3 refPos = refTargetPos_Buffer[id0];
                           
            //tPos = 2.0f * refPos - tPos;    //tPos = refPos + (refPos - tPos);      
            
            float3 p0 = posW;
            float3 p1 = tPos;
            p0.y = 0.0f;
            p1.y = 0.0f;
            //if (distance(p0, p1) > 0.001f)
            if (distance(p0, p1) > 0.01f)
            {
                float k = 0.5f;
                //k = 0.4f;
                //k = 0.98f * r;
                //k = 1.0f;
                k = 0.25f;
                tPos = tPos + k * normalize(p0 - p1);
                //tPos = tPos + 1.0f * normalize(p0 - p1);
            }
            else
            {
                tPos = posW;
            }
            
            //tPos = 0.5f * tPos + 0.5f * refPos;
                                    
            posT = Terrain::GetHMapPos(tPos, T, t1_t0.xy);
            //tPos.y = Terrain::GetHeight(posT, nomHTex);
            
            //if (distance(posW, tPos) > 0.1f)
            //{
            //    tPos = tPos + 2.0f * normalize(posW - tPos);
            //}
        }
        //else
        //{
        //    tPos.y = Terrain::GetHeight(posT, nomHTex);
        //}
        
        
        {                   
            float k = 0.0f;
            //k = 0.5f;
            tPos.y = Terrain::GetHeight(posT, nomHTex) + k;
        }
        
        //tPos = float3(10.0f, 20.0f, 30.0f);
        targetPos_Buffer[id0] = float4(tPos, 0.0f);
        
        {
            float4x4 data;
            //data = info.T;
            
            data.v4c0 = info.t1_t0;
            data.v4c1 = float4((float2) posT, 0.0f, info.countInfo.x);
            data.v4c2 = float4(tPos, 0.0f);
            data.v4c3 = circle_Buffer[id0];
            
            //data = trW_Buffer[id0].value;
            
            debug_Buffer[id0].value = data;
        }
    }
}


[numthreads(UNIT_COUNT, 1, 1)]
void CS_TargetPos1(uint3 dtid : SV_DispatchThreadID, uint3 gid : SV_GroupID, uint3 gtid : SV_GroupThreadID, uint gIndex : SV_GroupIndex)
{
    uint id0 = gid.x;
    uint id1 = gtid.x;
    int i = 0;
          
    float3 tPos = float3(0.0f, 0.0f, 0.0f);
    float4x4 W;
    float r;
        
    shTest[id1] = false;
    
    int count = info.countInfo.x;
    
    GroupMemoryBarrierWithGroupSync();
    
    if (0 <= id1 && id1 < count)
    {
        tPos = targetPos_Buffer[id0].xyz;
        //tPos = targetPosIn_Buffer[id0].xyz;
        float4 c0 = circle_Buffer[id0];
        c0.xyz = tPos;
        c0.y = 0.0f;
        c0.w *= 1.15f;
    
        float4 c1 = circle_Buffer[id1];
        c1.y = 0.0f;
        c1.w *= 1.15f;
       
        bool bTest = false;
               
        //if ((id0 != id1) && (ba0 && ba1))
        if ((id0 != id1))
        {
            if (distance(c0.xyz, c1.xyz) <= (c0.w + c1.w))
            {
                bTest = true;
            }
        }
        
        shTest[id1] = bTest;
    }
        
    GroupMemoryBarrierWithGroupSync();

    if (id1 == 0)
    {
        bool bTest = false;
                
        //W = trW_Buffer[id0].value;
        W = trW_Buffer[id0];
        float4 c = circle_Buffer[id0];
        
        r = c.w;
        float3 posW = c.xyz;

        [loop]
        for (i = 0; i < count; i++)
        {
            if (shTest[i] == true)
            {
                bTest = true;
                break;
            }
        }
        
        if (bTest)
        {
            float k = 0.0f;
            //k = 0.125f;         
            k = 0.2f;
            
            float3 offsetVec = mul((float3x3) W, block_Buffer[random_Buffer[id0]].xyz * r * k);
            //float3 offsetVec = block_Buffer[random_Buffer[id0]].xyz * r * k;
            
            //float3 offsetVec = float3(0.0f, 0.0f, -1.0f);
           
            tPos = tPos + offsetVec;
        }
        
        float4x4 T = info.T;
        //T = f4x4I;
        
        float4 t1_t0 = info.t1_t0;
        
        uint2 posT = Terrain::GetHMapPos(tPos, T, t1_t0.xy);
        
        //if (Terrain::isOutNV(posT, alphaTex) || Terrain::isOutNV_Hole(posT, holeTex))
        //{            
        //    float3 p0 = posW;
        //    float3 p1 = tPos;
        //    p0.y = 0.0f;
        //    p1.y = 0.0f;
        //    
        //    if (distance(p0, p1) > 0.005f)           
        //    {
        //        tPos = tPos + 0.1f * normalize(p0 - p1);                               
        //    }
        //    else
        //    {
        //        tPos = posW;
        //    }
        //             
        //    posT = Terrain::GetHMapPos(tPos, T, t1_t0.xy);                                   
        //}          
        
        {                   
            float k = 0.5f;
            tPos.y = Terrain::GetHeight(posT, nomHTex) + k;
        }
                
        targetPos_Buffer[id0] = float4(tPos, 0.0f);
        //targetPosIn_Buffer[id0] = float4(tPos, 0.0f);
        
        {
            float4x4 data;
            //data = info.T;
            
            data.v4c0 = info.t1_t0;
            data.v4c1 = float4((float2) posT, 0.0f, info.countInfo.x);
            data.v4c2 = float4(tPos, 0.0f);
            data.v4c3 = circle_Buffer[id0];
            
            //data = trW_Buffer[id0].value;
            
            debug_Buffer[id0].value = data;
        }
    }
}