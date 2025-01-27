﻿// CTAP_MeshFilter.cpp: 구현 파일

#include "pch.h"
#include "Editor.h"
#include "CTAP_MeshFilter.h"
#include "afxdialogex.h"
#include "MeshFilter.h"
#include "EngineData.h"
#include "Material.h"
#include "EditorManager.h"
#include "DialogFactory.h"



// CTAP_MeshFilter 대화 상자

IMPLEMENT_DYNAMIC(CTAP_MeshFilter, CustomDialog)

CTAP_MeshFilter::CTAP_MeshFilter(CWnd* pParent /*=nullptr*/)
	: CustomDialog(IDD_TAP_MESHFILTER, pParent)
	, SkyLight_Index(0)
	, mAlpha(FALSE)
{

}

CTAP_MeshFilter::~CTAP_MeshFilter()
{

}

void CTAP_MeshFilter::SetGameObject(MeshFilter* ObjectMeshFilter)
{
	mMeshFilter = ObjectMeshFilter;
	mMaterial = ObjectMeshFilter->GetMaterial();
	
	std::string MeshName = ObjectMeshFilter->GetBufferName();
	std::string ModelName = ObjectMeshFilter->GetModelName();
	
	MeshName_Edit.SetWindowTextW(ChangeToCString(MeshName));
	ModelName_Edit.SetWindowTextW(ChangeToCString(ModelName));

	Diffuse_Edit.GetWindowRect(EditRect[Diffuse_Index]);
	MeshName_Edit.GetWindowRect(EditRect[MeshName_Index]);
	ModelName_Edit.GetWindowRect(EditRect[ModelName_Index]);
	Nomal_Eidt.GetWindowRect(EditRect[Nomal_Index]);
	ORM_Edit.GetWindowRect(EditRect[ORM_Index]);
	MaterialName_Edit.GetWindowRect(EditRect[Material_Index]);
	EmissiveName_Edit.GetWindowRect(EditRect[Emissive_Index]);

	if (mMaterial != nullptr)
	{
		std::string Diffuse		= mMaterial->GetDiffuseName();
		std::string Nomal		= mMaterial->GetNormalName();
		std::string Emissive	= mMaterial->GetEmissiveName();
		std::string ORM			= mMaterial->GetORMName();

		MaterialName_Edit.SetWindowTextW(ChangeToCString(mMaterial->Name));
		Diffuse_Edit.SetWindowTextW(ChangeToCString(Diffuse));
		Nomal_Eidt.SetWindowTextW(ChangeToCString(Nomal));
		ORM_Edit.SetWindowTextW(ChangeToCString(ORM));
		EmissiveName_Edit.SetWindowTextW(ChangeToCString(Emissive));

		float EmissiveF		= mMaterial->m_MaterialData->Material_Property->EmissiveFactor;
		float RoughnessF	= mMaterial->m_MaterialData->Material_Property->RoughnessFactor;
		float MetallicF		= mMaterial->m_MaterialData->Material_Property->MetallicFactor;

		float AddColorR = mMaterial->m_MaterialData->Material_Property->AddColor.x;
		float AddColorG = mMaterial->m_MaterialData->Material_Property->AddColor.y;
		float AddColorB = mMaterial->m_MaterialData->Material_Property->AddColor.z;

	
		Emissive_Edit.SetWindowTextW(ChangeToCString(EmissiveF));
		Roughness_Edit.SetWindowTextW(ChangeToCString(RoughnessF));
		Matallic_Edit.SetWindowTextW(ChangeToCString(MetallicF));

		Emissive_Slider.SetPos((int)(EmissiveF*10.0f));
		Roughnees_Slider.SetPos((int)(RoughnessF* 100.0f + 100.0f));
		Matallic_Slider.SetPos((int)(MetallicF*100.0f + 100.0f));

		Emissive_Slider.SetRange(0, 100);
		Matallic_Slider.SetRange(0, 200);
		Roughnees_Slider.SetRange(0, 200);

		Vector4 Add = mMaterial->m_MaterialData->Material_Property->AddColor;
	
		Add_R_Slider.SetRange(0, 255);
		Add_G_Slider.SetRange(0, 255);
		Add_B_Slider.SetRange(0, 255);
		Add_R_Slider.SetPos((int)(Add.x * 255.0f));
		Add_G_Slider.SetPos((int)(Add.y * 255.0f));
		Add_B_Slider.SetPos((int)(Add.z * 255.0f));

		AddColor_R.SetWindowTextW(ChangeToCString(AddColorR));
		AddColor_G.SetWindowTextW(ChangeToCString(AddColorG));
		AddColor_B.SetWindowTextW(ChangeToCString(AddColorB));
		COLORREF mMF_Setting = RGB(Add.x, Add.y, Add.z);
		Custom_LimLightColor_Button.SetFaceColor(mMF_Setting);


		Vector4 Lim = mMaterial->m_MaterialData->Material_Property->LimLightColor;
		LimLight_R.SetRange(0, 255);
		LimLight_G.SetRange(0, 255);
		LimLight_B.SetRange(0, 255);
		LimLight_R.SetPos((int)(Lim.x * 255.0f));
		LimLight_G.SetPos((int)(Lim.y * 255.0f));
		LimLight_B.SetPos((int)(Lim.z * 255.0f));

		LimLight_R_Edit.SetWindowTextW(ChangeToCString(Lim.x));
		LimLight_G_Edit.SetWindowTextW(ChangeToCString(Lim.y));
		LimLight_B_Edit.SetWindowTextW(ChangeToCString(Lim.z));
		mMF_Setting = RGB(Lim.x, Lim.y, Lim.z);
		Custom_LimLightColor_Button.SetFaceColor(mMF_Setting);


		float LimFactor = mMaterial->m_MaterialData->Material_Property->LimLightFactor * 10.0f;
		float LimWidth  = mMaterial->m_MaterialData->Material_Property->LimLightWidth * 10.0f;
		LimLight_Factor.SetRange(0, 100);
		LimLight_Width.SetRange(0, 100);
		LimLight_Factor.SetPos((int)LimFactor);
		LimLight_Width.SetPos((int)LimWidth);
		LimLight_Factor_Edit.SetWindowTextW(ChangeToCString(LimFactor / 10.0f));
		LimLight_Width_Edit.SetWindowTextW(ChangeToCString(LimWidth / 10.0f));
		SkyLight_Index = mMaterial->m_MaterialData->Material_Property->SkyLightIndex;

		mAlpha = mMaterial->m_MaterialData->Material_Property->Alpha;
	}
	else
	{
		Add_R_Slider.SetRange(0, 255);
		Add_G_Slider.SetRange(0, 255);
		Add_B_Slider.SetRange(0, 255);
		Add_R_Slider.SetPos(255);
		Add_G_Slider.SetPos(255);
		Add_B_Slider.SetPos(255);


		Emissive_Slider.SetRange(0, 100);
		Matallic_Slider.SetRange(0, 200);
		Roughnees_Slider.SetRange(0, 200);

		Emissive_Slider.SetPos(10);
		Roughnees_Slider.SetPos(100);
		Matallic_Slider.SetPos(100);


		LimLight_R.SetRange(0, 255);
		LimLight_G.SetRange(0, 255);
		LimLight_B.SetRange(0, 255);
		LimLight_R.SetPos(0);
		LimLight_G.SetPos(0);
		LimLight_B.SetPos(0);
		LimLight_R_Edit.SetWindowTextW(L"0");
		LimLight_G_Edit.SetWindowTextW(L"0");
		LimLight_B_Edit.SetWindowTextW(L"0");
		LimLight_Factor.SetPos(0);
		LimLight_Width.SetPos(0);
		LimLight_Factor_Edit.SetWindowTextW(L"0");
		LimLight_Width_Edit.SetWindowTextW(L"0");
		SkyLight_Index = 0;

		mAlpha = false;
	}
	UpdateData(FALSE);
}

