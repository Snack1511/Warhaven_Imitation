#include "CUI_HUD.h"
#include "GameInstance.h"
#include "Texture.h"
#include "CButton.h"
#include "CFader.h"

#include "CUnit.h"
#include "CUI_Object.h"
#include "CUI_CrossHair.h"
#include "CUI_Portrait.h"
#include "CUI_Skill.h"
#include "CUI_HeroGauge.h"
#include "CUI_HpBar.h"

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
	m_pWrap[HpBar] = CUI_HpBar::Create();

	CREATE_GAMEOBJECT(m_pWrap[Crosshair], GROUP_UI);
	CREATE_GAMEOBJECT(m_pWrap[Port], GROUP_UI);
	CREATE_GAMEOBJECT(m_pWrap[Skill], GROUP_UI);
	CREATE_GAMEOBJECT(m_pWrap[HeroGauge], GROUP_UI);
	CREATE_GAMEOBJECT(m_pWrap[HpBar], GROUP_UI);

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

	dynamic_cast<CUI_Crosshair*>(m_pWrap[Crosshair])->Set_Crosshair(m_eCurClass);
	dynamic_cast<CUI_Portrait*>(m_pWrap[Port])->Start_Portrait(m_eCurClass);
	dynamic_cast<CUI_Skill*>(m_pWrap[Skill])->Set_SkillHUD(m_eCurClass);
	dynamic_cast<CUI_HeroGauge*>(m_pWrap[HeroGauge])->Start_HeroGauge();
	dynamic_cast<CUI_HpBar*>(m_pWrap[HpBar])->SetActive_HpBar(true);

	Bind_Btn();

	Set_FadePortHighlight();

	return S_OK;
}

void CUI_HUD::My_Tick()
{
	__super::My_Tick();

	if (m_pWrap[HpBar]->Is_Valid())
	{
		m_fHpRatio = m_tStatus.fHP / m_tStatus.fMaxHP;
		dynamic_cast<CUI_HpBar*>(m_pWrap[HpBar])->Set_HpRatio(m_fHpRatio);

		if (KEY(Z, HOLD))
		{
			m_tStatus.fHP -= fDT(0) * 10.f;
			if (m_tStatus.fHP <= 0)
			{
				m_tStatus.fHP = 0;
			}
		}

		if (KEY(X, HOLD))
		{
			m_tStatus.fHP += fDT(0) * 10.f;
			if (m_tStatus.fHP >= m_tStatus.fMaxHP)
			{
				m_tStatus.fHP = m_tStatus.fMaxHP;
			}
		}
	}

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

	if (m_pBG->Is_Valid())
	{
		if (KEY(F, TAP))
		{
			SetActive_CharacterSelectWindow(false);
		}
	}
	else
	{
		if (KEY(T, TAP))
		{
			SetActive_CharacterSelectWindow(true);
		}
	}
}

void CUI_HUD::On_PointEnter_Port(const _uint& iEventNum)
{
	CUI_Object* pTarget = m_pPortClone[iEventNum];
	if (pTarget)
	{
		Enable_Fade(m_pPortHighlights[iEventNum]);
	}
}

void CUI_HUD::On_PointExit_Port(const _uint& iEventNum)
{
	Disable_Fade(m_pPortHighlights[iEventNum]);
}

