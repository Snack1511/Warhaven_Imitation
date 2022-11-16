#include "stdafx.h"
#include "CButton.h"

#include "UsefulHeaders.h"

#include "CUI_Object.h"

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

	m_pOwnerUI = static_cast<CUI_Object*>(m_pOwner);

	Set_Rect();
}

void CButton::Tick()
{
	GetCursorPos(&m_ptMouse);
	ScreenToClient(g_hWnd, &m_ptMouse);
}

void CButton::Late_Tick()
{
	m_bIsInMouse = PtInRect(&m_tRect, m_ptMouse) ? true : false;

	if (m_bIsInMouse)
	{
		m_pOwnerUI->CallBack_MouseIn(0);

		if (KEY(LBUTTON, TAP))
		{
			m_pOwnerUI->CallBack_ButtonClick(0);
		}
	}
	else
	{
		// m_pOwnerUI->CallBack_MouseExit(0);
	}
}

void CButton::Release()
{
}

void CButton::Set_Rect()
{
	_float4 vUIPos = m_pOwnerUI->Get_Transform()->Get_MyWorld(WORLD_POS);
	_float4 vScale = m_pOwnerUI->Get_Scale();

	vUIPos = CFunctor::To_Window(_float4(vUIPos.x, -vUIPos.y, 0.f));

	int iLeft = int(vUIPos.x - vScale.x * 0.5f);
	int iTop = int(vUIPos.y - vScale.y * 0.5f);
	int iRight = int(vUIPos.x + vScale.x * 0.5f);
	int iBottom = int(vUIPos.y + vScale.y * 0.5f);

	SetRect(&m_tRect, iLeft, iTop, iRight, iBottom);
}