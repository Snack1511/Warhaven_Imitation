#include "CUI_HUD.h"
#include "GameInstance.h"

#include "CUnit.h"
#include "CUI_Portrait.h"

CUI_HUD::CUI_HUD()
{
}

CUI_HUD::~CUI_HUD()
{
}

HRESULT CUI_HUD::Initialize_Prototype()
{
	m_pWrap[Port] = CUI_Portrait::Create();

	CREATE_GAMEOBJECT(m_pWrap[Port], GROUP_UI);

	return S_OK;
}

HRESULT CUI_HUD::Initialize()
{
	return S_OK;
}

HRESULT CUI_HUD::Start()
{
	dynamic_cast<CUI_Portrait*>(m_pWrap[Port])->Start_Portrait(0);

	m_eCurClass = m_tStatus.eClass;

	return S_OK;
}

void CUI_HUD::My_Tick()
{
	__super::My_Tick();

	Change_UserPort();
}

void CUI_HUD::Change_UserPort()
{
	if (!m_tStatus.m_bIsHero)
	{
		if (KEY(T, TAP))
		{
			// 영웅 변경을 유아이로 하고 싶다
			// 값이 하나씩 증가하는게 아니라

			_uint iIndex = m_eCurClass;
			iIndex++;
			if (iIndex > CUnit::CLASS_ENGINEER)
			{
				iIndex = CUnit::CLASS_WARRIOR;
			}

			m_eCurClass = (CUnit::CLASS_TYPE)iIndex;
			Set_Portrait(m_eCurClass);
		}
		else if (KEY(NUM1, TAP))
		{
			Set_Portrait(CUnit::CLASS_FIONA);
		}
		else if (KEY(NUM2, TAP))
		{
			Set_Portrait(CUnit::CLASS_QANDA);
		}
		else if (KEY(NUM3, TAP))
		{
			Set_Portrait(CUnit::CLASS_HOEDT);
		}
		else if (KEY(NUM4, TAP))
		{
			Set_Portrait(CUnit::CLASS_LANCER);
		}
	}
	else
	{
		if (KEY(NUM1, TAP))
		{
			Set_Portrait(m_ePrvClass);
		}
	}
}

void CUI_HUD::Set_Portrait(CUnit::CLASS_TYPE eClass)
{
	m_ePrvClass = m_eCurClass;
	m_eCurClass = eClass;

	if (!m_tStatus.m_bIsHero)
	{
		if (eClass > CUnit::CLASS_ENGINEER)
		{
			m_tStatus.m_bIsHero = true;
		}
	}
	else
	{
		if (eClass < CUnit::CLASS_FIONA)
		{
			m_tStatus.m_bIsHero = false;
		}
	}

	dynamic_cast<CUI_Portrait*>(m_pWrap[Port])->Set_Portrait(eClass);
}