void CTAP_MeshFilter::GetData(ObjectOption& Option)
{
	CString Diffuse;
	CString Normal;
	CString Emissive;
	CString ORM;
	Diffuse_Edit.GetWindowTextW(Diffuse);
	Nomal_Eidt.GetWindowTextW(Normal);
	EmissiveName_Edit.GetWindowTextW(Emissive);
	ORM_Edit.GetWindowTextW(ORM);

	Option.Diffuse	= ChangeToString(Diffuse);
	Option.Normal	= ChangeToString(Normal);
	Option.Emissive = ChangeToString(Emissive);
	Option.ORM		= ChangeToString(ORM);

	CString Roughness;
	Roughness_Edit.GetWindowTextW(Roughness);
	Option.Roughness = ChangeToFloat(Roughness);

	CString Metallic;
	Matallic_Edit.GetWindowTextW(Metallic);
	Option.Metallic = ChangeToFloat(Metallic);

	CString Add_R;
	CString Add_G;
	CString Add_B;
	AddColor_R.GetWindowTextW(Add_R);
	AddColor_G.GetWindowTextW(Add_G);
	AddColor_B.GetWindowTextW(Add_B);

	Option.AddColor = { ChangeToFloat(Add_R),ChangeToFloat(Add_G) ,ChangeToFloat(Add_B) };
}

