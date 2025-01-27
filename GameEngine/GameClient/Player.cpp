#include "Player.h"
#include "AnimationController.h"
#include "Transform.h"
#include "MeshFilter.h"
#include "GameObject.h"
#include "EaterEngineAPI.h"
#include "Camera.h"
#include "Collider.h"

#include "PhysData.h"
#include "PhysRay.h"
#include "MessageManager.h"
#include "ClientTypeOption.h"
#include "ParticleController.h"
#include "ParticleFactory.h"


#include "PlayerCamera.h"

Transform* Player::mTransform = nullptr;
bool	Player::IsNoHit = false;
bool	Player::IsAttackTime = false;
int		Player::ChangeCount = 0;
int		Player::MaxChangeCount = 10;
int		Player::ComboCount = 0;

float	Player::PlayerPower		 = 20;
float	Player::PlayerComboPower = 40;

#define LERP(prev, next, time) ((prev * (1.0f - time)) + (next * time))
Player::Player()
{
	mAnimation				= nullptr;
	mTransform				= nullptr;
	mMeshFilter				= nullptr;
	mCameraTR				= nullptr;
	AttackColliderObject	= nullptr;
	AttackCollider			= nullptr;

	ANIMATION_NAME[(int)PLAYER_STATE::IDLE]			= "idle";
	ANIMATION_NAME[(int)PLAYER_STATE::ATTACK_01]	= "attack1";
	ANIMATION_NAME[(int)PLAYER_STATE::ATTACK_02]	= "attack2";
	ANIMATION_NAME[(int)PLAYER_STATE::SKILL_01]		= "skill1";
	ANIMATION_NAME[(int)PLAYER_STATE::SKILL_02]		= "skill2";
	ANIMATION_NAME[(int)PLAYER_STATE::JUMP]			= "evade";
	ANIMATION_NAME[(int)PLAYER_STATE::MOVE]			= "move";
	ANIMATION_NAME[(int)PLAYER_STATE::DEAD]			= "daed";


	ANIMATION_SPEED[(int)PLAYER_STATE::IDLE]		= 1.5f;
	ANIMATION_SPEED[(int)PLAYER_STATE::ATTACK_01]	= 1.8f;
	ANIMATION_SPEED[(int)PLAYER_STATE::ATTACK_02]	= 1.85f;
	ANIMATION_SPEED[(int)PLAYER_STATE::SKILL_01]	= 1.5f;
	ANIMATION_SPEED[(int)PLAYER_STATE::SKILL_02]	= 1.5f;
	ANIMATION_SPEED[(int)PLAYER_STATE::JUMP]		= 1.5f;
	ANIMATION_SPEED[(int)PLAYER_STATE::MOVE]		= 1.0f;
	ANIMATION_SPEED[(int)PLAYER_STATE::DEAD]		= 1.5f;


	RayCastHit = new PhysRayCast[5]();
	BasePos = Vector3(0, 0, 0);
	Start_Order = FUNCTION_ORDER_LAST;
}

Player::~Player()
{
	mAnimation = nullptr;
	mTransform = nullptr;
	mMeshFilter = nullptr;
	mCameraTR = nullptr;
	AttackColliderObject = nullptr;
	AttackCollider = nullptr;
	delete[] RayCastHit;
}

void Player::Awake()
{
	//컨퍼넌트 가져오기
	mTransform	= gameobject->GetTransform();
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
	mAnimation	= gameobject->GetComponent<AnimationController>();

	//무기 오브젝트 가져오기
	WeaponObject = Instance();
	WeaponObject->AddComponent<MeshFilter>()->SetModelName("Player_Weapon");

	//충돌 범위 가져오기
	AttackColliderObject = FindGameObjectTag("PlayerCollider");
	AttackCollider = AttackColliderObject->GetComponent<Collider>();

	IsCreate = true;
	mAttackParticle = ParticleFactory::Get()->CreateParticleController(PARTICLE_TYPE::HitSmoke);
	mHealParticle = ParticleFactory::Get()->CreateParticleController(PARTICLE_TYPE::PlayerHeal);
	mHealParticle->gameobject->transform->SetPosition_Y(0.1f);
	mHealParticle->gameobject->ChoiceParent(gameobject);
}

