#include "../../HeaderInc.hlsl"

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

struct RenderInfo
{
    float4 data;
};


cbuffer renderInfo : register(b0)
{
    RenderInfo renInfo;
}

StructuredBuffer<Vertex> vtxBuffer : register(t0);
StructuredBuffer<Object> objBuffer : register(t1);

struct IA_Out
{
    uint vid : SV_VertexID;
    uint iid : SV_InstanceID;
};

struct VS_Out
{
    float4 posL : PosL;
    float4 nomL : NomL;
    float4 tanL : TanL;
    float4 uv : Uv;
};

struct TS_Out
{
    float eFactor[3] : SV_TessFactor;
    float iFactor : SV_InsideTessFactor;
};


struct HS_Out
{
    float4 posL : PosL;
    float4 nomL : NomL;
    float4 tanL : TanL;
    float4 uv : Uv;
};
                
struct DS_Out
{
    float4 posL : PosL;
    float4 nomL : NomL;
    float4 tanL : TanL;
    float4 uv : Uv;
};

struct GS_Out
{
    float4 posW : PosW;
    float4 nomW : NomW;
    float4 tanW : TanW;
    float4 uv : Uv;
};


VS_Out VShader(IA_Out vIn)
{
    VS_Out vOut;
    uint vid = vIn.vid;      
    uint iid = vIn.iid;
    
    Vertex vtx = vtxBuffer[vid];
    float3 posL = vtx.pos.xyz;
    float3 nomL = vtx.nom.xyz;
    float3 tanL = vtx.tan.xyz;
    float2 uv = vtx.uv;
   
    vOut.posL = float4(posL, 1.0f);
    vOut.nomL = float4(nomL, 0.0f);
    vOut.tanL = float4(tanL, 0.0f);
    vOut.uv = float4(uv, 0.0f, 0.0);
    
    return vOut;
}


TS_Out TShader(
    	            InputPatch<VS_Out, 3> ip,
    	            uint pid : SV_PrimitiveID)
{
    TS_Out tOut;
    	                
    //float tfactor = obj.data.x;
    
    uint id = (uint) renInfo.data.x;   
    float tfactor = objBuffer[id].data.x;
    //float tfactor = 3.0f;
       
    {
        tOut.eFactor[0] = tfactor;
        tOut.eFactor[1] = tfactor;
        tOut.eFactor[2] = tfactor;
    	            
        tOut.iFactor = tfactor;
    }
    
    
    //tOut.eFactor[0] = tessFactor;
    //tOut.eFactor[1] = tessFactor;
    //tOut.eFactor[2] = tessFactor;
    //	            
    //tOut.iFactor = tessFactor;
        	                
    return tOut;
}


float4 getEdgeCPoint(InputPatch<VS_Out, 3> ip, int i, int j)
{
    float3 ni = normalize(ip[i].nomL);
    
    float3 outPos = float3(0.0f, 0.0f, 0.0f);
    outPos = (2.0f * ip[i].posL.xyz + ip[j].posL.xyz) / 3.0f + (-0.3f) * dot((ip[j].posL.xyz - ip[i].posL.xyz), ni) * ni; // -0.15
    return float4(outPos, 1.0f);
}

float4 getFaceCPoint(InputPatch<VS_Out, 3> ip)
{
    float3 outPos = float3(0.0f, 0.0f, 0.0f);
    float3 E = (
					getEdgeCPoint(ip, 0, 1) + getEdgeCPoint(ip, 1, 0) +
					getEdgeCPoint(ip, 1, 2) + getEdgeCPoint(ip, 2, 1) +
					getEdgeCPoint(ip, 2, 0) + getEdgeCPoint(ip, 0, 2)
					) / 6.0f;
    float3 V = (
					ip[0].posL.xyz + ip[1].posL.xyz + ip[2].posL.xyz
					) / 3.0f;
    outPos = E + (E - V) / 2.0f;

    return float4(outPos, 1.0f);
}