void CTAP_MeshFilter::UpdateGameObject()
{
	CString Name00;
	CString Name01;
	CString Name02;
	Emissive_Edit.GetWindowTextW(Name00);
	Roughness_Edit.GetWindowTextW(Name01);
	Matallic_Edit.GetWindowTextW(Name02);

	if (Name00 != "")
	{
		float Emissive = ChangeToFloat(Name00);
		mMaterial->m_MaterialData->Material_Property->EmissiveFactor = ChangeToFloat(Name00);
	}

	if (Name01 != "")
	{
		mMaterial->m_MaterialData->Material_Property->RoughnessFactor = ChangeToFloat(Name01);
	}

	if (Name02 != "")
	{
		mMaterial->m_MaterialData->Material_Property->MetallicFactor = ChangeToFloat(Name02);
	}

	CString GetNumber;
	Vector3 AddColor;

	AddColor_R.GetWindowTextW(GetNumber);
	AddColor.x = ChangeToFloat(GetNumber);

	AddColor_G.GetWindowTextW(GetNumber);
	AddColor.y = ChangeToFloat(GetNumber);

	AddColor_B.GetWindowTextW(GetNumber);
	AddColor.z = ChangeToFloat(GetNumber);

	if (mMaterial != nullptr)
	{
		mMaterial->SetAddColor(AddColor);
	}
}

void CTAP_MeshFilter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, Diffuse_Edit);
	DDX_Control(pDX, IDC_EDIT4, MeshName_Edit);
	DDX_Control(pDX, IDC_EDIT3, Nomal_Eidt);
	DDX_Control(pDX, IDC_EDIT5, ORM_Edit);
	DDX_Control(pDX, IDC_SLIDER1, Emissive_Slider);
	DDX_Control(pDX, IDC_SLIDER2, Roughnees_Slider);
	DDX_Control(pDX, IDC_SLIDER3, Matallic_Slider);
	DDX_Control(pDX, IDC_EDIT2, Emissive_Edit);
	DDX_Control(pDX, IDC_EDIT7, Roughness_Edit);
	DDX_Control(pDX, IDC_EDIT8, Matallic_Edit);
	DDX_Control(pDX, IDC_EDIT9, EmissiveName_Edit);
	DDX_Control(pDX, IDC_EDIT21, AddColor_R);
	DDX_Control(pDX, IDC_EDIT22, AddColor_G);
	DDX_Control(pDX, IDC_EDIT23, AddColor_B);
	DDX_Control(pDX, IDC_SLIDER7, Add_R_Slider);
	DDX_Control(pDX, IDC_SLIDER8, Add_G_Slider);
	DDX_Control(pDX, IDC_SLIDER9, Add_B_Slider);
	DDX_Control(pDX, IDC_EDIT15, ModelName_Edit);
	DDX_Control(pDX, IDC_EDIT16, MaterialName_Edit);
	DDX_Control(pDX, IDC_SLIDER10, LimLight_R);
	DDX_Control(pDX, IDC_SLIDER11, LimLight_G);
	DDX_Control(pDX, IDC_SLIDER12, LimLight_B);
	DDX_Control(pDX, IDC_EDIT24, LimLight_R_Edit);
	DDX_Control(pDX, IDC_EDIT25, LimLight_G_Edit);
	DDX_Control(pDX, IDC_EDIT26, LimLight_B_Edit);
	DDX_Control(pDX, IDC_SLIDER4, LimLight_Factor);
	DDX_Control(pDX, IDC_SLIDER5, LimLight_Width);
	DDX_Control(pDX, IDC_EDIT18, LimLight_Factor_Edit);
	DDX_Control(pDX, IDC_EDIT27, LimLight_Width_Edit);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON1, Custom_AddColor_Button);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON2, Custom_LimLightColor_Button);
	DDX_Text(pDX, IDC_EDIT28, SkyLight_Index);
	DDX_Control(pDX, IDC_SPIN3, SkyLight_Spin);
	DDX_Check(pDX, IDC_CHECK1, mAlpha);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON3, Custum_Emissive_Color);
}

