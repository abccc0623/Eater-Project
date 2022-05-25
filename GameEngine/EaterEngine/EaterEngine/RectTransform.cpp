#include "RectTransform.h"
#include "EngineData.h"

#include "GameObject.h"
#include "GameEngine.h"

RectTransform::RectTransform()
{
	PivotType		= RECT_PIVOT::PIVOT_MIDDLE_CENTER;
	ImageSize		= { 0, 0 };
	Position		= { 0, 0 };
	Rotation		= { 0, 0, 0 };
	Scale			= { 1, 1 };

	Position_Offset = { 0, 0 };
	Scale_Offset	= { 1, 1 };

	PositionXM		= DirectX::XMMatrixIdentity();
	RotationXM		= DirectX::XMMatrixIdentity();
	ScaleXM			= DirectX::XMMatrixIdentity();
}

RectTransform::~RectTransform()
{

}

void RectTransform::TransformUpdate()
{
	switch (PivotType)
	{
	case PIVOT_LEFT_TOP:
		Position_Offset.x = ImageSize.x * Scale.x;
		Position_Offset.y = ImageSize.y * Scale.y;
		break;
	case PIVOT_LEFT_BOTTOM:
		Position_Offset.x = ImageSize.x * Scale.x;
		Position_Offset.y = GameEngine::WinSizeHeight - (ImageSize.y * Scale.y);
		break;
	case PIVOT_RIGHT_TOP:
		Position_Offset.x = GameEngine::WinSizeWidth - (ImageSize.x * Scale.x);
		Position_Offset.y = ImageSize.y * Scale.y;
		break;
	case PIVOT_RIGHT_BOTTOM:
		Position_Offset.x = GameEngine::WinSizeWidth - (ImageSize.x * Scale.x);
		Position_Offset.y = GameEngine::WinSizeHeight - (ImageSize.y * Scale.y);
		break;
	case PIVOT_MIDDLE_TOP:
		Position_Offset.x = GameEngine::WinSizeWidth * 0.5f;
		Position_Offset.y = ImageSize.y * Scale.y;
		break;
	case PIVOT_MIDDLE_BOTTOM:
		Position_Offset.x = GameEngine::WinSizeWidth * 0.5f;
		Position_Offset.y = GameEngine::WinSizeHeight - (ImageSize.y * Scale.y);
		break;
	case PIVOT_MIDDLE_LEFT:
		Position_Offset.x = ImageSize.x * Scale.x;
		Position_Offset.y = GameEngine::WinSizeHeight * 0.5f;
		break;
	case PIVOT_MIDDLE_RIGHT:
		Position_Offset.x = GameEngine::WinSizeWidth - (ImageSize.x * Scale.x);
		Position_Offset.y = GameEngine::WinSizeHeight * 0.5f;
		break;
	case PIVOT_MIDDLE_CENTER:
		Position_Offset.x = GameEngine::WinSizeWidth * 0.5f;
		Position_Offset.y = GameEngine::WinSizeHeight * 0.5f;
		break;
	default:
		break;
	}

	// Position Matrix..
	PositionXM._41 = Position_Offset.x + Position.x;
	PositionXM._42 = Position_Offset.y + Position.y;

	// Rotation Matrix..
	float radX = Rotation.x * 3.141592f / 180.0f;
	float radY = Rotation.y * 3.141592f / 180.0f;
	float radZ = Rotation.z * 3.141592f / 180.0f;
	DirectX::XMMATRIX _P = DirectX::XMMatrixRotationX(radX);
	DirectX::XMMATRIX _Y = DirectX::XMMatrixRotationY(radY);
	DirectX::XMMATRIX _R = DirectX::XMMatrixRotationZ(radZ);

	RotationXM = DirectX::XMMatrixRotationRollPitchYaw(radX, radY + XM_PI, radZ + XM_PI);

	// Scale Matrix..
	ScaleXM._11 = ImageSize.x * Scale.x;
	ScaleXM._22 = ImageSize.y * Scale.y;

	// ���� Matrix ����..
	WorldXM = ScaleXM * RotationXM * PositionXM;
	gameobject->OneMeshData->UI_Buffer->UI_Property->World = WorldXM;
}

void RectTransform::SetImagePivot(RECT_PIVOT pivot_type)
{
	PivotType = pivot_type;
}

void RectTransform::SetImageSize(DirectX::SimpleMath::Vector2 imagesize)
{
	ImageSize = imagesize;
}

void RectTransform::SetPosition(float x, float y)
{
	Position.x = x;
	Position.y = y;
}

void RectTransform::SetPosition(DirectX::SimpleMath::Vector2 pos)
{
	Position = pos;
}

void RectTransform::SetRotation(float x, float y, float z)
{
	Rotation.x = x;
	Rotation.y = y;
	Rotation.z = z;
}

void RectTransform::SetRotation(DirectX::SimpleMath::Vector3 rot)
{
	Rotation = rot;
}

void RectTransform::SetRotation(float angle)
{
	Rotation.z = angle;
}

void RectTransform::SetScale(float x, float y)
{
	Scale.x = x;
	Scale.y = y;
}

void RectTransform::SetScale(DirectX::SimpleMath::Vector2 scale)
{
	Scale = scale;
}

void RectTransform::AddPosition(float x, float y)
{
	Position.x += x;
	Position.y += y;
}

void RectTransform::AddPosition(DirectX::SimpleMath::Vector2 pos)
{
	Position += pos;
}

void RectTransform::AddRotation(float x, float y, float z)
{
	Rotation.x += x;
	Rotation.y += y;
	Rotation.z += z;
}

void RectTransform::AddRotation(DirectX::SimpleMath::Vector3 rot)
{
	Rotation += rot;
}

void RectTransform::AddScale(float x, float y)
{
	Scale.x += x;
	Scale.y += y;
}

void RectTransform::AddScale(DirectX::SimpleMath::Vector2 scale)
{
	Scale += scale;
}
