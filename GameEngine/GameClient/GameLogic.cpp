#include "GameLogic.h"
#include "MainHeader.h"
#include "ObjectFactory.h"
#include "GameObject.h"
#include "Transform.h"
#include "ClientObjectManager.h"
#include "ClientComponent.h"

GameLogic::GameLogic()
{
	FactoryGM	= nullptr;
	ObjectGM	= nullptr;
}

GameLogic::~GameLogic()
{
	delete FactoryGM;
	delete ObjectGM;
}

void GameLogic::Initialize()
{
	//매니저 생성
	FactoryGM	= new ObjectFactory();
	ObjectGM	= new ClientObjectManager();

	//오브젝트 관리 매니저
	FactoryGM->Initialize(ObjectGM);
	ObjectGM->Initialize(FactoryGM);
}

void GameLogic::Release()
{
	FactoryGM->Release();
	delete FactoryGM;
}

void GameLogic::Update()
{

}