BOOL CTAP_MeshFilter::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	Emissive_Slider.SetRange(0, 100);
	Roughnees_Slider.SetRange(0, 100);
	Matallic_Slider.SetRange(0, 100);

	Emissive_Edit.SetWindowTextW(L"");
	Roughness_Edit.SetWindowTextW(L"");
	Matallic_Edit.SetWindowTextW(L"");

	AddColor_R.SetWindowTextW(L"255");
	AddColor_G.SetWindowTextW(L"255");
	AddColor_B.SetWindowTextW(L"255");

	LimLight_R.SetRange(0, 255);
	LimLight_G.SetRange(0, 255);
	LimLight_B.SetRange(0, 255);
	LimLight_R.SetPos(0);
	LimLight_G.SetPos(0);
	LimLight_B.SetPos(0);
	LimLight_R_Edit.SetWindowTextW(L"0");
	LimLight_G_Edit.SetWindowTextW(L"0");
	LimLight_B_Edit.SetWindowTextW(L"0");
	
	SkyLight_Spin.SetRange(-100,100);
	SkyLight_Spin.SetPos(0);
	return 0;
}

void CTAP_MeshFilter::SetSlider(CSliderCtrl& Silder,int Number)
{
	CString str;
	str.Format(_T("%d"), Number); //Format을 이용하여 int값을 변경
	Silder.SetWindowTextW(str);
	UpdateData(false); //set을 해주기 위해서 UpdateData(false); 를 사용

}

BEGIN_MESSAGE_MAP(CTAP_MeshFilter, CDialogEx)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(M_MSG_MeshFilter, &CTAP_MeshFilter::OnUserFun)
	ON_BN_CLICKED(IDC_BUTTON1, &CTAP_MeshFilter::OnDiffuse_Button)
	ON_BN_CLICKED(IDC_BUTTON2, &CTAP_MeshFilter::OnNomal_Button)
	ON_BN_CLICKED(IDC_BUTTON10, &CTAP_MeshFilter::OnORM_Button)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON26, &CTAP_MeshFilter::OnEmissive_Button)
	ON_BN_CLICKED(IDC_BUTTON5, &CTAP_MeshFilter::OnMaterialName_Button)
	ON_BN_CLICKED(IDC_BUTTON24, &CTAP_MeshFilter::OnBnClickedButton24)
	ON_BN_CLICKED(IDC_BUTTON29, &CTAP_MeshFilter::OnBnClickedButton29)
	ON_BN_CLICKED(IDC_BUTTON30, &CTAP_MeshFilter::OnBnClickedButton30)
	ON_BN_CLICKED(IDC_BUTTON31, &CTAP_MeshFilter::OnBnClickedButton31)
	ON_BN_CLICKED(IDC_BUTTON32, &CTAP_MeshFilter::OnCreateMaterial)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON1, &CTAP_MeshFilter::OnCustom_Color_Button)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON2, &CTAP_MeshFilter::OnCustom_LimLightColor_Button)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CTAP_MeshFilter::OnDeltaposSpin3)
	ON_BN_CLICKED(IDC_CHECK1, &CTAP_MeshFilter::OnAlphaButton)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON3, &CTAP_MeshFilter::OnCustom_EmissiveColor_Button)
