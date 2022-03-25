#ifndef SKIN_MESH
    #define SKIN_MESH
#endif

#include "Input_Header.hlsli"
#include "Instance_Header.hlsli"

cbuffer cbInstanceStaticMeshID : register(b0)
{
    float4x4 gViewProj           : packoffset(c0);
    float4x4 gBoneTransforms[96] : packoffset(c4);
};

MeshIDVertexOut ID_SkinMesh_Instance_VS(MeshPosVertexIn vin, MeshIDInstanceIn instance)
{
    MeshIDVertexOut vout;
    
    float3 posL = float3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 4; ++i)
    {
        posL += vin.BoneWeights1[i] * mul(gBoneTransforms[vin.BoneIndices1[i]], float4(vin.PosL, 1.0f)).xyz;
    }
	
    for (int j = 0; j < 4; ++j)
    {
        posL += vin.BoneWeights2[j] * mul(gBoneTransforms[vin.BoneIndices2[j]], float4(vin.PosL, 1.0f)).xyz;
    }
    
    float3 posW = mul(instance.World, float4(posL, 1.0f)).xyz;
    
    vout.PosH = mul(gViewProj, float4(posW, 1.0f));
    vout.HashColor = instance.HashColor;
    
    return vout;
};