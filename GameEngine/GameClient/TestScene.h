#pragma once
#include "Scene.h"

class GameObject;
class Transform;
class MeshFilter;
class Rigidbody;
class AnimationController;
class ParticleSystem;
class Camera;
class Image;
class RectTransform;

class TestScene : public Scene
{
public:
	TestScene() = default;
	~TestScene() = default;

public:
	void Awake() override;
	void Update() override;
	void End() override;

	void CreateMap();
	void CreateUI();
	void CreateParticle(float x, float y, float z);
	void ChangeCubeMap();

private:
	GameObject* Object;
	GameObject* Object1;
	GameObject* testobj;
	GameObject* ParticleObj;

	Camera* Cam1;
	Camera* Cam2;
	Transform* Cam2TR;

	MeshFilter* meshfilter;
	bool up;
	bool up1;

	float angle = 0.0f;

	AnimationController* AC;
	Image* IMG;
	RectTransform* RTR;
	std::vector<Image*> IMGList;
	std::vector<Transform*> TRList;
	std::vector<RectTransform*> RTRList;
	std::vector<AnimationController*> ACList;
};