END_MESSAGE_MAP()

LRESULT CTAP_MeshFilter::OnUserFun(WPARAM wParam, LPARAM lparam)
{
	CString* pstrString = (CString*)lparam;
	CString strString = pstrString->GetString();
	std::string FileName = ChangeToString(strString);
	FileName = FileName.substr(0, FileName.rfind('.'));

	int Type = GetFileNameType(strString);

	POINT point;
	GetCursorPos(&point);

	if (DropRect(EditRect[ModelName_Index]))
	{
		if (Type == EATER)
		{
			ModelName_Edit.SetWindowTextW(strString);
			mMeshFilter->SetModelName(FileName);
		}
		else
		{
			AfxMessageBox(L"Error : Eater 파일만 가능합니다");
		}
	}
	
	if (DropRect(EditRect[MeshName_Index]))
	{
		if (Type == EMESH)
		{
			MeshName_Edit.SetWindowTextW(strString);
			mMeshFilter->SetMeshName(FileName);
		}
		else
		{
			AfxMessageBox(L"Error : Emesh 파일만 가능합니다");
		}
	}
	
	if (DropRect(EditRect[Material_Index]))
	{
		if (Type == EMAT)
		{
			MaterialName_Edit.SetWindowTextW(strString);
			mMeshFilter->SetMaterialName(FileName);

			Material* m = mMeshFilter->GetMaterial();
			Diffuse_Edit.SetWindowTextW(ChangeToCString(m->GetDiffuseName()));
			Nomal_Eidt.SetWindowTextW(ChangeToCString(m->GetNormalName()));
			Emissive_Edit.SetWindowTextW(ChangeToCString(m->GetEmissiveName()));
			ORM_Edit.SetWindowTextW(ChangeToCString(m->GetORMName()));
		}
		else
		{
			AfxMessageBox(L"Error : Emat 파일만 가능합니다");
		}
	}
	
	
	for (int i = 0; i <= 3; i++)
	{
		if (DropRect(EditRect[i]))
		{
			if (Type == PNG || Type == DDS)
			{
				switch (i)
				{
				case 0:
					Diffuse_Edit.SetWindowTextW(strString);
					mMeshFilter->SetDiffuseTextureName(FileName);
					break;
				case 1:
					Nomal_Eidt.SetWindowTextW(strString);
					mMeshFilter->SetNormalTextureName(FileName);
					break;
				case 2:
					ORM_Edit.SetWindowTextW(strString);
					mMeshFilter->SetORMTextureName(FileName);
					break;
				case 3:
					EmissiveName_Edit.SetWindowTextW(strString);
					mMeshFilter->SetEmissiveTextureName(FileName);
					break;
				}
			}
			else
			{
				AfxMessageBox(L"Error : PNG , DDS 파일만 가능합니다");
			}
		}
	}
	return LRESULT();
}

void CTAP_MeshFilter::OnDiffuse_Button()
{
	CFileDialog mLoadFile(TRUE);
	char full[_MAX_PATH];
	_fullpath(full, "../Assets/Texture/ModelTexture", _MAX_PATH);
	CString Temp;
	Temp = full;
	mLoadFile.m_ofn.lpstrInitialDir = Temp.GetBuffer();


	if (mLoadFile.DoModal() == IDOK)
	{
		CString Name = mLoadFile.GetFileName();
		std::string FileName = CutStringFileType(Name);
		Diffuse_Edit.SetWindowTextW(ChangeToCString(FileName));
		mMeshFilter->SetDiffuseTextureName(FileName);
	}
}