float4 getEdgeCNormal(InputPatch<VS_Out, 3> ip, int i, int j)
{
    float3 outNormal = float3(1.0f, 1.0f, 1.0f);
    
    float3 a = ip[i].nomL.xyz + ip[j].nomL.xyz;
    float3 b = ip[j].posL.xyz - ip[i].posL.xyz;

    outNormal = a - 2.0f * (dot(a, b) / dot(b, b) * b);
    //outNormal = a;
    outNormal = normalize(outNormal);

    return float4(outNormal, 0.0f);
}


[domain("tri")]
[partitioning("integer")]
//[partitioning("fractional_odd")]
//[partitioning("fractional_even")]
//[partitioning("pow2")]
[outputtopology("triangle_cw")]
//[outputtopology("triangle_ccw")]
[outputcontrolpoints(13)]
[patchconstantfunc("TShader")]
[maxtessfactor(64.0f)]
			HS_Out HShader(
				const InputPatch<VS_Out, 3> ip,
				uint i : SV_OutputControlPointID,
				uint PatchID : SV_PrimitiveID)
{
    HS_Out hsOut;
    
    hsOut.posL = float4(0.0f, 0.0f, 0.0f, 0.0f);
    hsOut.nomL = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    VS_Out hsIn = ip[0];
       
    hsOut.posL = hsIn.posL;
    hsOut.nomL = hsIn.nomL;
    hsOut.tanL = hsIn.tanL;
    
    hsOut.uv = hsIn.uv;    
       
    int j;
    switch (i)
    {
		//vertex control position normal
        case 0:
            j = 0;
            hsIn = ip[j];
        
            hsOut.posL = hsIn.posL;
            hsOut.nomL = hsIn.nomL;
            hsOut.tanL = hsIn.tanL;
        
            hsOut.uv = hsIn.uv;            
            break;
        case 1:
            j = 1;
            hsIn = ip[j];
        
            hsOut.posL = hsIn.posL;
            hsOut.nomL = hsIn.nomL;
            hsOut.tanL = hsIn.tanL;
        
            hsOut.uv = hsIn.uv;            
            break;
        case 2:
            j = 2;
            hsIn = ip[j];
        
            hsOut.posL = hsIn.posL;
            hsOut.nomL = hsIn.nomL;
            hsOut.tanL = hsIn.tanL;
            
            hsOut.uv = hsIn.uv;            
            break;
					//Edge control position 0 , 1
        case 3:
            hsOut.posL = getEdgeCPoint(ip, 0, 1);
            break;
        case 4:
            hsOut.posL = getEdgeCPoint(ip, 1, 0);
            break;
					//Edge control position 1 , 2
        case 5:
            hsOut.posL = getEdgeCPoint(ip, 1, 2);
            break;
        case 6:
            hsOut.posL = getEdgeCPoint(ip, 2, 1);
            break;
					//Edge control position 2 , 0
        case 7:
            hsOut.posL = getEdgeCPoint(ip, 2, 0);
            break;
        case 8:
            hsOut.posL = getEdgeCPoint(ip, 0, 2);
            break;
					//Face control position 0 , 1
        case 9:
            hsOut.posL = getFaceCPoint(ip);
            break;
					//Edge control normal 0 , 1 , 2
        case 10:
            hsOut.nomL = getEdgeCNormal(ip, 0, 1);
            break;
        case 11:
            hsOut.nomL = getEdgeCNormal(ip, 1, 2);
            break;
        case 12:
            hsOut.nomL = getEdgeCNormal(ip, 2, 0);
            break;

    }

    return hsOut;
}


