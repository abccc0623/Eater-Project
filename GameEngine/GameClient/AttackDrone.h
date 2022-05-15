#pragma once
#include "ClientComponent.h"

class GameObject;
class Transform;
class MeshFilter;
class Collider;
class AttackDrone : public ClientComponent
{
public:
	AttackDrone();
	~AttackDrone();

	void Awake();
	void SetUp();
	void Update();
	void ReSet();
	void OnTriggerEnter(GameObject* Obj);
	void OnTriggerStay(GameObject* Obj);
	void OnTriggerExit(GameObject* Obj);
private:
	Transform*	mTransform;
	Transform*	mTargetMonsterTR;
	MeshFilter* mMeshFilter;
	Collider*	mCollider;
private:
	int MonsterTag;
	float AttackTime = 0;
	const float AttackMaxTime	= 0.5f;
	const float AttackRange		= 10.0f;
};