void CTAP_MeshFilter::OnNomal_Button()
{
	CFileDialog mLoadFile(TRUE);
	char full[_MAX_PATH];
	_fullpath(full, "../Assets/Texture/ModelTexture", _MAX_PATH);
	CString Temp;
	Temp = full;
	mLoadFile.m_ofn.lpstrInitialDir = Temp.GetBuffer();

	if (mLoadFile.DoModal() == IDOK)
	{
		CString Name = mLoadFile.GetFileName();
		std::string FileName = CutStringFileType(Name);
		Nomal_Eidt.SetWindowTextW(ChangeToCString(FileName));
		mMeshFilter->SetNormalTextureName(FileName);
	}
}

void CTAP_MeshFilter::OnEmissive_Button()
{
	CFileDialog mLoadFile(TRUE);
	char full[_MAX_PATH];
	_fullpath(full, "../Assets/Texture/ModelTexture", _MAX_PATH);
	CString Temp;
	Temp = full;
	mLoadFile.m_ofn.lpstrInitialDir = Temp.GetBuffer();

	if (mLoadFile.DoModal() == IDOK)
	{
		CString Name = mLoadFile.GetFileName();
		std::string FileName = CutStringFileType(Name);
		EmissiveName_Edit.SetWindowTextW(ChangeToCString(FileName));
		mMeshFilter->SetEmissiveTextureName(FileName);
	}
}
void CTAP_MeshFilter::OnORM_Button()
{
	CFileDialog mLoadFile(TRUE);
	char full[_MAX_PATH];
	_fullpath(full, "../Assets/Texture/ModelTexture", _MAX_PATH);
	CString Temp;
	Temp = full;
	mLoadFile.m_ofn.lpstrInitialDir = Temp.GetBuffer();

	if (mLoadFile.DoModal() == IDOK)
	{
		CString Name = mLoadFile.GetFileName();
		std::string FileName = CutStringFileType(Name);
		ORM_Edit.SetWindowTextW(ChangeToCString(FileName));
		mMeshFilter->SetORMTextureName(FileName);
	}
}


