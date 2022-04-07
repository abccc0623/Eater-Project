#pragma once
#include "ClientComponent.h"
class MeshFilter;
class Transform;
class GameObject;
class AnimationController;
class Collider;
class Rigidbody;

class MonsterA :public ClientComponent
{
public:
	MonsterA();
	virtual ~MonsterA();
public:
	void Awake();
	void SetUp();
	void Update();
	void OnTriggerStay(GameObject* Obj);
	void OnTriggerEnter(GameObject* Obj);
	void ReSet();
	void SetMovePoint(float x, float y, float z);
private:
	MeshFilter*				mMeshFilter;
	Transform*				mTransform;
	AnimationController*	mAnimation;
	Collider*				mColider;
	Rigidbody*				mRigidbody;
private:
	Vector3 MovePoint;	//이동해야하는 지점
	Vector3 DirPoint;	//이동지점의 방향벡터

	int BulletTag;

	float HP			= 100;	//현재 체력
	float Speed			= 2;	//이동 속도
	float AttackTime	= 0;	//현재 공격 시간

	const float PushPower		= 100;
	const float AttackMaxTime	= 10;	//공격주기
	const float MaxHP			= 100;	//최대체력
};