void Player::SetUp()
{
	//Player 컨퍼넌트값 넣어주기
	mCameraTR = FindGameObjectTag("PlayerCamera")->GetTransform();
	mMeshFilter->SetModelName("Player+");
	mMeshFilter->SetAnimationName("Player+");
	//AttackCollider 범위
	AttackCollider->SetSphereCollider(AttackRange);
	AttackCollider->SetTrigger(true);
	//애니메이션 기본 
	mAnimation->Play();
}

void Player::Start()
{
	//무기와 손을 연결
	GameObject* Hand = gameobject->GetChildBone("hand.l");
	//
	Hand->ChoiceChild(WeaponObject);
	WeaponObject->ChoiceParent(Hand);
	WeaponTR = WeaponObject->GetTransform();

	mPlayerColor.Setting(this->gameobject);
	mWeaponColor.Setting(WeaponObject);

	mPlayerColor.SetLimlightSetting(MeshFilterSetting::COLOR_TYPE::RED,0.5f, 0.5f);
	mWeaponColor.SetLimlightSetting(MeshFilterSetting::COLOR_TYPE::RED, 0.5f, 0.5f);

	mPlayerColor.SetEmissiveSetting(231, 39, 9, 2.2);
	mWeaponColor.SetEmissiveSetting(231, 39, 9, 2.2);

	// 플레이어 관련된 매쉬필터 리스트 설정..
	SetMeshFilterList();

	// UI 초기화..
	MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_EMAGIN_NOW, &ChangeCount);
	MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_EMAGIN_MAX, &MaxChangeCount);
	MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_HP_NOW, &HP);
	MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_HP_MAX, &HP_Max);
}

void Player::Update()
{
	if (IsCreate == false) { return; }
	if (GetKeyDown(VK_NUMPAD0)){mTransform->SetPosition(-16, 0, 0);}

	//플레이어 땅 체크
	PlayerGroundCheck();

	//플레이어 키입력
	PlayerKeyinput();

	//공격 충돌체의 위치를 설정
	PlayerAttackColliderUpdate();

	//플레이어 공격당했을때 무적시간
	PlayerHitTimeCheck();

	Player_Push();

	//공격상태일때 아닐떄를 먼저 체크
	if (mState == PLAYER_STATE_DEAD)
	{
		Player_Dead();
	}

	if (mState & PLAYER_STATE_JUMP)
	{
		Player_Jump();
	}
	else if (mState & (PLAYER_STATE_ATTACK_01 | PLAYER_STATE_ATTACK_02 | PLAYER_STATE_SKILL_01 | PLAYER_STATE_SKILL_02))
	{
		PlayerState_Attack();
	}
	else 
	{
		Player_Move_Check();
	}

	DirPos = { 0,0,0 };
}

