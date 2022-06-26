#pragma once
#include "HashBase.h"

#define SHADER_RESOURCE_VIEW(ClassName) CREATE_HASH_CLASS(ClassName, RESOURCE_TYPE::SRV) RESOURCE_PUSH(ClassName, RESOURCE_TYPE::SRV)

/// <summary>
/// ShaderResourceView Resource Struct
/// </summary>
/// 
/// - Shader 내부의 ShaderResourceView Resource와 1:1 대응하는 Struct
/// - Shader에서 설정한 레지스터 번호와 버퍼가 일치하는 한개의 ShaderResourceView
/// - D3DCompiler에서 현재 Shader에서 ShaderResourceView을 선언해 두어도 사용하지 않으면
///   Register Slot에 올리지 않는다..
/// 
/// - 주의점
///  1) 현재 struct의 이름과 변수 순서는 Shader 내부의 UnorderedAccessView와 일치해야한다 (Struct Name을 통해 UnorderedAccessView를 찾기 때문)

///////////////////////////////////////////////////////////////////////////////////////////
// Global ShaderResourceView Resource
///////////////////////////////////////////////////////////////////////////////////////////

SHADER_RESOURCE_VIEW(gDiffuseMap)
SHADER_RESOURCE_VIEW(gNormalMap)
SHADER_RESOURCE_VIEW(gEmissiveMap)
SHADER_RESOURCE_VIEW(gORMMap)
SHADER_RESOURCE_VIEW(gShadowMap)
SHADER_RESOURCE_VIEW(gSsaoMap)
SHADER_RESOURCE_VIEW(gSkyCube)
SHADER_RESOURCE_VIEW(gAnimationBuffer)

///////////////////////////////////////////////////////////////////////////////////////////
// SSAO ShaderResourceView Resource
///////////////////////////////////////////////////////////////////////////////////////////

SHADER_RESOURCE_VIEW(gRandomVecMap)
SHADER_RESOURCE_VIEW(gInputMap)

///////////////////////////////////////////////////////////////////////////////////////////
// Deferred ShaderResourceView Resource
///////////////////////////////////////////////////////////////////////////////////////////

SHADER_RESOURCE_VIEW(gAlbedoRT)
SHADER_RESOURCE_VIEW(gNormalRT)
SHADER_RESOURCE_VIEW(gPositionRT)
SHADER_RESOURCE_VIEW(gEmissiveRT)
SHADER_RESOURCE_VIEW(gNormalDepthRT)

///////////////////////////////////////////////////////////////////////////////////////////
// Terrain ShaderResourceView Resource
///////////////////////////////////////////////////////////////////////////////////////////

SHADER_RESOURCE_VIEW(gDiffuseLayer1)
SHADER_RESOURCE_VIEW(gNormalLayer1)
SHADER_RESOURCE_VIEW(gORMLayer1)
SHADER_RESOURCE_VIEW(gDiffuseLayer2)
SHADER_RESOURCE_VIEW(gNormalLayer2)
SHADER_RESOURCE_VIEW(gORMLayer2)
SHADER_RESOURCE_VIEW(gDiffuseLayer3)
SHADER_RESOURCE_VIEW(gNormalLayer3)
SHADER_RESOURCE_VIEW(gORMLayer3)
SHADER_RESOURCE_VIEW(gDiffuseLayer4)
SHADER_RESOURCE_VIEW(gNormalLayer4)
SHADER_RESOURCE_VIEW(gORMLayer4)

///////////////////////////////////////////////////////////////////////////////////////////
// OIT ShaderResourceView Resource
///////////////////////////////////////////////////////////////////////////////////////////

SHADER_RESOURCE_VIEW(gPieceLinkBuffer)
SHADER_RESOURCE_VIEW(gFirstOffsetBuffer)
SHADER_RESOURCE_VIEW(gBackGround)

///////////////////////////////////////////////////////////////////////////////////////////
// IBL ShaderResourceView Resource
///////////////////////////////////////////////////////////////////////////////////////////

SHADER_RESOURCE_VIEW(gIrradiance_0)
SHADER_RESOURCE_VIEW(gIrradiance_1)
SHADER_RESOURCE_VIEW(gPrefilter_0)
SHADER_RESOURCE_VIEW(gPrefilter_1)
SHADER_RESOURCE_VIEW(gBRDFlut)

///////////////////////////////////////////////////////////////////////////////////////////
// Blur ShaderResourceView Resource
///////////////////////////////////////////////////////////////////////////////////////////

SHADER_RESOURCE_VIEW(gOriginMap)
SHADER_RESOURCE_VIEW(gBlurMap)

///////////////////////////////////////////////////////////////////////////////////////////
// Bloom ShaderResourceView Resource
///////////////////////////////////////////////////////////////////////////////////////////

SHADER_RESOURCE_VIEW(gBloomMap)
SHADER_RESOURCE_VIEW(gDownMap)

///////////////////////////////////////////////////////////////////////////////////////////
// Hierachical Z-Map Occlusion Culling ShaderResourceView Resource
///////////////////////////////////////////////////////////////////////////////////////////

SHADER_RESOURCE_VIEW(gLastMipMap)
SHADER_RESOURCE_VIEW(gHizMap)
SHADER_RESOURCE_VIEW(gColliderBuffer)

///////////////////////////////////////////////////////////////////////////////////////////
// ColorGrading ShaderResourceView Resource
///////////////////////////////////////////////////////////////////////////////////////////

SHADER_RESOURCE_VIEW(gBaseLUT)
SHADER_RESOURCE_VIEW(gBlendLUT)

///////////////////////////////////////////////////////////////////////////////////////////
// OutLine ShaderResourceView Resource
///////////////////////////////////////////////////////////////////////////////////////////

SHADER_RESOURCE_VIEW(gOutLineMap)

///////////////////////////////////////////////////////////////////////////////////////////
// Engine ShaderResourceView Resource
///////////////////////////////////////////////////////////////////////////////////////////

SHADER_RESOURCE_VIEW(gNoise1)
SHADER_RESOURCE_VIEW(gNoise2)
SHADER_RESOURCE_VIEW(gNoise3)
SHADER_RESOURCE_VIEW(gNoise4)
SHADER_RESOURCE_VIEW(gNoise5)
SHADER_RESOURCE_VIEW(gNoise6)
SHADER_RESOURCE_VIEW(gNoise7)
SHADER_RESOURCE_VIEW(gNoise8)
SHADER_RESOURCE_VIEW(gNoise9)
SHADER_RESOURCE_VIEW(gNoise10)
SHADER_RESOURCE_VIEW(gNoise11)
SHADER_RESOURCE_VIEW(gNoise12)
SHADER_RESOURCE_VIEW(gNoise13)
SHADER_RESOURCE_VIEW(gNoise14)
SHADER_RESOURCE_VIEW(gNoise15)
SHADER_RESOURCE_VIEW(gNoise16)


SHADER_RESOURCE_VIEW(gNoiseVolume)
SHADER_RESOURCE_VIEW(gNoiseTexture)

SHADER_RESOURCE_VIEW(gDefalut_LUT)
SHADER_RESOURCE_VIEW(gDefalut_SkyCube)

SHADER_RESOURCE_VIEW(DirectionalLight_Icon)
SHADER_RESOURCE_VIEW(PointLight_Icon)
SHADER_RESOURCE_VIEW(SpotLight_Icon)
SHADER_RESOURCE_VIEW(Particle_Icon)
SHADER_RESOURCE_VIEW(Camera_Icon)

/// Resource Hash 재등록 방지 Define
RESOURCE_DEFINE(DEFINE_SRV)