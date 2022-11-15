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

	// ����� ������ 
	m_fHeroGauge = m_tStatus.fHeroGague / m_tStatus.fMaxHeroGauge;
	dynamic_cast<CUI_HeroGauge*>(m_pWrap[HeroGauge])->Set_HeroValue(m_fHeroGauge);

	// ���� ���� ���� ���°� �ƴ� ��
	if (!m_tStatus.bAbleHero)
	{
		// ���� �����̸�
		if(m_tStatus.bIsHero)
		{
			// ������ ����
			m_tStatus.fHeroGague += fDT(0) * 20.f;
			// �������� 0�̵Ǹ�
			if (m_fHeroGauge >= 1.f)
			{
				m_fHeroGauge = 1.f;
				// ���� ���� ����
				m_tStatus.bIsHero = false;
			}
			else if (KEY(NUM1, TAP))
			{
				m_tStatus.bIsHero = false;
			}
		}
		// ���� ���°� �ƴϸ�
		else
		{
			// ������ ����
			m_tStatus.fHeroGague -= fDT(0) * 20.f;
			// �������� �ִ밡 �Ǹ�
			if (m_fHeroGauge <= 0.f)
			{
				m_fHeroGauge = 0.f;
				// ���� ���� ���� ����
				m_tStatus.bAbleHero = true;
			}
		}
	}
	// ���� ���� ���� ������ ��
	else
	{
		// ���� ���� Ű�� ������ ���� ����
		if (KEY(NUM1, TAP) || KEY(NUM2, TAP) || KEY(NUM3, TAP) || KEY(NUM4, TAP))
		{
			// ���� ���� ���� ��Ȱ��ȭ
			m_tStatus.bAbleHero = false;

			// ���� ���� Ȱ��ȭ
			m_tStatus.bIsHero = true;
		}
	}
}

void CUI_HUD::Change_HUD()
{
	if (!m_tStatus.bIsHero)
	{
		if (KEY(T, TAP))
		{
			// ���� ������ �����̷� �ϰ� �ʹ�
			// ���� �ϳ��� �����ϴ°� �ƴ϶�

			_uint iIndex = m_eCurClass;
			iIndex++;
			if (iIndex > CUnit::CLASS_ENGINEER)
			{
				iIndex = CUnit::CLASS_WARRIOR;
			}

			m_eCurClass = (CUnit::CLASS_TYPE)iIndex;
			Set_HUD(m_eCurClass);
		}
		else if (KEY(NUM1, TAP))
		{
			Set_HUD(CUnit::CLASS_FIONA);
		}
		else if (KEY(NUM2, TAP))
		{
			Set_HUD(CUnit::CLASS_QANDA);
		}
		else if (KEY(NUM3, TAP))
		{
			Set_HUD(CUnit::CLASS_HOEDT);
		}
		else if (KEY(NUM4, TAP))
		{
			Set_HUD(CUnit::CLASS_LANCER);
		}
	}
	else
	{
		if (KEY(NUM1, TAP))
		{
			Set_HUD(m_ePrvClass);
		}
	}
}

void CUI_HUD::Set_HUD(CUnit::CLASS_TYPE eClass)
{
	m_ePrvClass = m_eCurClass;
	m_eCurClass = eClass;

	if (!m_tStatus.bIsHero)
	{
		if (eClass > CUnit::CLASS_ENGINEER)
		{
			m_tStatus.bIsHero = true;

			dynamic_cast<CUI_Portrait*>(m_pWrap[Port])->Set_HeroPort(false);
		}
	}
	else
	{
		if (eClass < CUnit::CLASS_FIONA)
		{
			m_tStatus.bIsHero = false;
		}
	}

	Set_Portrait(eClass);
	Set_Crosshair(eClass);
	Set_SkillHUD(eClass);
}

void CUI_HUD::Set_Portrait(CUnit::CLASS_TYPE eClass)
{
	dynamic_cast<CUI_Portrait*>(m_pWrap[Port])->Set_Portrait(eClass);
}

void CUI_HUD::Set_Crosshair(CUnit::CLASS_TYPE eClass)
{
	dynamic_cast<CUI_Crosshair*>(m_pWrap[Crosshair])->Set_Crosshair(eClass);
}

void CUI_HUD::Set_SkillHUD(CUnit::CLASS_TYPE eClass)
{
	dynamic_cast<CUI_Skill*>(m_pWrap[Skill])->Set_SkillHUD(eClass);
}

void CUI_HUD::Set_ActiveHeroPort(_bool value)
{
	dynamic_cast<CUI_Portrait*>(m_pWrap[Port])->Set_HeroPort(value);
}