void Player::SetMessageRECV(int Type, void* Data)
{
	//다른 객체에게 메세지를 받는다 받은 메세지는 
	//UI쪽으로 다시 보내줌

	switch (Type)
	{
	case MESSAGE_PLAYER_HIT:
		Player_Hit(*(reinterpret_cast<int*>(Data)));
		break;
	case MESSAGE_PLAYER_HEAL:
		Player_Heal();
		break;
	case MESSAGE_PLAYER_ATTACK_OK:
		ComboCount++;
		MessageManager::GetGM()->SEND_Message(TARGET_UI,MESSAGE_UI_COMBO,&ComboCount);
		break;
	case MESSAGE_PLAYER_ACTIVE_TRUE:
		gameobject->SetActive(true);
		WeaponObject->SetActive(true);
		AttackColliderObject->SetActive(true);
		break;
	case MESSAGE_PLAYER_ACTIVE_FALSE:
		gameobject->SetActive(false);
		WeaponObject->SetActive(false);
		AttackColliderObject->SetActive(false);
		break;
	case MESSAGE_PLAYER_COMBO_RESET:
		ComboCount = 0;
		break;
	case MESSAGE_PLAYER_PUSH:
		IsPush = true;
		PushPoint = *(reinterpret_cast<Vector3*>(Data)) - mTransform->GetPosition();
		PushNomal = PushPoint;
		PushNomal.Normalize();
		break;
	case MESSAGE_PLAYER_LIGHT_CHANGE:
		ChangeSkyLight(*(reinterpret_cast<int*>(Data)));
		break;
	case MESSAGE_PLAYER_GET_PUREMANA:
		Get_PureMana(*(reinterpret_cast<int*>(Data)));
		break;
	case MESSAGE_PLAYER_GET_COREMANA:
		Get_CoreMana(*(reinterpret_cast<int*>(Data)));
		break;
	case MESSAGE_PLAYER_UPGRADE_EMAGIN:
		Upgrade_Change_Emagin();
		break;
	case MESSAGE_PLAYER_UPGRADE_HP:
		Upgrade_Max_HP();
		break;
	case MESSAGE_PLAYER_UPGRADE_ATTACK:
		Upgrade_Attack_Speed();
		break;
	case MESSAGE_PLAYER_UPGRADE_MOVE:
		Upgrade_Move_Speed();
		break;
	}
}

void Player::SetKeyState(bool Active)
{
	IsKeyUpdate = Active;
}

Transform* Player::GetPlayerTransform()
{
	return mTransform;
}

bool Player::GetNoHitState()
{
	return IsNoHit;
}

bool Player::GetAttackState()
{
	return IsAttackTime;
}

int Player::GetPlayerColor()
{
	return ChangeCount % 2;
}

int Player::GetPlayerCombo()
{
	return ComboCount;
}

float Player::GetPlayerPower()
{
	return PlayerPower;
}

float Player::GetPlayerComboPower()
{
	return PlayerComboPower;
}

void Player::SetNoHit(bool Active)
{
	IsNoHit = Active;
}

void Player::PlayerKeyinput()
{
	if (IsKeyUpdate == false) return;


	if (mCameraTR == nullptr) 
	{
		DebugPrint("카메라 연결 안되어있음");
		return;
	}

	if (GetKeyDown(VK_SPACE))
	{
		if ((mState & PLAYER_STATE_JUMP) == 0)
		{
			Sound_Play_SFX("Player_Evade");
			mAnimation->SetFrame(0);
		}

		mState = PLAYER_STATE_JUMP;
	}

	if (GetKey('D'))
	{
		if (IsRightRayCheck == true)
		{
			DirPos += mCameraTR->GetLocalPosition_Right();
		}
		DirRot += mCameraTR->GetLocalPosition_Right();
	}

	if (GetKey('A'))
	{
		if (IsLeftRayCheck == true)
		{
			DirPos += -mCameraTR->GetLocalPosition_Right();
		}
		DirRot += -mCameraTR->GetLocalPosition_Right();
	}

	if (GetKey('W'))
	{
		if (IsFrontRayCheck == true)
		{
			Vector3 Pos = mCameraTR->GetLocalPosition_Look();
			Pos.y = 0;
			DirPos += Pos;
		}
		DirRot += mCameraTR->GetLocalPosition_Look();
	}

	if (GetKey('S'))
	{
		if (IsBackRayCheck == true)
		{
			Vector3 Pos = mCameraTR->GetLocalPosition_Look();
			Pos.y = 0;
			DirPos += -Pos;
		}
		DirRot += mCameraTR->GetLocalPosition_Look() * -1;
	}

	if (GetKeyDown(VK_LBUTTON))
	{
		if (AttackKeyDownCount < 1)
		{
			mState |= PLAYER_STATE_ATTACK_01;
		}
		else
		{
			mState |= PLAYER_STATE_ATTACK_02;
		}
		AttackKeyDownCount++;
	}
	else if (GetKeyDown(VK_RBUTTON))
	{
		mState |= PLAYER_STATE_SKILL_01;
	}
	else if (GetKeyDown('E'))
	{
		mState |= PLAYER_STATE_SKILL_02;
	}
	else if (GetKeyDown('Q'))
	{
		ChangeCount++;
		if (ChangeCount > MaxChangeCount) { ChangeCount = 0; }

		MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_EMAGIN_NOW, &ChangeCount);

		if ((ChangeCount % 2) == 0)
		{
			mPlayerColor.SetEmissiveSetting(231, 39, 9, 2.2);
			mWeaponColor.SetEmissiveSetting(231, 39, 9, 2.2);
		}
		else
		{
			mPlayerColor.SetEmissiveSetting(22, 71, 243, 2.2);
			mWeaponColor.SetEmissiveSetting(22, 71, 243, 2.2);
		}
		Sound_Play_SFX("ChangeEmagin");
	}

	

	//이번프레임에 이동해야하는 방향
	DirPos.Normalize();
	DirRot.Normalize();

	Vector3 position = mTransform->GetPosition();

	if (IsAttack == false)
	{
		mTransform->Slow_Y_Rotation(DirRot + position, 450);

		//공격중이 아니고 회전값이 있을경우
		if (DirPos != Vector3(0, 0, 0))
		{
			if (MoveSoundTime >= 0.75f)
			{
				Sound_Play_SFX("Player_Walking");
				MoveSoundTime = 0;
			}
			else
			{
				MoveSoundTime += GetDeltaTime();
			}

			PastDirPos = DirPos;
			mTransform->AddPosition(DirPos * Speed * GetDeltaTime());
		}
		Attack_Rot = false;
	}
	else
	{
		if (Attack_Rot == false)
		{
			AttackStartRot = mCameraTR->GetLocalPosition_Look();
			Attack_Rot = true;
		}
		mTransform->Slow_Y_Rotation(AttackStartRot + position, 450);
	}
}

