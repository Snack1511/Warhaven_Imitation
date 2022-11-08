#include "stdafx.h"
#include "CDefault_Effect.h"
#include "Model.h"
#include "GameInstance.h"

CDefault_Effect::CDefault_Effect()
{
}

CDefault_Effect::~CDefault_Effect()
{
}

CDefault_Effect* CDefault_Effect::Create(string strFileKey)
{
	wstring wstrModel = L"../bin/resources/Meshes/Effects/FBX/SM_Charge_Mesh_01.fbx";
	wstring wstrMask = L"../bin/resources/Textures/Effects/GradientMap/T_EFF_Blur_12_M.dds";
	wstring wstrColor = L"../bin/resources/Textures/Effects/GradationColor/T_EFF_GMS_AmeVillage_Sea_01_M.png";
	wstring wstrNoise = L"../bin/resources/Textures/Effects/GradientMap/T_EFF_Blur_12_M.dds";

	CDefault_Effect* pInstance = new CDefault_Effect;

	pInstance->m_wstrModelFilePath = wstrModel;
	pInstance->m_wstrMaskMapFilePath = wstrMask;
	pInstance->m_wstrColorMapFilePath = wstrColor;
	pInstance->m_wstrNoiseMapFilePath = wstrNoise;

	if (FAILED(pInstance->SetUp_Model(wstrModel)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_Model : CDefault_Effect");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CDefault_Effect");
		return nullptr;
	}

	return pInstance;
}

HRESULT CDefault_Effect::Initialize_Prototype()
{
	m_eEffectPassType = VTXEFFECT_PASS_DEFAULT;


	__super::Initialize_Prototype();

	GET_COMPONENT(CModel)->Change_Texture(0, 11, m_wstrMaskMapFilePath);
	GET_COMPONENT(CModel)->Change_Texture(0, 1, m_wstrColorMapFilePath);
	GET_COMPONENT(CModel)->Change_Texture(0, 16, m_wstrNoiseMapFilePath);
	return S_OK;
}

HRESULT CDefault_Effect::Initialize()
{
	m_vEffectShaderFlag = SH_EFFECT_DEFAULT;

	__super::Initialize();

	m_fUVSpeedX = 2.f;

	return S_OK;
}

HRESULT CDefault_Effect::Start()
{
	__super::Start();

	return S_OK;
}

void CDefault_Effect::My_Tick()
{
	__super::My_Tick();
}

void CDefault_Effect::My_LateTick()
{
	__super::My_LateTick();
}
