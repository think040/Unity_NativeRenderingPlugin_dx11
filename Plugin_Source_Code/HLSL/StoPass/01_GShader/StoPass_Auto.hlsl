#include "../../HeaderInc.hlsl"

struct Light
{
    float4 pos;
    float4 dir;
};


struct RenderInfo
{
    float4 data;
};

cbuffer perView : register(b0)
{
    float4 camInfo;
}

cbuffer perLight : register(b1)
{
    Light light;
}

cbuffer renderInfo : register(b2)
{
    RenderInfo renInfo;
}

StructuredBuffer<PerCamera> perCamBuffer : register(t0);
Texture2D<float4> tex2d_0 : register(t1);
Texture2D<float4> tex2d_1 : register(t2);

SamplerState sam : register(s0);

struct IA_Out
{
    float4 posW : PosW;
    float4 nomW : NomW;
    float4 tanW : TanW;
    float4 uv : Uv;
};

struct VS_Out
{
    float4 posC : SV_POSITION;
    float4 posW : PosW;
    float4 nomW : NomW;
    float4 tanW : TanW;
    float4 uv : Uv;
};

struct RS_Out
{
    float4 posS : SV_POSITION;
    float4 posW : PosW;
    float4 nomW : NomW;
    float4 tanW : TanW;
    float4 uv : Uv;
    
};

struct PS_Out
{
    float4 color : SV_Target;
};

VS_Out VShader(IA_Out vIn)
{
    VS_Out vOut;
   
    float3 posW = vIn.posW.xyz;
    float3 nomW = vIn.nomW.xyz;
    float3 tanW = vIn.tanW.xyz;
    float2 uv = vIn.uv;
     
    float4 posC = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    uint id_cam = camInfo.x;
    PerCamera cam = perCamBuffer[id_cam];
    
#if defined(ROW_MAJOR)
        posC = mul(mul(float4(posW, 1.0f), cam.V), cam.C);
#elif defined(COLUMN_MAJOR)
        posC = mul(cam.C, mul(cam.V, float4(posW, 1.0f)));
#else

#endif       
    
    vOut.posC = posC;
    vOut.posW = float4(posW, 1.0f);
    vOut.nomW = float4(nomW, 0.0f);
    vOut.tanW = float4(tanW, 0.0f);
    vOut.uv = float4(uv, 0.0f, 0.0);
    
    return vOut;
}


PS_Out PShader(RS_Out pIn)
{
    PS_Out pOut;
    
    float3 color = float3(0.0f, 1.0f, 0.0f);
    float2 uv = pIn.uv.xy;
    
    uint id = renInfo.data.x;
    
#if defined(USE_TEXTURE)
    if(id == 0)
    {
        color = tex2d_0.Sample(sam, uv).xyz;
    }
    else if (id == 1)
    {
        color = tex2d_1.Sample(sam, uv).xyz;
    }
   
#endif
    
    float3 L = normalize(light.dir);
    float3 N = normalize(pIn.nomW);
    float NdotL = max(dot(N, L), 0.1f);
    
    float a = 0.1f;
    //color = 0.5f * color + 0.5f * NdotL * color;
    color = a * color + (1.0f - a) * NdotL * color;
    //color = NdotL * color;
    
    pOut.color = float4(color, 1.0f);
    return pOut;
}