void Player::PlayerState_Attack()
{
	IsAttack = true;
	/// 플레이어 공격상태일때 들어옵니다
	int Type = 0;
	if (mState & PLAYER_STATE_SKILL_02)
	{
		Type = (int)PLAYER_STATE::SKILL_02;
		mAnimation->Choice(ANIMATION_NAME[Type], ANIMATION_SPEED[Type]);
		Player_Skill_02();
	}
	else if (mState & PLAYER_STATE_SKILL_01)
	{
		Type = (int)PLAYER_STATE::SKILL_01;
		mAnimation->Choice(ANIMATION_NAME[Type], ANIMATION_SPEED[Type]);
		Player_Skill_01();
	}
	else if (mState & PLAYER_STATE_ATTACK_01)
	{
		Type = (int)PLAYER_STATE::ATTACK_01;
		mAnimation->Choice(ANIMATION_NAME[Type], ANIMATION_SPEED[Type]);
		Player_Attack_01();
	}
	else if (mState & PLAYER_STATE_ATTACK_02)
	{
		Type = (int)PLAYER_STATE::ATTACK_02;
		mAnimation->Choice(ANIMATION_NAME[Type], ANIMATION_SPEED[Type]);
		Player_Attack_02();
	}

	mAnimation->Play();
}

void Player::PlayerState_Base()
{
	IsAttack	= false;
	/// <summary>
	/// 플레이어 공격상태가 아닐때 들어옵니다
	/// </summary>

	if (mState & PLAYER_STATE_JUMP)
	{
		Player_Jump();
	}
	else
	{
		Player_Move_Check();
	}
}

