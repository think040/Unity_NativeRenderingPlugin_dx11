#include "../../HeaderInc.hlsl"
#include "../../UtilityCS.hlsl"

struct VertexData
{
    float4 posL;
};

struct TerrainData
{
    float4 tileSize;
    float4 terrainSize;
    float4 tileCount;
    float4 layerSize;
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

cbuffer cbCamInfo : register(b0)
{
    float4 camInfo;
}

cbuffer cbTerrrian : register(b1)
{
    TerrainData perTerrain;
}

cbuffer cbObject : register(b2)
{
    Object perObject;
};

cbuffer cbLight : register(b3)
{
    Light perLight;
};

StructuredBuffer<PerCamera> camBuffer : register(t0);
StructuredBuffer<VertexData> vtxBuffer : register(t1);

Texture2D<float4> terrain_Tex_0 : register(t2);
Texture2D<float4> terrain_Tex_1 : register(t3);
Texture2D<float4> terrain_Tex_2 : register(t4);

Texture2D<float4> diffuse_Tex_0 : register(t5);
Texture2D<float4> diffuse_Tex_1 : register(t6);
Texture2D<float4> diffuse_Tex_2 : register(t7);

SamplerState sam : register(s0);

struct IA_Out
{
    uint vid : SV_VertexID;
    uint iid : SV_InstanceID;
};
            
struct VS_Out
{
    float4 posL : SV_POSITION;
};
            
struct TS_Out
{
    float eFactor[4] : SV_TessFactor;
    float iFactor[2] : SV_InsideTessFactor;
};
            
struct HS_Out
{
    float4 posL : SV_POSITION;
};
            
struct DS_Out
{
    float4 posL : SV_POSITION;
    float3 nomL : NORMAL;
    float3 tanL : TANGENT;
    
    float2 uv : UV;
    float2 uv0 : TEXCOORD0;
    float2 uv1 : TEXCOORD1;
    float2 uv2 : TEXCOORD2;
    float2 uv3 : TEXCOORD3;
  
};
                
struct GS_Out
{
    float4 posC : SV_POSITION;
    float3 nomW : NORMAL;
    float3 tanW : TANGENT;
    float3 posW : POSITION;
    
    float2 uv : UV;
    float2 uv0 : TEXCOORD0;
    float2 uv1 : TEXCOORD1;
    float2 uv2 : TEXCOORD2;
    float2 uv3 : TEXCOORD3;
};
           
struct RS_Out
{
    float4 posS : SV_POSITION;
    float3 nomW : NORMAL;
    float3 tanW : TANGENT;
    float3 posW : POSITION;
    
