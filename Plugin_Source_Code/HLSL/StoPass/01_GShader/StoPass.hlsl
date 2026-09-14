#pragma pack_matrix(row_major)

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


cbuffer perObject : register(b0)
{
    Object obj;
}


StructuredBuffer<Vertex> vtxBuffer : register(t0);


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
    	                
    float tfactor = obj.data.x;
    tOut.eFactor[0] = tfactor;
    tOut.eFactor[1] = tfactor;
    tOut.eFactor[2] = tfactor;
    	            
    tOut.iFactor = tfactor;
    
    //tOut.eFactor[0] = tessFactor;
    //tOut.eFactor[1] = tessFactor;
    //tOut.eFactor[2] = tessFactor;
    //	            
    //tOut.iFactor = tessFactor;
        	                
    return tOut;
}

[domain("tri")]
[partitioning("integer")]
//[partitioning("fractional_even")]
    //[partitioning("fractional_odd")]	
    //[partitioning("pow2")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("TShader")]
[maxtessfactor(64.0f)]
HS_Out HShader(
    	            const InputPatch<VS_Out, 3> ip,
    	            uint cid : SV_OutputControlPointID,
    	            uint pid : SV_PrimitiveID)
{
    HS_Out hOut;
    uint id = 0;
    switch (cid)
    {
        case 0:
            id = 0;
            hOut.posL = ip[id].posL;
            hOut.nomL = ip[id].nomL;
            hOut.tanL = ip[id].tanL;
            hOut.uv   = ip[id].uv;
            break;
        case 1:
            id = 1;
            hOut.posL = ip[id].posL;
            hOut.nomL = ip[id].nomL;
            hOut.tanL = ip[id].tanL;
            hOut.uv = ip[id].uv;
            break;
        case 2:
            id = 2;
            hOut.posL = ip[id].posL;
            hOut.nomL = ip[id].nomL;
            hOut.tanL = ip[id].tanL;
            hOut.uv   = ip[id].uv;
            break;
    }
       	           
    return hOut;
}

[domain("tri")]
DS_Out DShader(
    	            TS_Out tfactor,
    	            float3 uvw : SV_DomainLocation,
    	            const OutputPatch<HS_Out, 3> op)
{
    DS_Out dOut;

    float u = uvw.x;
    float v = uvw.y;
    float w = uvw.z;
    
    float3 p100 = op[0].posL.xyz;
    float3 p010 = op[1].posL.xyz;
    float3 p001 = op[2].posL.xyz;
    
    float3 n100 = op[0].nomL.xyz;
    float3 n010 = op[1].nomL.xyz;
    float3 n001 = op[2].nomL.xyz;
    
    float3 t100 = op[0].tanL.xyz;
    float3 t010 = op[1].tanL.xyz;
    float3 t001 = op[2].tanL.xyz;
    
    float3 pos = p100 * u + p010 * v + p001 * w;
    float3 nom = n100 * u + n010 * v + n001 * w;
    float3 tan = t100 * u + t010 * v + t001 * w;
    					       
    dOut.posL = float4(pos, 1.0f);
    dOut.nomL = float4(nom, 0.0f);
    dOut.tanL = float4(tan, 0.0f);
   
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
            float4x4 W = obj.W;
            vout.posW = mul(vin.posL, W);
            vout.nomW = mul(vin.nomL, W);
            vout.tanW = mul(vin.tanL, W);
            vout.uv = vin.uv;
            
            gOut.Append(vout);
        }
    }
    gOut.RestartStrip();
}