bool Player::PlayerEndFrameCheck()
{
	AttackEndFrame	= mAnimation->GetEndFrame();
	NowFrame		= mAnimation->GetNowFrame();
	if (NowFrame >= AttackEndFrame)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Player::PlayerHitTimeCheck()
{
	if (IsHit == false) { return; }
	HitTime += GetDeltaTime();
	if (HitTime >= 0.5f)
	{
		IsHit = false;
		HitTime = 0;
	}
}

void Player::Player_Attack_01()
{
	WeaponTR->SetPosition(0.0f, -0.02f, 0.1f);
	WeaponTR->SetRotate(8.0f, 19.0f, -10.0f);

	if (mAnimation->EventCheck() == true)
	{
		IsAttackTime = true;
	}
	else
	{
		IsAttackTime = false;
	}

	if (mAnimation->GetNowFrame() == 32)
	{
		mAttackParticle->SetPosition(AttackColliderObject->GetTransform()->GetPosition());
		mAttackParticle->Play();
	}
	
	//망치를 살짝올리고 내려찍는 공격
	if (PlayerEndFrameCheck() == true)
	{
		if (AttackKeyDownCount > 1)
		{
			mState = PLAYER_STATE_ATTACK_02;
		}
		else
		{
			Player_Move_Check();
		}
	}
}

void Player::Player_Attack_02()
{
	//망치를 뒤로 돌리고 크게 내려찍는 공격
	WeaponTR->SetPosition(0.0f, -0.02f, 0.1f);
	WeaponTR->SetRotate(186.0f, 31.0f, 0.0f);

	if (mAnimation->GetNowFrame() == 35)
	{
		mAttackParticle->SetPosition(AttackColliderObject->GetTransform()->GetPosition());
		mAttackParticle->Play();
	}


	if (mAnimation->EventCheck() == true)
	{
		IsAttackTime = true;
	}
	else
	{
		IsAttackTime = false;
	}

	if (PlayerEndFrameCheck() == true)
	{
		Player_Move_Check();
		AttackKeyDownCount = 0;
	}
}

void Player::Player_Skill_01()
{
	WeaponTR->SetPosition(0.0f, -0.02f, 0.1f);
	WeaponTR->SetRotate(186.0f, 31.0f, 0.0f);

	if (mAnimation->EventCheck() == true)
	{
		IsAttackTime = true;
	}
	else
	{
		IsAttackTime = false;
	}

	if (mAnimation->GetNowFrame() == 49)
	{
		mAttackParticle->SetPosition(AttackColliderObject->GetTransform()->GetPosition());
		mAttackParticle->Play();
	}

	if (PlayerEndFrameCheck())
	{
		Player_Move_Check();
	}
}

void Player::Player_Skill_02()
{
	WeaponTR->SetPosition(0.0f, -0.02f, 0.1f);
	WeaponTR->SetRotate(186.0f, 31.0f, 0.0f);

	if (mAnimation->EventCheck() == true)
	{
		IsAttackTime = true;
	}
	else
	{
		IsAttackTime = false;
	}

	if (mAnimation->GetNowFrame() == 72)
	{
		mAttackParticle->SetPosition(AttackColliderObject->GetTransform()->GetPosition());
		mAttackParticle->Play();
	}

	if (PlayerEndFrameCheck())
	{
		Player_Move_Check();
	}
}

void Player::Player_Skill_03()
{
	WeaponTR->SetPosition(0.0f, -0.02f, 0.1f);
	WeaponTR->SetRotate(8.0f, 19.0f, -10.0f);

	if (mAnimation->EventCheck() == true)
	{
		IsAttackTime = true;
	}
	else
	{
		IsAttackTime = false;
	}


	if (PlayerEndFrameCheck())
	{
		Player_Move_Check();
	}
}

void Player::Player_Jump()
{
	IsAttack = false;
	IsAttackTime = false;

	WeaponTR->SetPosition(0.0f, -0.02f, 0.1f);
	WeaponTR->SetRotate(-22.0f, 33.0f, 2.0f);
	mAnimation->Choice(ANIMATION_NAME[(int)PLAYER_STATE::JUMP] ,ANIMATION_SPEED[(int)PLAYER_STATE::JUMP]);

	int Now = mAnimation->GetNowFrame();
	int End = mAnimation->GetEndFrame();

	if (mAnimation->EventCheck() == true && IsFrontRayCheck == true)
	{
		if (IsFrontRayCheck == true && IsBackRayCheck == true &&
			IsRightRayCheck == true && IsLeftRayCheck == true)
		{
			Vector3 MovePos = PastDirPos * GetDeltaTime() * SpeedMax;
			mTransform->AddPosition(MovePos.x, 0, MovePos.z);
		}
	}

	if(Now>= End)
	{
		mState = PLAYER_STATE_IDLE;
		IsNoHit = false;
	}
	else
	{
		IsNoHit = true;
	}
}

void Player::Player_Hit(int HitPower)
{
	if (IsHit == true) { return; }
	if (IsNoHit == true) { return; }

	Sound_Play_SFX("Player_Hit");
	IsHit = true;
	HP -= HitPower;
	if (HP <= 0)
	{
		//죽었을때
		HP = 0;
		MessageManager::GetGM()->SEND_Message(TARGET_DRONE, MESSAGE_DRONE_PLAYER_DIE);
		mState = PLAYER_STATE_DEAD;
	}
	MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_HP_NOW, &HP);
	MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_PLAYER_HIT);
}