void CUI_HUD::On_PointDown_Port(const _uint& iEventNum)
{
	CUI_Object* pTarget = m_pPortClone[iEventNum];

	// pTarget->Lerp_PosY();

	if (pTarget)
	{
		switch (iEventNum)
		{
		case 0:
			m_pClassInfo->Set_FontText(TEXT("블레이드"));
			GET_COMPONENT_FROM(m_pClassInfoIcon, CTexture)->Set_CurTextureIndex(iEventNum);
			Set_HUD(CUnit::WARRIOR);
			break;

		case 1:
			m_pClassInfo->Set_FontText(TEXT("스파이크"));
			GET_COMPONENT_FROM(m_pClassInfoIcon, CTexture)->Set_CurTextureIndex(iEventNum);
			Set_HUD(CUnit::SPEAR);
			break;

		case 2:
			m_pClassInfo->Set_FontText(TEXT("아치"));
			GET_COMPONENT_FROM(m_pClassInfoIcon, CTexture)->Set_CurTextureIndex(iEventNum);
			Set_HUD(CUnit::ARCHER);
			break;

		case 3:
			m_pClassInfo->Set_FontText(TEXT("가디언"));
			GET_COMPONENT_FROM(m_pClassInfoIcon, CTexture)->Set_CurTextureIndex(iEventNum);
			Set_HUD(CUnit::PALADIN);
			break;

		case 4:
			m_pClassInfo->Set_FontText(TEXT("스모크"));
			GET_COMPONENT_FROM(m_pClassInfoIcon, CTexture)->Set_CurTextureIndex(iEventNum);
			Set_HUD(CUnit::PRIEST);
			break;

		case 5:
			m_pClassInfo->Set_FontText(TEXT("워해머"));
			GET_COMPONENT_FROM(m_pClassInfoIcon, CTexture)->Set_CurTextureIndex(iEventNum);
			Set_HUD(CUnit::ENGINEER);
			break;
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
		m_pPortClone[i]->CallBack_PointEnter += bind(&CUI_HUD::On_PointEnter_Port, this, i);
		m_pPortClone[i]->CallBack_PointExit += bind(&CUI_HUD::On_PointExit_Port, this, i);

		m_pPortClone[i]->CallBack_PointDown += bind(&CUI_HUD::On_PointDown_Port, this, i);
	}
}

void CUI_HUD::Create_CharacterSelectWindow()
{
	m_pBG = CUI_Object::Create();
	m_pBG->Set_Scale(1280.f, 720.f);
	m_pBG->Set_Sort(0.02f);
	m_pBG->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/T_LobbyBG.dds"));

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

	m_pPortHighlight = CUI_Object::Create();
	m_pPortHighlight->Set_Scale(100.f, 144.f);
	m_pPortHighlight->Set_Sort(0.001f);
	m_pPortHighlight->Set_Color(_float4(1.f, 1.f, 1.f, 0.3f));
	m_pPortHighlight->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/CharacterSelect/T_PortraitBustShotBG.png"));

	m_pClassInfo = CUI_Object::Create();
	m_pClassInfo->Set_Scale(75.f, 165.f);
	m_pClassInfo->Set_Pos(-550.f, 200.f);
	m_pClassInfo->Set_Sort(0.01f);
	m_pClassInfo->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/CharacterSelect/T_DecoLam04.dds"));

	m_pClassInfo->Set_FontRender(true);
	m_pClassInfo->Set_FontStyle(true);
	m_pClassInfo->Set_FontScale(0.5f);
	m_pClassInfo->Set_FontOffset(50.f, -50.f);

	m_pClassInfoIcon = CUI_Object::Create();
	m_pClassInfoIcon->Set_Scale(54.f);
	m_pClassInfoIcon->Set_Pos(-550.f, 220.f);
	m_pClassInfoIcon->Set_Sort(0.001f);
	m_pClassInfoIcon->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/CharacterSelect/T_WarriorIconGold.dds"));

	GET_COMPONENT_FROM(m_pClassInfoIcon, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/CharacterSelect/T_SpearmanIconGold.dds"));
	GET_COMPONENT_FROM(m_pClassInfoIcon, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/CharacterSelect/T_ArcherIconGold.dds"));
	GET_COMPONENT_FROM(m_pClassInfoIcon, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/CharacterSelect/T_PaladinIconGold.dds"));
	GET_COMPONENT_FROM(m_pClassInfoIcon, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/CharacterSelect/T_PriestIconGold.dds"));
	GET_COMPONENT_FROM(m_pClassInfoIcon, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/CharacterSelect/T_EngineerIconGold.dds"));

	m_pLine = CUI_Object::Create();
	m_pLine->Set_Scale(650.f, 28.f);
	m_pLine->Set_Pos(0.f, -100.f);
	m_pLine->Set_Sort(0.01f);
	m_pLine->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/CharacterSelect/T_DecoLam02.png"));

	for (int i = 0; i < 6; ++i)
	{
		m_pPortClone[i] = m_pPort->Clone();
		m_pPortBGClone[i] = m_pPortBG->Clone();
		m_pClassIconClone[i] = m_pClassIcon->Clone();
		m_pPortHighlights[i] = m_pPortHighlight->Clone();

		_float fPosX = -300.f + (i * 120.f);

		m_pPortClone[i]->Set_Pos(fPosX, -250.f);
		m_pPortHighlights[i]->Set_Pos(fPosX, -248.f);

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

		CREATE_GAMEOBJECT(m_pPortHighlights[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pPortHighlights[i]);
	}

	CREATE_GAMEOBJECT(m_pBG, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pBG);

	CREATE_GAMEOBJECT(m_pPort, GROUP_UI);
	DELETE_GAMEOBJECT(m_pPort);

	CREATE_GAMEOBJECT(m_pPortBG, GROUP_UI);
	DELETE_GAMEOBJECT(m_pPortBG);

	CREATE_GAMEOBJECT(m_pClassIcon, GROUP_UI);
	DELETE_GAMEOBJECT(m_pClassIcon);

	CREATE_GAMEOBJECT(m_pPortHighlight, GROUP_UI);
	DELETE_GAMEOBJECT(m_pPortHighlight);

	CREATE_GAMEOBJECT(m_pClassInfo, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pClassInfo);

	CREATE_GAMEOBJECT(m_pClassInfoIcon, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pClassInfoIcon);

	CREATE_GAMEOBJECT(m_pLine, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pLine);
}

void CUI_HUD::Set_FadePortHighlight()
{
	FADEDESC tFadeDesc;
	ZeroMemory(&tFadeDesc, sizeof(FADEDESC));

	tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_DISABLE;
	tFadeDesc.eFadeStyle = FADEDESC::FADE_STYLE_DEFAULT;

	tFadeDesc.bFadeInFlag = FADE_NONE;
	tFadeDesc.bFadeOutFlag = FADE_NONE;

	tFadeDesc.fFadeInStartTime = 0.f;
	tFadeDesc.fFadeInTime = 0.3f;

	tFadeDesc.fFadeOutStartTime = 0.f;
	tFadeDesc.fFadeOutTime = 0.3f;

	for (int i = 0; i < 6; ++i)
	{
		GET_COMPONENT_FROM(m_pPortHighlights[i], CFader)->Get_FadeDesc() = tFadeDesc;
	}
}

void CUI_HUD::SetActive_CharacterSelectWindow(_bool value)
{
	Set_ClassInfo(m_eCurClass);

	dynamic_cast<CUI_HpBar*>(m_pWrap[HpBar])->SetActive_HpBar(!value);

	if (value == true)
	{
		ENABLE_GAMEOBJECT(m_pBG);
		ENABLE_GAMEOBJECT(m_pLine);
		ENABLE_GAMEOBJECT(m_pClassInfo);
		ENABLE_GAMEOBJECT(m_pClassInfoIcon);

		for (int i = 0; i < 6; ++i)
		{
			ENABLE_GAMEOBJECT(m_pPortClone[i]);
			ENABLE_GAMEOBJECT(m_pPortBGClone[i]);
			ENABLE_GAMEOBJECT(m_pClassIconClone[i]);
		}
	}
	else
	{
		DISABLE_GAMEOBJECT(m_pBG);
		DISABLE_GAMEOBJECT(m_pLine);
		DISABLE_GAMEOBJECT(m_pClassInfo);
		DISABLE_GAMEOBJECT(m_pClassInfoIcon);

		for (int i = 0; i < 6; ++i)
		{
			DISABLE_GAMEOBJECT(m_pPortClone[i]);
			DISABLE_GAMEOBJECT(m_pPortBGClone[i]);
			DISABLE_GAMEOBJECT(m_pClassIconClone[i]);
			DISABLE_GAMEOBJECT(m_pPortHighlights[i]);
		}
	}
}

void CUI_HUD::Set_ClassInfo(CUnit::CLASS_TYPE eClass)
{
	switch (eClass)
	{
	case Client::CUnit::WARRIOR:
		m_pClassInfo->Set_FontText(TEXT("블레이드"));
		GET_COMPONENT_FROM(m_pClassInfoIcon, CTexture)->Set_CurTextureIndex(0);
		break;
	case Client::CUnit::SPEAR:
		m_pClassInfo->Set_FontText(TEXT("스파이크"));
		GET_COMPONENT_FROM(m_pClassInfoIcon, CTexture)->Set_CurTextureIndex(1);
		break;
	case Client::CUnit::ARCHER:
		m_pClassInfo->Set_FontText(TEXT("아치"));
		GET_COMPONENT_FROM(m_pClassInfoIcon, CTexture)->Set_CurTextureIndex(2);
		break;
	case Client::CUnit::PALADIN:
		m_pClassInfo->Set_FontText(TEXT("가디언"));
		GET_COMPONENT_FROM(m_pClassInfoIcon, CTexture)->Set_CurTextureIndex(3);
		break;
	case Client::CUnit::PRIEST:
		m_pClassInfo->Set_FontText(TEXT("스모크"));
		GET_COMPONENT_FROM(m_pClassInfoIcon, CTexture)->Set_CurTextureIndex(4);
		break;
	case Client::CUnit::ENGINEER:
		m_pClassInfo->Set_FontText(TEXT("워해머"));
		GET_COMPONENT_FROM(m_pClassInfoIcon, CTexture)->Set_CurTextureIndex(5);
		break;
	}
}