BOOL CTAP_MeshFilter::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:
			UpdateGameObject();
			return TRUE;
		default:
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CTAP_MeshFilter::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	mMaterial = mMeshFilter->GetMaterial();
	if (mMaterial == nullptr) { return; }

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (pScrollBar->GetDlgCtrlID() == Emissive_Slider.GetDlgCtrlID())
	{
		float Emissive = (float)Emissive_Slider.GetPos();
		Emissive *= 0.1f;

		Emissive_Edit.SetWindowTextW(ChangeToCString(Emissive));
		mMaterial->SetEmissiveFactor(Emissive);
	}

	if (pScrollBar->GetDlgCtrlID() == Roughnees_Slider.GetDlgCtrlID())
	{
		float Roughnees = (float)Roughnees_Slider.GetPos();
		Roughnees -= 100;
		Roughnees *= 0.01f;
		Roughness_Edit.SetWindowTextW(ChangeToCString(Roughnees));
		mMaterial->SetRoughnessFactor(Roughnees);
	}

	if (pScrollBar->GetDlgCtrlID() == Matallic_Slider.GetDlgCtrlID())
	{
		float Matallic = (float)Matallic_Slider.GetPos();
		Matallic -= 100;
		Matallic *= 0.01f;
		Matallic_Edit.SetWindowTextW(ChangeToCString(Matallic));
		mMaterial->SetMetallicFactor(Matallic);
	}

	if (pScrollBar->GetDlgCtrlID() == Add_R_Slider.GetDlgCtrlID())
	{
		int AddColor = Add_R_Slider.GetPos();
		mMaterial->m_MaterialData->Material_Property->AddColor.x = AddColor / 255.0f;
		AddColor_R.SetWindowTextW(ChangeToCString(AddColor / 255.0f));
	}

	if (pScrollBar->GetDlgCtrlID() == Add_G_Slider.GetDlgCtrlID())
	{
		int AddColor = Add_G_Slider.GetPos();
		mMaterial->m_MaterialData->Material_Property->AddColor.y = AddColor / 255.0f;
		AddColor_G.SetWindowTextW(ChangeToCString(AddColor / 255.0f));
	}

	if (pScrollBar->GetDlgCtrlID() == Add_B_Slider.GetDlgCtrlID())
	{
		int AddColor = Add_B_Slider.GetPos();
		mMaterial->m_MaterialData->Material_Property->AddColor.z = AddColor / 255.0f;
		AddColor_B.SetWindowTextW(ChangeToCString(AddColor / 255.0f));
	}

	if (pScrollBar->GetDlgCtrlID() == LimLight_R.GetDlgCtrlID())
	{
		int LimLightColor = LimLight_R.GetPos();
		mMaterial->m_MaterialData->Material_Property->LimLightColor.x = LimLightColor / 255.0f;
		LimLight_R_Edit.SetWindowTextW(ChangeToCString(LimLightColor / 255.0f));
	}

	if (pScrollBar->GetDlgCtrlID() == LimLight_G.GetDlgCtrlID())
	{
		int LimLightColor = LimLight_G.GetPos();
		mMaterial->m_MaterialData->Material_Property->LimLightColor.y = LimLightColor / 255.0f;
		LimLight_G_Edit.SetWindowTextW(ChangeToCString(LimLightColor / 255.0f));
	}

	if (pScrollBar->GetDlgCtrlID() == LimLight_B.GetDlgCtrlID())
	{
		int LimLightColor = LimLight_B.GetPos();
		mMaterial->m_MaterialData->Material_Property->LimLightColor.z = LimLightColor / 255.0f;
		LimLight_B_Edit.SetWindowTextW(ChangeToCString(LimLightColor / 255.0f));
	}

	if (pScrollBar->GetDlgCtrlID() == LimLight_Factor.GetDlgCtrlID())
	{
		int Factor = LimLight_Factor.GetPos();
		mMaterial->m_MaterialData->Material_Property->LimLightFactor = Factor / 10.0f;
		LimLight_Factor_Edit.SetWindowTextW(ChangeToCString(Factor / 10.0f));
	}

	if (pScrollBar->GetDlgCtrlID() == LimLight_Width.GetDlgCtrlID())
	{
		int Width = LimLight_Width.GetPos();
		mMaterial->m_MaterialData->Material_Property->LimLightWidth = Width / 10.0f;
		LimLight_Width_Edit.SetWindowTextW(ChangeToCString(Width / 10.0f));
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CTAP_MeshFilter::OnMaterialName_Button()
{
	
}

void CTAP_MeshFilter::OnBnClickedButton24()
{
	Diffuse_Edit.SetWindowTextW(L"");
	mMeshFilter->SetDiffuseTextureName("");
}


void CTAP_MeshFilter::OnBnClickedButton29()
{
	Nomal_Eidt.SetWindowTextW(L"");
	mMeshFilter->SetNormalTextureName("");
}


void CTAP_MeshFilter::OnBnClickedButton30()
{
	Emissive_Edit.SetWindowTextW(L"");
	mMeshFilter->SetEmissiveTextureName("");
}


void CTAP_MeshFilter::OnBnClickedButton31()
{
	ORM_Edit.SetWindowTextW(L"");
	mMeshFilter->SetORMTextureName("");
}


void CTAP_MeshFilter::OnCreateMaterial()
{
	EditorManager* mEater = DialogFactory::GetFactory()->GetEditorManager();
	CFileDialog mSaveFile(FALSE, L"*.Emat", NULL, OFN_OVERWRITEPROMPT, L"Material(*.Emat)|*.Emat");
	char full[_MAX_PATH];
	_fullpath(full, "../Assets/Texture/Material", _MAX_PATH);
	CString Temp;
	Temp = full;
	mSaveFile.m_ofn.lpstrInitialDir = Temp.GetBuffer();

	if (mSaveFile.DoModal() == IDOK)
	{
		mEater->CreateMaterialData(mMeshFilter->gameobject, ChangeFilePath(mSaveFile.GetPathName()));
	}
}


void CTAP_MeshFilter::OnCustom_Color_Button()
{
	COLORREF mRGB = Custom_AddColor_Button.GetColor();
	float R = GetRValue(mRGB) / 255.0f;
	float G = GetGValue(mRGB) / 255.0f;
	float B = GetBValue(mRGB) / 255.0f;

	AddColor_R.SetWindowTextW(ChangeToCString(R));
	AddColor_G.SetWindowTextW(ChangeToCString(G));
	AddColor_B.SetWindowTextW(ChangeToCString(B));

	if (mMaterial != nullptr && mMaterial->m_MaterialData != nullptr)
	{
		mMaterial->m_MaterialData->Material_Property->AddColor.x = R;
		mMaterial->m_MaterialData->Material_Property->AddColor.y = G;
		mMaterial->m_MaterialData->Material_Property->AddColor.z = B;

		Add_R_Slider.SetPos(GetRValue(mRGB));
		Add_G_Slider.SetPos(GetGValue(mRGB));
		Add_B_Slider.SetPos(GetBValue(mRGB));
	}
}


void CTAP_MeshFilter::OnCustom_LimLightColor_Button()
{
	COLORREF mRGB = Custom_LimLightColor_Button.GetColor();
	float R = GetRValue(mRGB) / 255.0f;
	float G = GetGValue(mRGB) / 255.0f;
	float B = GetBValue(mRGB) / 255.0f;

	LimLight_R_Edit.SetWindowTextW(ChangeToCString(R));
	LimLight_G_Edit.SetWindowTextW(ChangeToCString(G));
	LimLight_B_Edit.SetWindowTextW(ChangeToCString(B));

	if (mMaterial->m_MaterialData != nullptr)
	{
		mMaterial->m_MaterialData->Material_Property->LimLightColor.x = R;
		mMaterial->m_MaterialData->Material_Property->LimLightColor.y = G;
		mMaterial->m_MaterialData->Material_Property->LimLightColor.z = B;

		LimLight_R.SetPos(GetRValue(mRGB));
		LimLight_G.SetPos(GetGValue(mRGB));
		LimLight_B.SetPos(GetBValue(mRGB));
	}
}


void CTAP_MeshFilter::OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (pNMUpDown->iDelta > 0)
	{
		SkyLight_Index++;
		if (mMaterial->m_MaterialData != nullptr && mMaterial != nullptr)
		{
			mMaterial->m_MaterialData->Material_Property->SkyLightIndex = SkyLight_Index;
		}
	}
	else
	{
		SkyLight_Index--;

		// 인덱스가 음수일경우 보정..
		if (SkyLight_Index < 0)
		{
			SkyLight_Spin.SetPos(0);
			SkyLight_Index = 0;
		}

		if (mMaterial->m_MaterialData != nullptr)
		{
			mMaterial->m_MaterialData->Material_Property->SkyLightIndex = SkyLight_Index;
		}
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CTAP_MeshFilter::OnAlphaButton()
{
	if (mMaterial != nullptr && mMaterial->m_MaterialData != nullptr && mMaterial->m_MaterialData->Material_Property != nullptr) 
	{
		UpdateData(true);
		mMaterial->SetAlpha(mAlpha);
		UpdateData(false);
	}
}


void CTAP_MeshFilter::OnCustom_EmissiveColor_Button()
{
	COLORREF mRGB = Custum_Emissive_Color.GetColor();
	float R = GetRValue(mRGB) / 255.0f;
	float G = GetGValue(mRGB) / 255.0f;
	float B = GetBValue(mRGB) / 255.0f;

	if (mMaterial != nullptr && mMaterial->m_MaterialData != nullptr)
	{
		mMaterial->m_MaterialData->Material_Property->EmissiveColor.x = R;
		mMaterial->m_MaterialData->Material_Property->EmissiveColor.y = G;
		mMaterial->m_MaterialData->Material_Property->EmissiveColor.z = B;
	}
}