bool Player::Player_Move_Check()
{
	IsAttack = false;
	IsAttackTime = false;

	int Type = 0;
	if (DirPos == BasePos)
	{
		WeaponTR->SetPosition(0.0f, -0.02f, 0.1f);
		WeaponTR->SetRotate(186.0f, 31.0f, 0.0f);

		mState = PLAYER_STATE_IDLE;
		Type = (int)PLAYER_STATE::IDLE;
		mAnimation->Choice(ANIMATION_NAME[Type], ANIMATION_SPEED[Type]);
		AttackKeyDownCount = 0;
		return false;
	}
	else
	{
		WeaponTR->SetPosition(0.0f, -0.02f, 0.1f);
		WeaponTR->SetRotate(0.0f, 21.0f, -5.0f);

		mState = PLAYER_STATE_MOVE; 
		Type = (int)PLAYER_STATE::MOVE;
		mAnimation->Choice(ANIMATION_NAME[Type], ANIMATION_SPEED[Type]);
		AttackKeyDownCount = 0;
		return true;
	}
}

void Player::Player_Push()
{
	//밀기가 시작되고
	static float PushTime = 0;
	if (IsPush == false){return;}

	//캐릭터의 속력을 0으로 애니메이션은 움직이고 이동은 못하도록
	Speed = 0;

	//만약 레이캐스트중 한개라도 충돌이 되지않았다면
	if (IsFrontRayCheck == false || IsBackRayCheck == false ||
		IsLeftRayCheck == false || IsRightRayCheck == false)
	{
		//미는것을 멈춘다
		IsPush		= false;
		Speed		= SpeedMax;
		PushTime	= 0;
	}
	else
	{
		if (PushTime >= 1.5f)
		{
			IsPush = false;
			Speed = SpeedMax;
			PushTime = 0;
		}
		else
		{
			PushTime += GetDeltaTime();
			mTransform->AddPosition(PushNomal * SpeedMax * GetDeltaTime());
		}
	}
}

void Player::Player_Dead()
{
	mAnimation->Choice("daed");
	int Now = mAnimation->GetNowFrame();
	int End = mAnimation->GetEndFrame();
	if (Now > End)
	{
		mState = PLAYER_STATE_IDLE;
		mTransform->SetPosition(-16, 0, 0);
		//체력 초기화,콤보 초기화
		HP = HP_Max;
		MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_HP_NOW, &HP);
		MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_COMBO, &ComboCount);
	}

}

void Player::Player_Heal()
{
	HP += 100;
	if (HP >= HP_Max) { HP = HP_Max; }

	mHealParticle->Play();
	MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_HP_NOW, &HP);
	Sound_Play_SFX("Player_Heal");
	MessageManager::GetGM()->SEND_Message(TARGET_DRONE, MESSAGE_DRONE_PLAYER_HEAL);
}

void Player::ChangeSkyLight(int index)
{
	for (int i = 0; i < ChildMeshFilter.size(); i++)
	{
		ChildMeshFilter[i]->SetSkyLightIndex(index);
	}
}

void Player::SetMeshFilterList()
{
	MeshFilter* filter = WeaponObject->GetComponent<MeshFilter>();
	ChildMeshFilter.push_back(filter);

	int count = gameobject->GetChildMeshCount();
	for (int i = 0; i < count; i++)
	{
		GameObject* object = gameobject->GetChildMesh(i);

		filter = object->GetComponent<MeshFilter>();

		if (filter == nullptr) continue;

		ChildMeshFilter.push_back(filter);
	}
}