    float2 uv : UV;
    float2 uv0 : TEXCOORD0;
    float2 uv1 : TEXCOORD1;
    float2 uv2 : TEXCOORD2;
    float2 uv3 : TEXCOORD3;
};
            
struct PS_Out
{
    float4 color : SV_Target;
};



VS_Out VShader(
		IA_Out vIn)
{
    VS_Out vOut;
    
    uint vid = vIn.vid;
	
    vOut.posL = float4(vtxBuffer[vid].posL.xyz, 1.0f);
    //vOut.posL = float4(0.0f, 0.0f, 0.0f, 1.0f);
    	            
    return vOut;
}
            

TS_Out TShader(
	                InputPatch<VS_Out, 4> ip,
	                uint pid : SV_PrimitiveID)
{
    TS_Out tOut;
    
    {
        float factor = 4.0f;
        
        tOut.eFactor[0] = factor;
        tOut.eFactor[1] = factor;
        tOut.eFactor[2] = factor;
        tOut.eFactor[3] = factor;
            
        tOut.iFactor[0] = factor;
        tOut.iFactor[1] = factor;
    }
   	                
    return tOut;
}



[domain("quad")]
[partitioning("integer")]
	//[partitioning("fractional_even")]
	//[partitioning("fractional_odd")]	
	//[partitioning("pow2")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("TShader")]
[maxtessfactor(64.0f)]
HS_Out HShader(
	                const InputPatch<VS_Out, 4> ip,
	                uint cid : SV_OutputControlPointID,
	                uint pid : SV_PrimitiveID)
{
    HS_Out hOut;
	            
    hOut.posL = ip[cid].posL;
	           
    return hOut;
}



[domain("quad")]
DS_Out DShader(
	                TS_Out tfactor,
	                float2 uv : SV_DomainLocation,
	                const OutputPatch<HS_Out, 4> op,
                    uint pid : SV_PrimitiveID)
{
    DS_Out dOut;
    
    TerrainData tData = perTerrain;
    float3 terrainSize = tData.terrainSize.xyz;
    float3 tileCount = tData.tileCount.xyz;
    float3 tileSize = tData.tileSize.xyz;
    float4 layerSize = tData.layerSize;
    
    float cx = tileCount.x;
    float cz = tileCount.z;
    
    //float tw = tileSize.x;
    //float th = tileSize.z;
        
    //float wu = terrainSize.x;
    //float hv = terrainSize.z;
    
    //dOut.tileId = GetTileId(pid, (uint) cx);
    
    float u = uv.x;
    float v = uv.y;
       
    float h0 = terrainSize.y;
    float3 p0 = lerp(op[1].posL.xyz, op[0].posL.xyz, v);
    float3 p1 = lerp(op[2].posL.xyz, op[3].posL.xyz, v);
    float3 posL = lerp(p0, p1, u);
    v = 1.0f - v;
    
    uint uj = pid % (uint) cx;
    uint vi = pid / (uint) cx;
    
    //u = ((float) uj * (float) tw + (u * (float) tw)) / wu;
    //v = ((float) vi * (float) th + (v * (float) th)) / hv;
    
    float u1;
    float v1;
    u1 = ((float) uj + u) * (tileSize.x / terrainSize.x);
    v1 = ((float) vi + v) * (tileSize.z / terrainSize.z);
    
    //Test
    //u = ((float) uj + u);
    //v = ((float) vi + v);
        
  
    
    //float k = 1.0f;
    //float du = u / (k * wu);
    //float dv = v / (k * hv);
        
    float k = 1.0f; //0.375f;  
    k = 0.075f;
    float du = k / terrainSize.x;
    float dv = k / terrainSize.z;
        
    
    int i = 0;
    int j = 0;
    
    const int num = 1;
    const int count = 2 * num + 1;
    float h[count][count];
    
    //h[0][0] = h0 * hMap.SampleLevel(sampler_hMap, float2(u - du, v - dv), 0);
    //h[0][1] = h0 * hMap.SampleLevel(sampler_hMap, float2(u + 0 , v - dv), 0);
    //h[0][2] = h0 * hMap.SampleLevel(sampler_hMap, float2(u + du, v - dv), 0);
    //
    //h[1][0] = h0 * hMap.SampleLevel(sampler_hMap, float2(u - du, v + 0 ), 0);
    //h[1][1] = h0 * hMap.SampleLevel(sampler_hMap, float2(u + 0 , v + 0 ), 0);
    //h[1][2] = h0 * hMap.SampleLevel(sampler_hMap, float2(u + du, v + 0 ), 0);
    //
    //h[2][0] = h0 * hMap.SampleLevel(sampler_hMap, float2(u - du, v + dv), 0);
    //h[2][1] = h0 * hMap.SampleLevel(sampler_hMap, float2(u + 0 , v + dv), 0);
    //h[2][2] = h0 * hMap.SampleLevel(sampler_hMap, float2(u + du, v + dv), 0);
    
    Texture2D<float4> nhMap = terrain_Tex_0;
    
    [loop]
    for (i = -num; i <= num; i++)
    {
        for (j = -num; j <= num; j++)
        {
            //h0 = 50.0f;
            h[i + num][j + num] = h0 * nhMap.SampleLevel(sam, float2(u1 + (float) j * du, v1 + (float) i * dv), 0).w;
            //h[i + num][j + num] = h0 * nhMap.Load(int3(0, 0, 0)).w;
            //h[i + num][j + num] = h0 * nhMap[uint2(0.0f, 0.0f)].w;
            //h[i + num][j + num] = h0;
        }
    }
    
 
    
    //float Gu = (h[0][0] - h[0][2]) + 2.0f * (h[1][0] - h[1][2]) + (h[2][0] - h[2][2]);
    //float Gv = (h[0][0] - h[2][0]) + 2.0f * (h[0][1] - h[2][1]) + (h[0][2] - h[2][2]);
    
    float Gu = 0.0f;
    float Gv = 0.0f;
    
    for (i = -num; i <= num; i++)
    {
        for (j = -num; j <= num; j++)
        {
            float ha = 0.5f * (count - (abs(i) + abs(j)));
            Gu += ha * (-sign(j)) * (h[+i + num][+j + num] - h[+i + num][-j + num]);
            Gv += ha * (-sign(i)) * (h[+i + num][+j + num] - h[-i + num][+j + num]);
        }
    }
    
        //for (i = -num; i <= num; i++)
    //{
    //    for (j = -num; j <= num; j++)
    //    {
    //        float ha = 0.5f * (count - (abs(i) + abs(j)));
    //        Gu += ha * (sign(j)) * (h[+i + num][+j + num] - h[+i + num][-j + num]);
    //        Gv += ha * (sign(i)) * (h[+i + num][+j + num] - h[-i + num][+j + num]);
    //    }
    //}
       
                          
    float Gw = sqrt(max(0.0f, 1.0f - Gu * Gu - Gv * Gv));
            
    //posL += float3(0.0f, h[num][num], 0.0f);
    
    //h[0][0] = h[0][0] * 50.0f;
    posL += float3(0.0f, h[0][0], 0.0f);
    //posL.y = 100.0f;
    dOut.posL = float4(posL, 1.0f);
    
    float3 normal = normalize(float3(2.0f * Gu, Gw, 2.0f * Gv));
    //normal = float3(0.0f, 1.0f, 0.0f);
    
    //float3 normal = normalize(float3(1.0f * Gu, Gw, 1.0f * Gv));
    //float3 normal = normalize(float3(10.0f * Gu, Gw, 10.0f * Gv));
    //float3 normal = normalize(float3(2.0f * Gu, 10.0f * Gw, 2.0f * Gv));
    
    //float3 t = normalize(float3(2.0f, 2.0f * Gu, 0.0f));
    //float3 b = normalize(float3(2.0f, 0.0f, 2.0f * Gv));
    //normal = normalize(cross(b, t));
    
    float3 tangent = normalize(float3(1.0f, 0.0f, 0.0f) - normal * dot(normal, float3(1.0f, 0.0f, 0.0f)));
    //tangent = float3(1.0f, 0.0f, 0.0f);
        
    dOut.nomL = normal;
    dOut.tanL = tangent;
                          
    dOut.uv = float2(u1, v1);
    
    dOut.uv0 = terrainSize.xz * float2(u1, v1) / layerSize.x;
    dOut.uv1 = terrainSize.xz * float2(u1, v1) / layerSize.y;
    dOut.uv2 = terrainSize.xz * float2(u1, v1) / layerSize.z;
    dOut.uv3 = terrainSize.xz * float2(u1, v1) / layerSize.w;
   
        
    ////Texture debug
    //{
    //    dOut.posL.y = 0.0f;
    //    dOut.nomL = float3(0.0f, 1.0f, 0.0f);
    //}
             
    //Test   
    //dOut.uv0 = float2(u, v);
    //dOut.uv1 = float2(u, v);
    //dOut.uv2 = float2(u, v);
    //dOut.uv3 = float2(u, v);
    
    //dOut.uv0 = 16.0f * float2(u1, v1);
    //dOut.uv0 = (terrainSize.xz / (4.0f * tileSize.xz)) * float2(u1, v1);
    
    return dOut;
}


[maxvertexcount(3)]
void GShader(triangle DS_Out gin[3], inout TriangleStream<GS_Out> gOut)
{
    uint id_cam = camInfo.x;
    PerCamera cam = camBuffer[id_cam];
    
    float4x4 W = perObject.W;
    float4x4 CV = cam.CV;
    
    for (int i = 0; i < 3; i++)
    {
        GS_Out vertice;
        uint rid = 0;
             
        float4 posW = mul(W, float4(gin[i].posL.xyz, 1.0f));
        float3 nomW = mul((float3x3) W, gin[i].nomL.xyz);
        float3 tanW = mul((float3x3) W, gin[i].tanL.xyz);
        
        vertice.posC = mul(CV, posW);
        vertice.posW = posW.xyz;
        vertice.nomW = nomW;
        vertice.tanW = tanW;
        
        vertice.uv = gin[i].uv;
        vertice.uv0 = gin[i].uv0;
        vertice.uv1 = gin[i].uv1;
        vertice.uv2 = gin[i].uv2;
        vertice.uv3 = gin[i].uv3;
            
        gOut.Append(vertice);
    }
    gOut.RestartStrip();
}



PS_Out PShader(RS_Out pIn)
{
    PS_Out pOut;
    
    float3 color = float3(0.0f, 1.0f, 0.0f);
    
    float3 posW = pIn.posW;
    float3 nomW = normalize(pIn.nomW);
    float2 uv = pIn.uv;
    
    Texture2D<float4> diffuseTex[4];
    diffuseTex[0] = diffuse_Tex_0;
    diffuseTex[1] = diffuse_Tex_1;
    diffuseTex[2] = diffuse_Tex_0;
    diffuseTex[3] = diffuse_Tex_0;
        
    //{
    //    TestObject tObj;
    //    //tObj.dstTex = diffuseTex;
    //    tObj.SetTexture(diffuse_Tex);
    //    //diffuseTex = tObj.dstTex;    
    //    diffuseTex = tObj.tTex.tex;
    //}
  
    Texture2D<float4> waterTex;
    waterTex = diffuse_Tex_2;
    
    Texture2D<float4> alphaTex;
    alphaTex = terrain_Tex_1;
    
    Texture2D<float4> holeTex;
    holeTex = terrain_Tex_2;
    
    //debug
    {        
        //color = test_Tex.Sample(sam, uv).xyz;     //AlphaTex
        //color.g = test_Tex.Sample(sam, uv).w;       //HoleTex
        
        //color.g = test_Tex.Load(float3(uv, 0.0f)).w;
        
        //color = diffuse_Tex[0].Sample(sam, uv).xyz;
        //color = diffuse_Tex[1].Sample(sam, uv).xyz;
        //color = diffuse_Tex[2].Sample(sam, uv).xyz;
        //c
        
        //color = diffuseTex[0].Sample(sam, uv).xyz;
        //color = diffuseTex[1].Sample(sam, uv).xyz;
        //color = diffuseTex[2].Sample(sam, uv).xyz;
        //color = diffuseTex[3].Sample(sam, uv).xyz;        
        //color = waterTex.Sample(sam, uv).xyz;        
        //color = test_Tex.Sample(sam, uv).xyz;
        
        //color = alphaTex.Sample(sam, uv).xyz;
        //color.g = holeTex.Sample(sam, uv).w;
    }
    
    float holeAlpha = holeTex.Sample(sam, pIn.uv).w;
    
    if (holeAlpha <= 0.0f)
    {
        color = waterTex.Sample(sam, pIn.uv0).xyz;
    }
    else
    {
        float alpha[4];
        {
            float4 mask = alphaTex.Sample(sam, pIn.uv);
            alpha[0] = mask.r;
            alpha[1] = mask.g;
            alpha[2] = mask.b;
            alpha[3] = mask.a;
        }
        
        float3 diffuse[4];
        {
            diffuse[0] = diffuseTex[0].Sample(sam, pIn.uv0).xyz;
            diffuse[1] = diffuseTex[1].Sample(sam, pIn.uv1).xyz;
            diffuse[2] = diffuseTex[2].Sample(sam, pIn.uv2).xyz;
            diffuse[3] = diffuseTex[3].Sample(sam, pIn.uv3).xyz;
        }
        
        color = float3(0.0f, 0.0f, 0.0f);
        for (int i = 0; i < 4; i++)
        {
            color += alpha[i] * diffuse[i];
        }
        
        {
            //color = diffuse[0];
            //color = diffuse[1];
            //color = diffuse[2];
            //color = diffuse[3];
        }
    }
    
    //float NdotL = 1.0f;
    
    float3 L = normalize(perLight.dir.xyz);
    float3 N = normalize(pIn.nomW);
    float NdotL = max(dot(N, L), 0.1f);
    
    float a = 0.1f;
    //color = 0.5f * color + 0.5f * NdotL * color;
    color = a * color + (1.0f - a) * NdotL * color;
    
    //{
    //    float3 dirW_light;
    //    float3 posW_Cam;
    //
    //    {      
    //        posW_Cam = viewData.posW.xyz;
    //        dirW_light = lightData.dirW.xyz;
    //        
    //        //dirW_light = float3(0.0f, 1.0f, 0.0f);
    //    }
    //       
    //    //float3 nomW = normalize(pIn.nomW);
    //    //nomW = float3(0.0f, 1.0f, 0.0f);
    //    
    //    {               
    //        
    //        float3 L = float3(0.0f, 0.0f, 0.0f);
    //        {        
    //            L = normalize(-dirW_light);
    //            float3 N = nomW;
    //            NdotL = max(dot(N, L), 0.25f);
    //            //NdotL = abs(dot(N, L));
    //        
    //        }
    //    
    //        float RdotV;
    //        float sFactor = 2.0f;
    //        {
    //            float3 N = nomW;
    //            float3 R = normalize(2 * N * NdotL - L);
    //            float3 V = normalize(posW_Cam - posW);
    //            RdotV = max(dot(R, V), 0.25f);
    //        }
    //       
    //        //color.xyz = NdotL * (color.xyz + RdotV * float3(1.0f, 1.0f, 1.0f));
    //        //color.xyz = NdotL * color.xyz;                        
    //    }
    //}
    
    {
        //color = float3(0.0f, 1.0f, 0.0f);
    }
    
    pOut.color = float4(color, 1.0f);
    
    
    {        
        //pOut.color = test_Tex.Sample(sam, uv).xyzw;
        //pOut.color.g = test_Tex.Sample(sam, uv).w;
    }
            
    return pOut;
}



