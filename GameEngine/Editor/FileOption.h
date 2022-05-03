﻿#pragma once


// FileOption 대화 상자
#include <string>
#include "EditorData.h"
#include "GameObject.h"
#include "CustomDialog.h"
class RightOption;
class EditorManager;
class SceneSaveDialog;
class CreateMaterial;
class RenderOption;

class FileOption : public CustomDialog
{
	DECLARE_DYNAMIC(FileOption)

public:
	FileOption(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~FileOption();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BUTTON_OPTION };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	RightOption*		mRightOption;
	SceneSaveDialog*	mScene;
	CreateMaterial*		mMaterial;
	DECLARE_MESSAGE_MAP()
public:
	void Initialize(RightOption* mOption);
	void SetChoiceGameObjectName(std::string Name, GameObject* Obj);
public:
	CEdit AddComponent_ObjectName_Edit;
	GameObject* ChoiceObject;

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnUserFunc(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCreateTerrain();
	afx_msg void OnCreateLight();
	afx_msg void OnCreateParticle();
	afx_msg void OnSceneSave();
	afx_msg void OnOpenAssetsFolder();
	afx_msg void OnOpenExe();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCreateCamera();
	afx_msg void OnCreateGameObject();
	afx_msg void OnAddCollider();
	afx_msg void OnAddRigidbody();
	afx_msg void OnAddLight();
	afx_msg void OnCreateMaterial();
	afx_msg void OnDebugButton();

	CButton Debug_Check;
	CButton SSAO_Check;
	CButton FOG_Check;
	CButton Shadow_Check;
	CButton IBL_Check;
	CButton Bloom_Check;
	CButton FXAA_Check;

	RenderOption* mRenderOption;
	afx_msg void OnHDR_Button();
	afx_msg void OnSSAO_Button();
	afx_msg void OnFOG_Button();
	afx_msg void OnShadow_Button();
	afx_msg void OnIBL_Button();
	afx_msg void OnBloom_Button();
	afx_msg void OnFXAA_Button();
	CButton HDR_Check;
};
