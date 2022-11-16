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

CButton* CButton::Create(_uint iGroupID)
{
	CButton* pInstance = new CButton(iGroupID);

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

	Set_Rect();
}

void CButton::Tick()
{
	GetCursorPos(&m_ptMouse);
	ScreenToClient(g_hWnd, &m_ptMouse);
	_float4 vMousePos = CFunctor::To_Window(_float4(m_ptMouse.x, m_ptMouse.y, 0.f));
}

void CButton::Late_Tick()
{
	//m_bIsInMouse = PtInRect(&m_tRect, m_ptMouse) ? true : false;
	//if (m_bIsInMouse)
	//{
	//	if (1)
	//	{
	//		m_pOwnerUI->CallBack_MouseIn(0);
	//	}
	//	else if (1)
	//	{
	//		m_pOwnerUI->CallBack_MouseExit(0);
	//	}
	//	else
	//	{
	//		m_pOwnerUI->CallBack_ButtonClick(0);
	//	}
	//}
}

void CButton::Release()
{
	
}

void CButton::Set_Rect()
{
	_float4 vUIPos = m_pOwnerUI->Get_Pos();
	_float4 vPos = CFunctor::To_Descartes(vUIPos);
	_float4 vScale = m_pOwnerUI->Get_Scale();

	int iLeft = int(vUIPos.x - vScale.x * 0.5f);
	int iTop = int(vUIPos.y - vScale.y * 0.5f);
	int iRight = int(vUIPos.x + vScale.x * 0.5f);
	int iBottom = int(vUIPos.y + vScale.y * 0.5f);

	SetRect(&m_tRect, iLeft, iTop, iRight, iBottom);
}