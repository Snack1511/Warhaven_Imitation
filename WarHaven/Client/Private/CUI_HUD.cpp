#include "CUI_HUD.h"
#include "GameInstance.h"

#include "CUnit.h"

#include "CUI_CrossHair.h"
#include "CUI_Portrait.h"
#include "CUI_Skill.h"
#include "CUI_HeroGauge.h"

CUI_HUD::CUI_HUD()
{
}

CUI_HUD::~CUI_HUD()
{
}

HRESULT CUI_HUD::Initialize_Prototype()
{
	m_pWrap[Crosshair] = CUI_Crosshair::Create();
	m_pWrap[Port] = CUI_Portrait::Create();
	m_pWrap[Skill] = CUI_Skill::Create();
	m_pWrap[HeroGauge] = CUI_HeroGauge::Create();

	CREATE_GAMEOBJECT(m_pWrap[Port], GROUP_UI);
	CREATE_GAMEOBJECT(m_pWrap[Crosshair], GROUP_UI);
	CREATE_GAMEOBJECT(m_pWrap[Skill], GROUP_UI);
	CREATE_GAMEOBJECT(m_pWrap[HeroGauge], GROUP_UI);

	return S_OK;
}

HRESULT CUI_HUD::Initialize()
{
	return S_OK;
}

HRESULT CUI_HUD::Start()
{
	m_eCurClass = m_tStatus.eClass;

	dynamic_cast<CUI_Portrait*>(m_pWrap[Port])->Start_Portrait(m_eCurClass);
	dynamic_cast<CUI_Crosshair*>(m_pWrap[Crosshair])->Set_Crosshair(m_eCurClass);
	dynamic_cast<CUI_Skill*>(m_pWrap[Skill])->Set_SkillHUD(m_eCurClass);

	dynamic_cast<CUI_HeroGauge*>(m_pWrap[HeroGauge])->Start_HeroGauge();

	return S_OK;
}

void CUI_HUD::My_Tick()
{
	__super::My_Tick();

	m_fHeroGauge = m_tStatus.fHeroGague / m_tStatus.fMaxHeroGauge;
	dynamic_cast<CUI_HeroGauge*>(m_pWrap[HeroGauge])->Set_HeroValue(m_fHeroGauge);

	if (!m_tStatus.bAbleHero)
	{
		if (m_tStatus.bIsHero)
		{
			m_tStatus.fHeroGague += fDT(0) * 20.f;
			if (m_fHeroGauge >= 1.f)
			{
				m_fHeroGauge = 1.f;
				m_tStatus.bIsHero = false;

				Set_HUD(m_ePrvClass);
			}
			else if (KEY(NUM1, TAP))
			{
				Set_HUD(m_ePrvClass);
			}
		}
		else
		{
			// 영웅 변경 키를 누르면 영웅 변경
			if (KEY(T, TAP))
			{
				// 영웅 변경 창 활성화
			}

			m_tStatus.fHeroGague -= fDT(0) * 20.f;
			if (m_fHeroGauge <= 0.f)
			{
				m_fHeroGauge = 0.f;
				m_tStatus.bAbleHero = true;

				Set_ActiveHeroPort(true);
			}
		}
	}
	else
	{
		if (KEY(NUM1, TAP))
		{
			Set_HUD(CUnit::FIONA);
		}
		else if (KEY(NUM2, TAP))
		{
			Set_HUD(CUnit::QANDA);
		}
		else if (KEY(NUM3, TAP))
		{
			Set_HUD(CUnit::HOEDT);
		}
		else if (KEY(NUM4, TAP))
		{
			Set_HUD(CUnit::LANCER);
		}
	}
}

void CUI_HUD::Set_HUD(CUnit::CLASS_TYPE eClass)
{
	m_ePrvClass = m_eCurClass;
	m_eCurClass = eClass;

	if (eClass > CUnit::ENGINEER)
	{
		if (m_tStatus.bAbleHero)
		{
			Set_ActiveHeroPort(false);
		}

		m_tStatus.bAbleHero = false;
		m_tStatus.bIsHero = true;
	}
	else
	{
		m_tStatus.bIsHero = false;
	}

	dynamic_cast<CUI_Portrait*>(m_pWrap[Port])->Set_Portrait(eClass);
	dynamic_cast<CUI_Crosshair*>(m_pWrap[Crosshair])->Set_Crosshair(eClass);
	dynamic_cast<CUI_Skill*>(m_pWrap[Skill])->Set_SkillHUD(eClass);
}

void CUI_HUD::Set_ActiveHeroPort(_bool value)
{
	CUI_Portrait::HeroPortAnimType eType;

	if (value == true)
	{
		eType = CUI_Portrait::Enable;
	}
	else
	{
		eType = CUI_Portrait::Disable;
	}

	dynamic_cast<CUI_Portrait*>(m_pWrap[Port])->Set_HeroPort(eType);
}
