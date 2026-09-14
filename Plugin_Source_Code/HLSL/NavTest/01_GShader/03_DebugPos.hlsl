#include "../../HeaderInc.hlsl"
#include "../../UtilityCS.hlsl"

struct Vertex
{
    float4 pos;
    float4 nom;
    float4 tan;
    float4 uv;
    float4 boneI;
    float4 boneW;
};

struct Object
{
    float4x4 W;
    float4 data;
};

struct Light
{
    float4 pos;
    float4 dir;
};

cbuffer perView : register(b0)
{
    float4 camInfo;
}

cbuffer perLight : register(b1)
{
    Light perLight;
}

StructuredBuffer<PerCamera> perCamBuffer : register(t0);
StructuredBuffer<Vertex> vtxBuffer : register(t1);
//StructuredBuffer<Object> perObjBuffer : register(t2);
StructuredBuffer<float4> targetPos_Buffer : register(t2);
//Texture2D<float4> tex2d : register(t3);

SamplerState sam : register(s0);

struct IA_Out
{
    uint vid : SV_VertexID;
    uint iid : SV_InstanceID;
};

struct VS_Out
{
    float4 posC : SV_POSITION;
    float4 posW : PosW;
    float4 nomW : NomW;
    float4 tanW : TanW;
    float4 uv : Uv;
    uint vid : Vtx_ID;
};

struct RS_Out
{
    float4 posS : SV_POSITION;
    float4 posW : PosW;
    float4 nomW : NomW;
    float4 tanW : TanW;
    float4 uv : Uv;
    uint vid : Vtx_ID;
};

struct PS_Out
{
    float4 color : SV_Target;
};

VS_Out VShader(IA_Out vIn)
{
    VS_Out vOut;
    uint vid = vIn.vid;
    uint iid = vIn.iid;
    
    Vertex vtx = vtxBuffer[vid];
    
    //float4x4 W = perObjBuffer[iid].W;
    float3 tPos = targetPos_Buffer[iid].xyz;
    tPos.y += 2.5f;
    
    float4x4 W;
    W.v4c0 = float4(1.0f, 0.0f, 0.0f, 0.0f);
    W.v4c1 = float4(0.0f, 1.0f, 0.0f, 0.0f);
    W.v4c2 = float4(0.0f, 0.0f, 1.0f, 0.0f);
    W.v4c3 = float4(tPos, 1.0f);
    
    float4x4 CV = perCamBuffer[(uint) (camInfo.x)].CV;
    
    float3 posW = mul(W, float4(vtx.pos.xyz, 1.0f));
    float3 nomW = mul((float3x3) W, vtx.nom.xyz);
    float3 tanW = mul((float3x3) W, vtx.tan.xyz);
    float2 uv = vtx.uv;
                     
    float4 posC = mul(CV, float4(posW, 1.0f));
     
    vOut.posC = posC;
    vOut.posW = float4(posW, 1.0f);
    vOut.nomW = float4(nomW, 0.0f);
    vOut.tanW = float4(tanW, 0.0f);
    vOut.uv = float4(uv, 0.0f, 0.0);
    vOut.vid = vid;
    
    return vOut;
}


PS_Out PShader(RS_Out pIn)
{
    PS_Out pOut;
    
    float3 color = float3(0.0f, 0.0f, 1.0f);
    float2 uv = pIn.uv.xy;
    uint vid = pIn.vid;
       
    float3 L = normalize(perLight.dir.xyz);
    float3 N = normalize(pIn.nomW);
    float NdotL = max(dot(N, L), 0.1f);
    
    //if (8 <= vid && vid < 12)    //front face
    //{
    //    color = float4(1.0f, 0.0f, 0.0f, 1.0f);
    //}
    
    float a = 0.5f;
    //color = 0.5f * color + 0.5f * NdotL * color;
    color = a * color + (1.0f - a) * NdotL * color;
    
   
    
    pOut.color = float4(color, 1.0f);
    return pOut;
}