void Player::Get_PureMana(int count)
{
	if (PureMana >= Max_Mana_Value) return;

	// 퓨어 마나 최초 획득
	if (IsGetPureMana == false)
	{
		IsGetPureMana = true;

		MessageManager::GetGM()->SEND_Message(TARGET_DRONE, MESSAGE_DRONE_GET_PUREMANA);
	}

	PureMana += count;

	if (PureMana >= Max_Mana_Value)
	{
		PureMana = Max_Mana_Value;
	}

	MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_PUREMANA, &PureMana);
}

void Player::Get_CoreMana(int count)
{
	if (CoreMana >= Max_Mana_Value) return;

	// 코어 마나 최초 획득
	if (IsGetCoreMana == false)
	{
		IsGetCoreMana = true;

		MessageManager::GetGM()->SEND_Message(TARGET_DRONE, MESSAGE_DRONE_GET_COREMANA);
	}

	CoreMana += count;

	if (CoreMana >= Max_Mana_Value)
	{
		CoreMana = Max_Mana_Value;
	}

	MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_COREMANA, &CoreMana);
}

void Player::Upgrade_Change_Emagin()
{
	if (MaxChangeCount >= Max_ChangeEmagin_Value)
	{
		Sound_Play_SFX("UI_Button_Click");
	}

	if (PureMana < ChangeEmagin_PureMana_Count)
	{
		Sound_Play_SFX("UI_Button_Click");
		MessageManager::GetGM()->SEND_Message(TARGET_DRONE, MESSAGE_DRONE_PURCHASE_FAIL);
		return;
	}

	Sound_Play_SFX("VendingMachine");

	MaxChangeCount += Upgrade_ChangeEmagin;
	PureMana -= ChangeEmagin_PureMana_Count;

	MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_EMAGIN_MAX, &MaxChangeCount);
	MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_PUREMANA, &PureMana);
	MessageManager::GetGM()->SEND_Message(TARGET_DRONE, MESSAGE_DRONE_PURCHASE_SUCCESS);
}

void Player::Upgrade_Max_HP()
{
	if (HP_Max >= Max_HP_Value)
	{
		Sound_Play_SFX("UI_Button_Click");
	}

	if (CoreMana < HP_CoreMana_Count)
	{
		Sound_Play_SFX("UI_Button_Click");
		MessageManager::GetGM()->SEND_Message(TARGET_DRONE, MESSAGE_DRONE_PURCHASE_FAIL);
		return;
	}
	
	Sound_Play_SFX("VendingMachine");

	HP += Upgrade_HP;
	HP_Max += Upgrade_HP;
	CoreMana -= HP_CoreMana_Count;

	MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_HP_NOW, &HP);
	MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_HP_MAX, &HP_Max);
	MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_COREMANA, &CoreMana);
	MessageManager::GetGM()->SEND_Message(TARGET_DRONE, MESSAGE_DRONE_PURCHASE_SUCCESS);
}

void Player::Upgrade_Attack_Speed()
{
	if (Now_Attack_Speed >= Max_AttackSpeed_Animation)
	{
		Sound_Play_SFX("UI_Button_Click");
	}

	if (PureMana < AttackSpeed_PureMana_Count)
	{
		Sound_Play_SFX("UI_Button_Click");
		MessageManager::GetGM()->SEND_Message(TARGET_DRONE, MESSAGE_DRONE_PURCHASE_FAIL);
		return;
	}

	Sound_Play_SFX("VendingMachine");
	
	Now_Attack_Speed += Upgrade_AttackSpeed_1;
	PureMana -= AttackSpeed_PureMana_Count;

	ANIMATION_SPEED[(int)PLAYER_STATE::ATTACK_01] += Upgrade_AttackSpeed_1;
	ANIMATION_SPEED[(int)PLAYER_STATE::ATTACK_02] += Upgrade_AttackSpeed_2;
	ANIMATION_SPEED[(int)PLAYER_STATE::SKILL_01] += Upgrade_AttackSpeed_1;
	ANIMATION_SPEED[(int)PLAYER_STATE::SKILL_02] += Upgrade_AttackSpeed_1;

	MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_PUREMANA, &PureMana);
	MessageManager::GetGM()->SEND_Message(TARGET_DRONE, MESSAGE_DRONE_PURCHASE_SUCCESS);
}

