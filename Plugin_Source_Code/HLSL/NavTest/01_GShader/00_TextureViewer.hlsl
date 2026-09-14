#include "../../HeaderInc.hlsl"

cbuffer perView : register(b0)
{
    float4 camInfo;
}

cbuffer cMode : register(b1)
{
    float4 mode;
}

StructuredBuffer<PerCamera> perCamBuffer : register(t0);
Texture2D<float4> tex : register(t1);


SamplerState sam : register(s0);

struct IA_Out
{
    uint vid : SV_VertexID; 
};

struct VS_Out
{
    float4 posC : SV_POSITION;    
    float4 uv : Uv;
};

struct RS_Out
{
    float4 posS : SV_POSITION;  
    float4 uv : Uv;
    
};

struct PS_Out
{
    float4 color : SV_Target;
};

VS_Out VShader(IA_Out vIn)
{
    VS_Out vOut;
    uint vid = vIn.vid;
    
    float3 pos[6];
    float2 uv[6];
    
    {
        pos[0] = float3(-1.0f, -1.0f, +0.0f);
        pos[1] = float3(+1.0f, +1.0f, +0.0f);
        pos[2] = float3(+1.0f, -1.0f, +0.0f);
    
        pos[3] = float3(+1.0f, +1.0f, +0.0f);
        pos[4] = float3(-1.0f, -1.0f, +0.0f);
        pos[5] = float3(-1.0f, +1.0f, +0.0f);
    }
    
    ////for_Unity_Texture
    //{
    //    uv[0] = float2(0.0f, 0.0f);
    //    uv[1] = float2(1.0f, 1.0f);
    //    uv[2] = float2(1.0f, 0.0f);
    //
    //    uv[3] = float2(1.0f, 1.0f);
    //    uv[4] = float2(0.0f, 0.0f);
    //    uv[5] = float2(0.0f, 1.0f);
    //}
    
    ////for_Raw_Texture   //invert_Y_of_UnityTexture
    {
        uv[0] = float2(0.0f, 1.0f);
        uv[1] = float2(1.0f, 0.0f);
        uv[2] = float2(1.0f, 1.0f);
    
        uv[3] = float2(1.0f, 0.0f);
        uv[4] = float2(0.0f, 1.0f);
        uv[5] = float2(0.0f, 0.0f);
    }
    
    float4x4 M;
    M[0] = float4(+1.0f, +0.0f, +0.0f, +0.0f);
    M[1] = float4(+0.0f, +1.0f, +0.0f, +0.0f);
    M[2] = float4(+0.0f, +0.0f, +1.0f, +0.0f);
    M[3] = float4(+0.0f, +0.0f, +0.0f, +1.0f);
    
    {
        uint id_cam = camInfo.x;
        PerCamera cam = perCamBuffer[id_cam];
        bool bSceneView = cam.data[1] == 2 ? true : false;
    
        if (bSceneView)
        {
            M[1].y = -1.0f;
        }
    }
          
    vOut.posC = mul(M, float4(pos[vid], 1.0f));
    
    vOut.uv = float4(uv[vid], 0.0f, 0.0);
    
    return vOut;
}


PS_Out PShader(RS_Out pIn)
{
    PS_Out pOut;
    
    float3 color = float3(0.0f, 1.0f, 0.0f);
    float2 uv = pIn.uv.xy;
       
    //if(mode.x == 0)
    {
        color = tex.Sample(sam, uv).xyz;
    } 
   
   
    
    ////hole_texture
    //{
    //    if(color.x > 0.0f)
    //    {
    //        color.x = 1.0f;
    //    }
    //    else
    //    {
    //        color.x = 0.0f;
    //    }
    //}
    
    //{
    //    color = float3(color.x / 50.0f, 0.0f, 0.0f);
    //}
  
    pOut.color = float4(color, 1.0f);
    return pOut;
}