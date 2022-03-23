#pragma once
#include <windows.h>
#include <string>
#include "EaterEngineDLL.h"
#include "TypeOptionHeader.h"

/// <summary>
/// 전체 게임 엔진을 총괄하는 게임엔진 클래스
/// 커다란 게임엔진 틀
/// </summary>

namespace flatbuffers
{
	class FlatBufferBuilder;
}


//각종 매니저들
class DebugManager;
class LoadManager;
class ObjectManager;
class SceneManager;
class KeyinputManager;
class GraphicEngineManager;
class TimeManager;
class MaterialManager;
class LightManager;
class PhysManager;
class NetworkManager;
class Scene;
class NetworkManagerComponent;
class ThreadManager;
//게임 오브젝트
class GameObject;
class ModelData;
class Material;

class RenderOption;

class GameEngine
{
private:
	CRITICAL_SECTION g_CS;


public:
	GameEngine();
	~GameEngine();
	
public:
	///클라이언트 에서 만질 함수들
	///게임 엔진 관련 함수들
	void Initialize(HWND gHwnd,bool mConsoleDebug = true);	//초기화
	void Start();											//추가 작업
	void Update();											//엔진 가동
	void Finish();											//엔진 중지
	void OnResize(int Change_Width, int Change_Height);		//리사이즈
	static int WinSizeWidth;	//윈도우 사이즈 가로
	static int WinSizeHeight;	//윈도우 사이즈 세로
public:
	///오브젝트 관련(오브젝트 매니저)
	GameObject* Instance(std::string ObjName = "GameObject");			//obj 생성 (빈 게임오브젝트)
	GameObject* InstanceTerrain(std::string ObjName = "Terrain");
	GameObject* InstanceParticle(std::string ObjName = "Particle");
	GameObject* InstanceCamera(std::string ObjName = "Camera");
	GameObject* InstanceLight(std::string ObjName = "Light", LIGHT_TYPE type = LIGHT_TYPE::POINT_LIGHT);

	Material* InstanceMaterial(std::string matName = "Material");

	GameObject* FindGameObjectTag(std::string& TagName);
	GameObject* FindGameObjectName(std::string& ObjName);

	void Destroy(GameObject* obj);		//obj 삭제
	void DestroyAll();
public:
	///스크린 관련
	void PushScene(Scene* mScene, std::string name);	//씬 생성		
	void ChoiceScene(std::string name);					//씬 선택
public:
	///로드 관련 
	void LoadTerrain(std::string mMeshName, std::string mMaskName, UINT parsingMode);
	void Load(std::string& Path, UINT MODE);
	int  LoadMeshCount();
	int  LoadTextureCount();
	int  LoadAnimationCount();
	ModelData* GetLoadMeshData(std::string& Path);
public:
	///환경맵 관련
	void LoadEnvironment(std::string mPath);
	void SetEnvironment(bool enable);
public:
	///키관련 
	bool  GetKeyDown(byte number);	//키다운
	bool  GetKeyUp(byte number);	//키업
	bool  GetKey(byte number);		//키누르고있을때
	bool  GetTogle(byte number);	//키 on/off
	float GetMousePosX();			//마우스 위치 X
	float GetMousePosY();			//마우스 위치 Y
	void  SetMousePos(float x,float y);
	void  SetMousePosCenter();
	void  ShowMouseCursor(bool Show);
	void  MouseCursorClip(bool Clip);
public:
	///카메라 관련
	GameObject* GetMainCamera();

public:
	///시간 관련
	float GetdeltaTime();

public:
	///네트워크 관련
	void SetNetworkManager(NetworkManagerComponent* Manager);
	void NETWORK_SEND(flatbuffers::FlatBufferBuilder* Builder, int Type);
	void NETWORK_LOADING_COMPLETE(unsigned int Number);
	void NETWORK_CONNECT(int ServerPort, std::string  Local_Connect_IP);

public:
	///디버그 관련
	void EditorSetting();
	void DebugDrawLine(Vector3 start, Vector3 end, Vector3 color);
	void DebugDrawLine(Vector3 start, Vector3 dir, float distance, Vector3 color);

private:
	GameObject* CreateInstance();
	Material* CreateMaterial();
	void CreateObject();			//기본 생성 오브젝트
	void RenderOptionCheck();

private:
	///각종 매니저들
	ObjectManager*			mObjectManager;		// 오브젝트 관리 매니저
	DebugManager*			mDebugManager;		// 디버깅매니저
	LoadManager*			mLoadManager;		// 로드 관련 매니저
	SceneManager*			mSceneManager;		// 스크린 관리 매니저
	KeyinputManager*		mKeyManager;		// 키인풋 관련 매니저
	GraphicEngineManager*	mGraphicManager;	// 그래픽엔진 관리 매니저
	TimeManager*			mTimeManager;		// 시간 관련 타임매니저
	LightManager*			mLightManager;		// 라이트 관리 매니저
	PhysManager*			mPhysManager;		// 물리 관리 매니저
	NetworkManager*			mNetworkManager;	// 네트워크 매니저
	ThreadManager*			mThreadManager;		// 쓰레드 매니저
private:
	///클라쪽에서 받아야 하는 데이터
	HWND mHwnd; //핸들
	bool ConsoleDebug;
private:
	RenderOption* mRenderOption;
};

