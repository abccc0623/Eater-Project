#include "MonsterB.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "GameObject.h"
#include "AnimationController.h"
#include "EaterEngineAPI.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "ClientTypeOption.h"
#include "ManaStone.h"
#include <time.h>

MonsterB::MonsterB()
{
	mMeshFilter		= nullptr;
	mTransform		= nullptr;
	mAnimation		= nullptr;
	mColider		= nullptr;
	mRigidbody		= nullptr;
}

MonsterB::~MonsterB()
{
	mMeshFilter		= nullptr;
	mTransform		= nullptr;
	mAnimation		= nullptr;
	mColider		= nullptr;
	mRigidbody		= nullptr;
}

void MonsterB::Create(ManaStone* mMana, int mCreatePointIndex)
{
	Mana = mMana;
	PointIndex = mCreatePointIndex;
}

void MonsterB::Awake()
{
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
	mTransform = gameobject->GetTransform();
	mAnimation = gameobject->GetComponent<AnimationController>();
	mColider = gameobject->GetComponent<Collider>();
	mRigidbody = gameobject->GetComponent<Rigidbody>();
}
void MonsterB::SetUp()
{
	//콜라이더 값 조정
	mColider->SetCenter(0, 0.25, 0);
	mColider->SetSphereCollider(0.25f);
	mColider->SetMaterial_Restitution(0);
	mRigidbody->SetFreezeRotation(true, true, true);
	mRigidbody->SetGravity(true);
	mColider->CreatePhys();

	//매쉬 생성
	mMeshFilter->SetModelName("MonsterB+");
	mMeshFilter->SetAnimationName("MonsterB+");
	mAnimation->Choice("idle");
	mAnimation->Play();

	//이동 위치
	Vector3 Point = Mana->GetPoint(PointIndex, 1);
	SetMovePoint(Point.x, Point.y, Point.z);

	srand((unsigned int)time(NULL));
	State = (int)MONSTER_STATE::IDLE;
}

void MonsterB::Update()
{
	switch (State)
	{
	case (int)MONSTER_STATE::IDLE:
		Idle();
		break;
	case (int)MONSTER_STATE::MOVE:
		Move();
		break;
	case (int)MONSTER_STATE::ATTACK:
		Attack();
		break;
	}

	Debug();
}

void MonsterB::Move()
{
	if (MoveStart == false)
	{
		mAnimation->Choice("move");
		MoveStart = true;
	}

	if (GetStopPoint() == false)
	{
		//목표지점의 도달하지 않았을때
		mTransform->Slow_Y_Rotation(MovePoint, 100, false);
		mRigidbody->SetVelocity(DirPoint.x, DirPoint.y, DirPoint.z);
	}
	else
	{
		//목표지점 도달 후 상태 변화
		State = (int)MONSTER_STATE::IDLE;
		PointNumber = -1;
		MoveStart = false;
	}
}

void MonsterB::Attack()
{
	if (AttackStart == false)
	{


		AttackStart = true;
	}
}

void MonsterB::Idle()
{
	//기본 값 셋팅
	if (IdleStart == false)
	{
		mAnimation->Choice("idle");
		PointNumber = rand() % 5;
		Idle_MaxTime = (rand() % Idle_MaxTime_Max) + Idle_MaxTime_Min;
		Vector3 Point = Mana->GetPoint(PointIndex, PointNumber);
		SetMovePoint(Point.x, Point.y, Point.z);
		IdleStart = true;
	}


	if (IdleTime <= Idle_MaxTime)
	{
		IdleTime += GetDeltaTime();
	}
	else
	{
		//상태 변화 기본값 초기화
		State = (int)MONSTER_STATE::MOVE;
		IdleStart = false;
		IdleTime = 0;
	}
}

void MonsterB::Debug()
{
	DebugDrawCircle(2.5f, mTransform->Position, Vector3(0, 0, 0), Vector3(1, 0, 0));
}
