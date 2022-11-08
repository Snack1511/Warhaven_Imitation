#include "stdafx.h"
#include "CTestEffect.h"

#include "GameInstance.h"
#include "Model.h"



CTestEffect::CTestEffect()
{
}

CTestEffect::~CTestEffect()
{
}

CTestEffect* CTestEffect::Create(wstring wstrModelPath, wstring wstrMaskPath, wstring wstrColorPath, wstring wstrNoisePath)
{
	CTestEffect* pInstance = new CTestEffect;

	pInstance->m_wstrModelFilePath = wstrModelPath;
	pInstance->m_wstrMaskMapFilePath = wstrMaskPath;
	pInstance->m_wstrColorMapFilePath = wstrColorPath;
	pInstance->m_wstrNoiseMapFilePath = wstrNoisePath;

	if (FAILED(pInstance->SetUp_Model(wstrModelPath)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_Model : CTestEffect");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CTestEffect");
		return nullptr;
	}

	return pInstance;
}

HRESULT CTestEffect::Initialize_Prototype()
{
	m_eEffectPassType = VTXEFFECT_PASS_DEFAULT;


	__super::Initialize_Prototype();

	GET_COMPONENT(CModel)->Change_Texture(0, 11 , m_wstrMaskMapFilePath);
	GET_COMPONENT(CModel)->Change_Texture(0, 1, m_wstrColorMapFilePath);
	GET_COMPONENT(CModel)->Change_Texture(0, 16, m_wstrNoiseMapFilePath);
	return S_OK;
}

HRESULT CTestEffect::Initialize()
{
	m_vEffectShaderFlag = SH_EFFECT_DEFAULT;

	__super::Initialize();

	m_fUVSpeedX = 2.f;

	return S_OK;
}

HRESULT CTestEffect::Start()
{
	__super::Start();

	return S_OK;
}

void CTestEffect::My_Tick()
{
	__super::My_Tick();
}

void CTestEffect::My_LateTick()
{
	__super::My_LateTick();

}
