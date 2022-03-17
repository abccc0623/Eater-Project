#include "DirectDefine.h"
#include "EngineData.h"
#include "RenderData.h"

RenderData::RenderData()
{
	// Renderer 전용 Mesh Data 생성..
	m_MeshBuffer = new MeshRenderBuffer();
	m_MaterialBuffer = new MaterialRenderBuffer();
}

RenderData::~RenderData()
{
	Release();
}

void RenderData::ConvertData(MeshData* originMesh)
{
	// Origin Mesh Data 설정..
	m_OriginData = originMesh;

	// Mesh Data 설정..
	m_ObjectData = originMesh->Object_Data;
	m_ParticleData = originMesh->Particle_Data;
	m_ColliderData = originMesh->Collider_Data;

	// Mesh Buffer Data 변환..
	ConvertMeshBuffer(originMesh->Mesh_Buffer);

	// Material Data 변환..
	ConvertMaterial(originMesh->Material_Buffer);

	// Obejct Type에 따른 추가 변환 작업..
	switch (m_ObjectData->ObjType)
	{
	case OBJECT_TYPE::SKINNING:
	{
		m_BoneOffsetTM = &originMesh->BoneOffsetTM;
	}
		break;
	case OBJECT_TYPE::TERRAIN:
	{
		m_TerrainData = new TerrainRenderData();

		for (MaterialBuffer* layer : originMesh->Terrain_Data->Material_List)
		{
			// 새로운 Material 생성..
			MaterialRenderBuffer* layerMaterial = new MaterialRenderBuffer();

			// Material Data 변환..
			ConvertMaterial(layer, layerMaterial);

			// Material List 추가..
			m_TerrainData->m_MaterialList.push_back(layerMaterial);
		}
	}
		break;
	case OBJECT_TYPE::PARTICLE_SYSTEM:
		break;
	default:
		break;
	}
}

void RenderData::Release()
{
	if (m_TerrainData)
	{
		for (MaterialRenderBuffer* mat : m_TerrainData->m_MaterialList)
		{
			SAFE_DELETE(mat);
		}

		m_TerrainData->m_MaterialList.clear();
	}


	SAFE_DELETE(m_TerrainData);
	SAFE_DELETE(m_MaterialBuffer);
	SAFE_DELETE(m_MeshBuffer);
}

void RenderData::ConvertMeshBuffer(MeshBuffer* originBuf)
{
	if (originBuf == nullptr) return;

	// Mesh Buffer Index 삽입..
	m_MeshBuffer->m_BufferIndex = originBuf->BufferIndex;

	// Index Buffer Data Convert..
	m_MeshBuffer->m_IndexCount = originBuf->IndexBuf->Count;
	m_MeshBuffer->m_IndexBuf = (ID3D11Buffer*)originBuf->IndexBuf->pIndexBuf;

	// Vertex Buffer Data Convert..
	m_MeshBuffer->m_Stride = originBuf->VertexBuf->Stride;
	m_MeshBuffer->m_VertexBuf = (ID3D11Buffer*)originBuf->VertexBuf->pVertexBuf;
}

void RenderData::ConvertMaterial(MaterialBuffer* originMat)
{
	if (originMat == nullptr) return;

	ConvertMaterial(originMat, m_MaterialBuffer);
}

void RenderData::ConvertMaterial(MaterialBuffer* originMat, MaterialRenderBuffer* convertMat)
{
	// 해당 Material Data 변환..
	convertMat->m_MaterialIndex = originMat->BufferIndex;
	convertMat->m_MaterialSubData = originMat->Material_SubData;

	if (originMat->Albedo) convertMat->m_Albedo		= (ID3D11ShaderResourceView*)originMat->Albedo->pTextureBuf;
	if (originMat->Normal) convertMat->m_Normal		= (ID3D11ShaderResourceView*)originMat->Normal->pTextureBuf;
	if (originMat->Emissive) convertMat->m_Emissive	= (ID3D11ShaderResourceView*)originMat->Emissive->pTextureBuf;
	if (originMat->ORM) convertMat->m_ORM			= (ID3D11ShaderResourceView*)originMat->ORM->pTextureBuf;
}

InstanceRenderData::InstanceRenderData()
{

}

InstanceRenderData::~InstanceRenderData()
{

}

void InstanceRenderData::SetInstanceData(InstanceRenderBuffer* instance)
{
	m_Instance = instance;
}

void InstanceRenderData::PushInstance(RenderData* renderData)
{
	m_MeshList.push_back(renderData);
}
