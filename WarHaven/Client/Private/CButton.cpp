#include "stdafx.h"
#include "CButton.h"

#include "UsefulHeaders.h"

#include "CUI.h"

CButton::CButton(_uint iIdx)
	: CComponent(iIdx)
{
}

CButton::~CButton()
{
	Release();
}

CButton* CButton::Create(COMPONENT_PIPELINE ePipeLine)
{
	CButton* pInstance = new CButton(ePipeLine);


	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CButton");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CButton::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
}

void CButton::On_MouseInEvent(const _uint& iEventNum)
{
}

void CButton::On_MouseExitEvent(const _uint& iEventNum)
{
}

void CButton::On_ButtonClickEvent(const _uint& iEventNum)
{
}

HRESULT CButton::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CButton::Initialize()
{
	return S_OK;
}

void CButton::Start()
{
	__super::Start();

	//RECT 만드셈
}

void CButton::Tick()
{
	
}

void CButton::Late_Tick()
{
	//마우스 올려진거 체크



	//마우스 올려지면
	if (1)
	{
		m_pMyUI->CallBack_MouseIn(0);
	}
	else if (1)
	{
		m_pMyUI->CallBack_MouseExit(0);

	}
	else
	{
		m_pMyUI->CallBack_ButtonClick(0);

	}
}

void CButton::Release()
{
}

void CButton::OnEnable()
{
	__super::OnEnable();
	//m_vTargetPos = m_pOwner->Get_Transform()->Get_World(WORLD_POS);
	//m_vOriginScale = m_pOwner->Get_Transform()->Get_Scale();
	BIND_SHADERRESOURCES(CButton, "g_fAlpha");
}

void CButton::OnDisable()
{
	__super::OnDisable();

	//REMOVE_SHADERRESOURCES(CButton, "g_fAlpha");
}

void CButton::OnDead()
{
	REMOVE_SHADERRESOURCES(CButton, "g_fAlpha");
}
