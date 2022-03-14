#pragma once
#include <windows.h>
#include <string>
#include <queue>
#include "GraphicDLL.h"

/// <summary>
/// 게임 엔진에서 제공하는 그래픽 엔진 최상위 클래스
/// 해당 클래스를 통해 Graphic Engine을 받을 수 있다..
/// </summary>

class MeshData;
class GlobalData;
class IndexBuffer;
class VertexBuffer;
class TextureBuffer;
class LoadMeshData;
class RenderOption;

namespace ParserData 
{
	class Mesh;
}

class GraphicEngine
{
protected:
	GraphicEngine() {};
	virtual ~GraphicEngine() {};

public:
	/// Graphic Engine Create Function..
	static GRAPHIC_DLL GraphicEngine* Create();

public:
	/// Graphic Engine Funtion..
	virtual GRAPHIC_DLL void Initialize(HWND _hWnd, int screenWidth, int screenHeight) abstract;
	virtual GRAPHIC_DLL void OnReSize(int screenWidth, int screenHeight) abstract;				
	virtual GRAPHIC_DLL void Release() abstract;

	/// Graphic Data Setting Function..
	virtual GRAPHIC_DLL void RenderSetting(RenderOption* renderOption) abstract;
	virtual GRAPHIC_DLL void SetGlobalData(GlobalData* globalData) abstract;
	virtual GRAPHIC_DLL void SetEnvironmentMap(bool enable) abstract;

	/// Render Mesh Data & Rendering Function..
	virtual GRAPHIC_DLL void AddMeshData(MeshData* mesh) abstract;
	virtual GRAPHIC_DLL void DeleteMeshData(MeshData* meshData) abstract;
	virtual GRAPHIC_DLL void ConvertMeshData() abstract;

	virtual GRAPHIC_DLL void Render() abstract;

	/// Graphic Resource Create Function..
	virtual GRAPHIC_DLL TextureBuffer* CreateTextureBuffer(std::string path) abstract;
	virtual GRAPHIC_DLL void CreateMeshBuffer(ParserData::Mesh* mesh, LoadMeshData* meshData) abstract;

	virtual GRAPHIC_DLL void CreateEnvironmentMap(std::string path) abstract;
};