void Player::Upgrade_Move_Speed()
{
	if (Speed >= Max_Move_Speed_Value)
	{
		Sound_Play_SFX("UI_Button_Click");
	}

	if (PureMana < MoveSpeed_PureMana_Count)
	{
		Sound_Play_SFX("UI_Button_Click");
		MessageManager::GetGM()->SEND_Message(TARGET_DRONE, MESSAGE_DRONE_PURCHASE_FAIL);
		return;
	}

	Sound_Play_SFX("VendingMachine");

	Speed += Upgrade_MoveSpeed_Value;
	SpeedMax += Upgrade_MoveSpeed_Value;
	PureMana -= MoveSpeed_PureMana_Count;

	ANIMATION_SPEED[(int)PLAYER_STATE::MOVE] += Upgrade_MoveSpeed_Animation;

	MessageManager::GetGM()->SEND_Message(TARGET_UI, MESSAGE_UI_PUREMANA, &PureMana);
	MessageManager::GetGM()->SEND_Message(TARGET_DRONE, MESSAGE_DRONE_PURCHASE_SUCCESS);
}

void Player::PlayerAttackColliderUpdate()
{
	Vector3 Look = mTransform->GetLocalPosition_Look();
	Look *= 2;
	Look.y = 1;
	Look.z *= -1 * AttackDir;
	AttackColliderObject->GetTransform()->SetPosition(mTransform->GetPosition() + Look);
}

void Player::PlayerGroundCheck()
{
	Vector3 RayStartPoint;
	Vector3 position = mTransform->GetPosition();
	for (int i = 0; i < 5; i++)
	{
		switch (i)
		{
		case 0: //Front
			RayStartPoint = position + mCameraTR->GetLocalPosition_Look() * 0.65f;
			RayStartPoint.y = position.y + 1.25f;
			break;
		case 1: //Back
			RayStartPoint = position + (mCameraTR->GetLocalPosition_Look() * -0.65f);
			RayStartPoint.y = position.y + 1.25f;
			break;
		case 2: //Right
			RayStartPoint = position + mCameraTR->GetLocalPosition_Right() * 0.65f;
			RayStartPoint.y = position.y + 1.25f;
			break; 
		case 3: //Left
			RayStartPoint = position + (mCameraTR->GetLocalPosition_Right() * -0.65f);
			RayStartPoint.y = position.y + 1.25f;
			break;
		case 4: //Center
			RayStartPoint = position;
			RayStartPoint.y = position.y + 0.5f;
			break;
		}
		
		//Ray 값 조정
		RayCastHit[i].Origin		= RayStartPoint;
		RayCastHit[i].Direction		= {0,-1,0};
		RayCastHit[i].MaxDistance	= 5;
		bool isHit = RayCast(&RayCastHit[i]);

		if (isHit == true)
		{
			float Offset_Min = mTransform->GetPosition().y - 1.0f;
			float Offset_Max = mTransform->GetPosition().y + 1.0f;

			float Point_Y = RayCastHit[i].Hit.HitPoint.y;
			if (Point_Y <= Offset_Min || Point_Y >= Offset_Max)
			{
				isHit = false;
			}
		}

		switch (i)
		{
		case 0:
			IsFrontRayCheck = isHit;
			break;
		case 1:
			IsBackRayCheck = isHit;
			break;
		case 2:
			IsRightRayCheck = isHit;
			break;
		case 3:
			IsLeftRayCheck = isHit;
			break;
		case 4:
			mTransform->SetPosition_Y(RayCastHit[i].Hit.HitPoint.y);
			break;
		}
	}
}
