#pragma pack_matrix(row_major)

RWTexture3D<float4> tex3d : register(u0);

[numthreads(8, 8, 8)]
void CS_Compute_Tex(uint3 dtid : SV_DispatchThreadID, uint3 gid : SV_GroupID, uint3 gtid : SV_GroupThreadID, uint gidx : SV_GroupIndex)
{
    //tex3d[gtid] = float4(0.0f, 0.0f, 0.0f, 0.0f);
    //tex3d[gtid] = float4(100.0f, 100.0f, 100.0f, 100.0f);
    
    //tex3d[gtid] = float4(gtid.z, gtid.y, gtid.x, 0.0f);
    
    float4 data = tex3d[gtid];
    //tex3d[gtid] = data + float4(100.0f, 100.0f, 100.0f, 100.0f);
    
    //float4 data = float4(gtid.zyx, 0.0f);
    tex3d[gtid] = -data;
    //tex3d[gtid] = data;
}