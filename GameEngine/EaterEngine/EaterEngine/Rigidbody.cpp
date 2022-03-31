#include "Rigidbody.h"
#include "Transform.h"
#include "GameObject.h"
#include "PhysData.h"
#include "PhysX_API.h"
#include "PhysData.h"
#include "EngineData.h"
#include "Collider.h"

Rigidbody::Rigidbody()
{
	//컨퍼넌트 실행 순서를 변경
	Component::Awake_Order				= Component::FUNCTION_ORDER_LAST;
	Component::TransformUpdate_Order	= Component::FUNCTION_ORDER_FIRST;
	Component::SetUp_Order				= Component::FUNCTION_ORDER_LAST;
}

Rigidbody::~Rigidbody()
{
	
}

void Rigidbody::Awake()
{
	Collider* mCollider = gameobject->GetComponent<Collider>();
	mTransform			= gameobject->GetTransform();
	if (mCollider != nullptr)
	{
		RigidbodyData = mCollider->mPhysData;

		RigidbodyData->isGravity	= isGravity;
		RigidbodyData->isKinematic	= isKinematic;
		RigidbodyData->MT_Mass = Mass;
		RigidbodyData->SetLockAxis_Position(FreezePos[0], FreezePos[1], FreezePos[2]);
		RigidbodyData->SetLockAxis_Rotation(FreezeRot[0], FreezeRot[1], FreezeRot[2]);
	}
}

void Rigidbody::Start()
{
	//if (ColliderName != "")
	//{
	//	//이것은 트라이엥글 매쉬를 만들때 필요하다
	//	ModelData* data = LoadManager::GetMesh(ColliderName);
	//
	//	TriangleMeshData* Triangle = new TriangleMeshData();
	//	int IndexSize	= data->TopMeshList[0]->m_OriginIndexListCount;
	//	int VertexSize	= data->TopMeshList[0]->m_OriginVertexListCount;
	//
	//	Triangle->VertexList		= data->TopMeshList[0]->m_OriginVertexList;
	//	Triangle->VertexListSize	= VertexSize;
	//
	//	Triangle->IndexList		= data->TopMeshList[0]->m_OriginIndexList;
	//	Triangle->IndexListSize = IndexSize;
	//
	//	RigidbodyData->CreateTriangleCollider(Triangle,VertexSize,IndexSize);
	//} 
	////RigidbodyData->Rotation = {0,0,0,1};
	////RigidbodyData->SetWorldPosition(Tr->Position.x, Tr->Position.y, Tr->Position.z);
	//PhysX_Create_Actor(RigidbodyData);
	////Tr->Load_Local._41 += RigidbodyData->CenterPoint.x;
	////Tr->Load_Local._42 += RigidbodyData->CenterPoint.y;
	////Tr->Load_Local._43 += RigidbodyData->CenterPoint.z;
	//
	//// Collider Data 설정..
	//gameobject->OneMeshData->Collider_Data = Collider_Data;
}

void Rigidbody::SetUp()
{
	/// 시작 값을 넣어준다
	//Tr = gameobject->GetTransform();
}

void Rigidbody::PhysicsUpdate()
{
	

}

 bool Rigidbody::GetTriggerEnter()
{
	 return RigidbodyData->GetTriggerEnter();
}

 bool Rigidbody::GetTriggerStay()
{
	return RigidbodyData->GetTriggerStay();
}

 bool Rigidbody::GetTriggerExit()
{
	return RigidbodyData->GetTriggerExit();
}

int Rigidbody::GetTriggerCount()
{
	return RigidbodyData->TriggerCount;
}

void Rigidbody::SetGravity(bool grvity)
{
	if (RigidbodyData == nullptr)
	{
		isGravity = grvity;
	}
	else 
	{
		RigidbodyData->isGravity = grvity;
	}
}

bool Rigidbody::GetGravity()
{
	return RigidbodyData->isGravity;
}


void Rigidbody::SetKinematic(bool kinematic)
{
	if (RigidbodyData == nullptr)
	{
		isKinematic = kinematic;
	}
	else
	{
		RigidbodyData->isKinematic = kinematic;
	}
}

bool Rigidbody::GetKinematic()
{
	return RigidbodyData->isKinematic;
}

void Rigidbody::SetMass(float mass)
{
	if (RigidbodyData == nullptr)
	{
		Mass = mass;
	}
	else
	{
		RigidbodyData->MT_Mass = mass;
	}
}

float Rigidbody::GetMass()
{
	return RigidbodyData->MT_Mass;
}

void Rigidbody::SetFreezePosition(bool x, bool y, bool z)
{
	if (RigidbodyData == nullptr)
	{
		FreezePos[0] = x;
		FreezePos[1] = y;
		FreezePos[2] = z;
	}
	else
	{
		RigidbodyData->SetLockAxis_Position(x, y, z);
	}
}

Vector3 Rigidbody::GetFreezePosition()
{
	return RigidbodyData->GetLockAxis_Position();
}

void Rigidbody::SetFreezeRotation(bool x, bool y, bool z)
{
	if (RigidbodyData == nullptr)
	{
		FreezeRot[0] = x;
		FreezeRot[1] = y;
		FreezeRot[2] = z;
	}
	else
	{
		RigidbodyData->SetLockAxis_Rotation(x, y, z);
	}
}

Vector3 Rigidbody::GetFreezeRotation()
{
	return RigidbodyData->GetLockAxis_Rotation();
}

void Rigidbody::SetVelocity(float x, float y, float z)
{
	RigidbodyData->SetVelocity(x, y, z);
}

Vector3 Rigidbody::GetVelocity()
{
	return RigidbodyData->GetVelocity();
}

 void Rigidbody::SetAddForce(float x, float y, float z)
 {
	 RigidbodyData->AddForce(x, y, z);
 }

 void Rigidbody::SetCenterPoint(float x, float y, float z)
 {
	 RigidbodyData->CenterPoint = {x,y,z}; 
 }





