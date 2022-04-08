#pragma once
// Parsing Option
typedef enum PARSING_OPTION : UINT
{
	SCALING			= 0x00000001,	// FBX Parser Parsing Scale 100 -> 1
	ANIMATION_ONLY	= 0x00000010,	// FBX Parser Parsing Only Animaition
	ORIGIN_ONLY		= 0x00000100	// FBX Parser Parsing Only Origin Vertex (Collision 전용)
}PARSING_OPTION;

// Lighting Option
typedef enum RENDER_OPTION : UINT
{
	RENDER_DEBUG			= 0x00000001,
	RENDER_SHADOW			= 0x00000010,
	RENDER_SSAO				= 0x00000100,
	RENDER_IBL				= 0x00001000,
}RENDER_OPTION;

// PostProcess Option
typedef enum POSTPROCESS_OPTION : UINT
{
	RENDER_FOG		= 0x00000001,
	RENDER_BLOOM	= 0x00000010,
	RENDER_HDR		= 0x00000100,
	RENDER_FXAA		= 0x00001000,
}POSTPROCESS_OPTION;

// Debug Option
typedef enum DEBUG_OPTION : UINT
{
	DEBUG_ENGINE,
	DEBUG_EDITOR,
}DEBUG_OPTION;

// Particle Animation Option
typedef enum PARTICLE_ANIMATION_OPTION : UINT
{
	TEXTURE_ANI		= 0x00000001,
	COLOR_ANI		= 0x00000010,
	POSITION_ANI	= 0x00000100,
	ROTATION_ANI	= 0x00001000,
	SCALE_ANI		= 0x00010000
}PARTICLE_ANIMATION_OPTION;

// Particle Render Option
typedef enum PARTICLE_RENDER_OPTION
{
	BILLBOARD,
	VERTICAL_BILLBOARD,
	HORIZONTAL_BILLBOARD,
	MESH
}PARTICLE_RENDER_OPTION;

// Particle LifeTime Option
typedef enum PARTICLE_LIFETIME_OPTION
{
	NONE,
	UP,
	DOWN,
	UPDOWN
}PARTICLE_LIFETIME_OPTION;

// Object Type
typedef enum OBJECT_TYPE
{
	DEFALT,			//값을 넣지않았을때 기본형
	GAMEOBJECT,		//다른 오브젝트들을 묶어놓는 용도
	BASE,			//상속구조로 되어있는 오브젝트
	SKINNING,		//스키닝이 추가된 오브젝트
	BONE,			//본만 있는 오브젝트
	TERRAIN,		//터레인 오브젝트
	CAMERA,			//카메라 기능만 하는 오브젝트
	LIGHT,			//라이트 객체
	SKYBOX,			//스카이 박스
	TEXTURE,		//한개의 텍스쳐로된 오브젝트(페이스두개가 합쳐진 사각형 텍스쳐)
	PARTICLE,		//파티클
	PARTICLE_SYSTEM,//파티클 시스템
}OBJECT_TYPE;

// Light Type
typedef enum LIGHT_TYPE
{
	NONE_LIGHT,
	DIRECTION_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT
}LIGHT_TYPE;

// Debug Mesh Type
typedef enum DEBUG_MESH_TYPE
{
	DEBUG_MESH_RAY,
	DEBUG_MESH_CIRCLE,
	DEBUG_MESH_BOX,
	DEBUG_MESH_SPHERE
}DEBUG_MESH_TYPE;