[domain("tri")]
DS_Out DShader(
				const OutputPatch<HS_Out, 13> op,
				float3 bc : SV_DomainLocation,
				TS_Out tsOut,
                uint pid : SV_PrimitiveID)
{
    DS_Out dOut;

    float u = bc.x;
    float v = bc.y;
    float w = bc.z;

				//Control Point
    float3 p300 = op[0].posL.xyz;
    float3 p030 = op[1].posL.xyz;
    float3 p003 = op[2].posL.xyz;

    float3 p210 = op[3].posL.xyz;
    float3 p120 = op[4].posL.xyz;

    float3 p021 = op[5].posL.xyz;
    float3 p012 = op[6].posL.xyz;

    float3 p102 = op[7].posL.xyz;
    float3 p201 = op[8].posL.xyz;

    float3 p111 = op[9].posL.xyz;

				//Control Normal
    float3 n200 = op[0].nomL.xyz;
    float3 n020 = op[1].nomL.xyz;
    float3 n002 = op[2].nomL.xyz;

    float3 n110 = op[10].nomL.xyz;
    float3 n011 = op[11].nomL.xyz;
    float3 n101 = op[12].nomL.xyz;

    float3 pos =
					p300 * pow(u, 3) + p030 * pow(v, 3) + p003 * pow(w, 3) +
					3.0f * p210 * pow(u, 2) * v + 3.0f * p120 * u * pow(v, 2) +
					3.0f * p021 * pow(v, 2) * w + 3.0f * p012 * v * pow(w, 2) +
					3.0f * p102 * pow(w, 2) * u + 3.0f * p201 * w * pow(u, 2) +
					6.0f * p111 * u * v * w;

    float3 nom =
					n200 * pow(u, 2) + n020 * pow(v, 2) + n002 * pow(w, 2) +
					2.0f * n110 * u * v +
					2.0f * n011 * v * w +
					2.0f * n101 * w * u;
    
    //float3 nom =
	//				n200 * u + n020 * v + n002 * w;
    
    HS_Out p0 = op[0];
    HS_Out p1 = op[1];
    HS_Out p2 = op[2];
        
    float3 tan = p0.tanL.xyz * u + p1.tanL.xyz * v + p2.tanL.xyz * w;
    float4 uv = p0.uv * u + p1.uv * v + p2.uv * w;    
        
    dOut.posL = float4(pos, 1.0f);
    dOut.nomL = float4(nom, 0.0f);
    dOut.tanL = float4(tan, 0.0f);
    
    dOut.uv = uv;        

    return dOut;
}


[maxvertexcount(3)]
void GShader(triangle DS_Out gIn[3], inout TriangleStream<GS_Out> gOut)
{
    for (int i = 0; i < 3; i++)
    {
        GS_Out vout;
        DS_Out vin = gIn[i];
       
        {          
            //float4x4 W = obj.W;
            
            uint id = (uint) renInfo.data.x;
            float4x4 W = objBuffer[id].W;
            
            vout.posW = float4(0.0f, 0.0f, 0.0f, 0.0f);
            vout.nomW = float4(0.0f, 0.0f, 0.0f, 0.0f);
            vout.tanW = float4(0.0f, 0.0f, 0.0f, 0.0f);
            
            //if(renInfo.data.y == 0.0f)
            {
#if defined(ROW_MAJOR)
            vout.posW = mul(float4(vin.posL.xyz, 1.0f), W);
            vout.nomW = float4(mul(vin.nomL.xyz, (float3x3) W), 0.0f);
            vout.tanW = float4(mul(vin.tanL.xyz, (float3x3) W), 0.0f);
#elif defined(COLUMN_MAJOR)
            vout.posW = mul(W, float4(vin.posL.xyz, 1.0f));
            vout.nomW = float4(mul((float3x3) W, vin.nomL.xyz), 0.0f);
            vout.tanW = float4(mul((float3x3) W, vin.tanL.xyz), 0.0f);
#else

#endif
            }
                             
            vout.uv = vin.uv;
            
            gOut.Append(vout);
        }
    }
    gOut.RestartStrip();
}