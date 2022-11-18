#include "CUI_HUD.h"
#include "GameInstance.h"
#include "Texture.h"
#include "CButton.h"

#include "CUnit.h"
#include "CUI_Object.h"
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

	CREATE_GAMEOBJECT(m_pWrap[Crosshair], GROUP_UI);
	CREATE_GAMEOBJECT(m_pWrap[Port], GROUP_UI);
	CREATE_GAMEOBJECT(m_pWrap[Skill], GROUP_UI);
	CREATE_GAMEOBJECT(m_pWrap[HeroGauge], GROUP_UI);

	Create_CharacterSelectWindow();

	return S_OK;
}

HRESULT CUI_HUD::Initialize()
{
	return S_OK;
}

HRESULT CUI_HUD::Start()
{
	m_eCurClass = m_tStatus.eClass;

	//dynamic_cast<CUI_Crosshair*>(m_pWrap[Crosshair])->Set_Crosshair(m_eCurClass);
	//dynamic_cast<CUI_Portrait*>(m_pWrap[Port])->Start_Portrait(m_eCurClass);
	//dynamic_cast<CUI_Skill*>(m_pWrap[Skill])->Set_SkillHUD(m_eCurClass);
	//dynamic_cast<CUI_HeroGauge*>(m_pWrap[HeroGauge])->Start_HeroGauge();

	ENABLE_GAMEOBJECT(m_pBG);

	for (int i = 0; i < 6; ++i)
	{
		ENABLE_GAMEOBJECT(m_pPortClone[i]);
		ENABLE_GAMEOBJECT(m_pPortBGClone[i]);
		ENABLE_GAMEOBJECT(m_pClassIconClone[i]);
	}

	Bind_Btn();

	return S_OK;
}

void CUI_HUD::My_Tick()
{
	__super::My_Tick();

	if (m_pWrap[HeroGauge]->Is_Valid())
	{
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
}

void CUI_HUD::On_PointEnter_Port(const _uint& iEventNum)
{
	for (int i = 0; i < 6; ++i)
	{
		CUI_Object* pTarget = GET_COMPONENT_FROM(m_pPortClone[i], CButton)->Get_TargetUI();
		if (pTarget)
		{
			_uint iTextureNum = GET_COMPONENT_FROM(pTarget, CTexture)->Get_CurTextureIndex();
			
			switch (iTextureNum)
			{
			case 0:
				Call_MsgBox(TEXT("워리어"));
				break;

			case 1:
				Call_MsgBox(TEXT("스피어"));
				break;

			case 2:
				Call_MsgBox(TEXT("아처"));
				break;

			case 3:
				Call_MsgBox(TEXT("팔라딘"));
				break;

			case 4:
				Call_MsgBox(TEXT("프리스트"));
				break;

			case 5:
				Call_MsgBox(TEXT("엔지니어"));
				break;
			}

			//_float4 vPos = pTarget->Get_Pos();
			//m_pPortHighlight->Set_Pos(vPos);

			//ENABLE_GAMEOBJECT(m_pPortHighlight);
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

void CUI_HUD::Bind_Btn()
{
	for (int i = 0; i < 6; ++i)
	{
		m_pPortClone[i]->CallBack_PointEnter += bind(&CUI_HUD::On_PointEnter_Port, this, placeholders::_1);
	}
}

void CUI_HUD::Create_CharacterSelectWindow()
{
	// 백그라운드
	m_pBG = CUI_Object::Create();
	m_pBG->Set_Scale(1280.f, 720.f);
	m_pBG->Set_Sort(0.02f);
	m_pBG->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/T_LobbyBG.dds"));

	// 포트레이트
	m_pPort = CUI_Object::Create();
	m_pPort->Set_Scale(100.f, 140.f);
	m_pPort->Set_Sort(0.01f);
	m_pPort->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/CharacterSelect/T_Portrait2Warrior.dds"));

	m_pPort->Set_MouseTarget(true);

	GET_COMPONENT_FROM(m_pPort, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/CharacterSelect/T_Portrait2Spearman.dds"));
	GET_COMPONENT_FROM(m_pPort, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/CharacterSelect/T_Portrait2Archer.dds"));
	GET_COMPONENT_FROM(m_pPort, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/CharacterSelect/T_Portrait2Paladin.dds"));
	GET_COMPONENT_FROM(m_pPort, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/CharacterSelect/T_Portrait2Priest.dds"));
	GET_COMPONENT_FROM(m_pPort, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/CharacterSelect/T_Portrait2Engineer.dds"));

	m_pPortBG = CUI_Object::Create();
	m_pPortBG->Set_Scale(100.f, 140.f);
	m_pPortBG->Set_Sort(0.015f);
	m_pPortBG->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/CharacterSelect/T_CharacterBG.dds"));

	m_pClassIcon = CUI_Object::Create();
	m_pClassIcon->Set_Scale(20.f);
	m_pClassIcon->Set_Sort(0.01f);
	m_pClassIcon->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/CharacterSelect/T_WarriorIconGold.dds"));

	GET_COMPONENT_FROM(m_pClassIcon, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/CharacterSelect/T_SpearmanIconGold.dds"));
	GET_COMPONENT_FROM(m_pClassIcon, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/CharacterSelect/T_ArcherIconGold.dds"));
	GET_COMPONENT_FROM(m_pClassIcon, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/CharacterSelect/T_PaladinIconGold.dds"));
	GET_COMPONENT_FROM(m_pClassIcon, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/CharacterSelect/T_PriestIconGold.dds"));
	GET_COMPONENT_FROM(m_pClassIcon, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/CharacterSelect/T_EngineerIconGold.dds"));

	for (int i = 0; i < 6; ++i)
	{
		m_pPortClone[i] = m_pPort->Clone();
		m_pPortBGClone[i] = m_pPortBG->Clone();
		m_pClassIconClone[i] = m_pClassIcon->Clone();

		_float fPosX = -275.f + (i * 110.f);

		m_pPortClone[i]->Set_Pos(fPosX, -250.f);

		_float4 vPos = m_pPortClone[i]->Get_Pos();

		m_pPortBGClone[i]->Set_Pos(vPos);
		m_pClassIconClone[i]->Set_Pos(vPos.x + 35.f, vPos.y + 55.f);

		GET_COMPONENT_FROM(m_pPortClone[i], CTexture)->Set_CurTextureIndex(i);
		GET_COMPONENT_FROM(m_pClassIconClone[i], CTexture)->Set_CurTextureIndex(i);

		CREATE_GAMEOBJECT(m_pPortClone[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pPortClone[i]);

		CREATE_GAMEOBJECT(m_pPortBGClone[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pPortBGClone[i]);

		CREATE_GAMEOBJECT(m_pClassIconClone[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pClassIconClone[i]);
	}

	CREATE_GAMEOBJECT(m_pBG, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pBG);

	CREATE_GAMEOBJECT(m_pPort, GROUP_UI);
	DELETE_GAMEOBJECT(m_pPort);

	CREATE_GAMEOBJECT(m_pPortBG, GROUP_UI);
	DELETE_GAMEOBJECT(m_pPortBG);

	CREATE_GAMEOBJECT(m_pClassIcon, GROUP_UI);
	DELETE_GAMEOBJECT(m_pClassIcon);
}
