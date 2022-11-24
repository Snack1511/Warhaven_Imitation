#include "CUI_HUD.h"
#include "GameInstance.h"
#include "Texture.h"
#include "CButton.h"
#include "CFader.h"

#include "CUser.h"
#include "CUnit.h"
#include "CUI_Object.h"
#include "CUI_CrossHair.h"
#include "CUI_Portrait.h"
#include "CUI_Skill.h"
#include "CUI_HeroGauge.h"
#include "CUI_HpBar.h"
#include "CUI_Training.h"

#include "Easing_Utillity.h"

#include"CPlayer.h"


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
	Create_PortUnderLine();
	Create_TraingText();
	Create_HeroGaugeText();
	Create_OxenJumpText();

	return S_OK;
}

HRESULT CUI_HUD::Initialize()
{
	return S_OK;
}

HRESULT CUI_HUD::Start()
{
	m_tStatus = CUser::Get_Instance()->Get_Player()->Get_Status();
	m_eCurClass = m_tStatus.eClass;

	dynamic_cast<CUI_Crosshair*>(m_pWrap[Crosshair])->Set_Crosshair(m_eCurClass);
	dynamic_cast<CUI_Portrait*>(m_pWrap[Port])->Start_Portrait(m_eCurClass);
	dynamic_cast<CUI_Skill*>(m_pWrap[Skill])->Set_SkillHUD(m_eCurClass);
	dynamic_cast<CUI_HeroGauge*>(m_pWrap[HeroGauge])->Start_HeroGauge();
	dynamic_cast<CUI_HpBar*>(m_pWrap[HpBar])->SetActive_HpBar(true);

	ENABLE_GAMEOBJECT(m_pChangeClassText);
	ENABLE_GAMEOBJECT(m_pHeroGaugeText);

	Bind_Btn();

	Set_FadePortHighlight();

	return S_OK;
}

void CUI_HUD::My_Tick()
{
	__super::My_Tick();

	m_tStatus = CUser::Get_Instance()->Get_Player()->Get_Status();

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
		//m_fHeroGauge = -1.f * (m_tStatus.fHeroGague / m_tStatus.fMaxHeroGauge) + 1.f;
		//m_fHeroGauge = m_tStatus.fMaxHeroGauge / m_tStatus.fHeroGague -1.f;

		_tchar  szTemp[MAX_STR] = {};
		swprintf_s(szTemp, TEXT("%.f"), m_fHeroGauge * 100.f);
		m_pHeroGaugeText->Set_FontText(szTemp);

		dynamic_cast<CUI_HeroGauge*>(m_pWrap[HeroGauge])->Set_HeroValue(m_fHeroGauge);

		if (!m_tStatus.bAbleHero)
		{
			if (!m_tStatus.bIsHero)
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
				DISABLE_GAMEOBJECT(m_pInactiveHeroText);

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

	for (int i = 0; i < 6; ++i)
	{
		_float4 vScale = m_pPortUnderLines[i]->Get_Scale();
		if (vScale.x <= 2.f)
		{
			DISABLE_GAMEOBJECT(m_pPortUnderLines[i]);
		}
	}
}

void CUI_HUD::On_PointEnter_Port(const _uint& iEventNum)
{
	CUI_Object* pTarget = m_pPortClone[iEventNum];
	if (pTarget)
	{
		Enable_Fade(m_pPortHighlights[iEventNum], 0.3f);
	}
}

void CUI_HUD::On_PointExit_Port(const _uint& iEventNum)
{
	Disable_Fade(m_pPortHighlights[iEventNum], 0.3f);
}

void CUI_HUD::On_PointDown_Port(const _uint& iEventNum)
{
	CUI_Object* pTarget = m_pPortClone[iEventNum];

	_float fDuraition = 0.15f;

	for (int i = 0; i < 6; ++i)
	{
		_float fPosY = m_pPortClone[i]->Get_PosY();
		if (fPosY > -245.f)
		{
			m_pPortClone[i]->DoMoveY(-10.f, fDuraition);
			m_pPortBGClone[i]->DoMoveY(-10.f, fDuraition);
			m_pClassIconClone[i]->DoMoveY(-10.f, fDuraition);
			m_pPortHighlights[i]->DoMoveY(-10.f, fDuraition);

			m_pPortUnderLines[i]->Lerp_ScaleX(100.f, 2.f, fDuraition);
		}
	}

	m_pPortClone[iEventNum]->DoMoveY(10.f, fDuraition);
	m_pPortBGClone[iEventNum]->DoMoveY(10.f, fDuraition);
	m_pClassIconClone[iEventNum]->DoMoveY(10.f, fDuraition);
	m_pPortHighlights[iEventNum]->DoMoveY(10.f, fDuraition);

	ENABLE_GAMEOBJECT(m_pPortUnderLines[iEventNum]);
	m_pPortUnderLines[iEventNum]->Lerp_ScaleX(2.f, 100.f, fDuraition);

	CPlayer* pPlayer = CUser::Get_Instance()->Get_PlayerObejects();


	if (pTarget)
	{
		switch (iEventNum)
		{
		case 0:
			m_pClassInfo->Set_FontText(TEXT("블레이드"));
			GET_COMPONENT_FROM(m_pClassInfoIcon, CTexture)->Set_CurTextureIndex(iEventNum);
			pPlayer->Change_DefaultUnit(CPlayer::CLASS_DEFAULT_WARRIOR);
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
			pPlayer->Change_DefaultUnit(CPlayer::CLASS_DEFAULT_ENGINEER);
			Set_HUD(CUnit::ENGINEER);
			break;
		}
	}
}

void CUI_HUD::SetActive_OxenJumpText(_bool value)
{
	if (value == true)
	{
		ENABLE_GAMEOBJECT(m_pOxenJumpText);
	}
	else
	{
		DISABLE_GAMEOBJECT(m_pOxenJumpText);
	}
}

void CUI_HUD::Set_SkillCoolTime(_uint iSkillType, _float fCoolTime, _float fMaxCoolTime)
{
	dynamic_cast<CUI_Skill*>(m_pWrap[Skill])->Set_CoolTime(iSkillType, fCoolTime, fMaxCoolTime);
}

void CUI_HUD::Set_HUD(CUnit::CLASS_TYPE eClass)
{
	m_ePrvClass = m_eCurClass;
	m_eCurClass = eClass;

	if (eClass > CUnit::ENGINEER)
	{
		if (m_tStatus.bAbleHero)
		{
			Enable_Fade(m_pInactiveHeroText, 1.f);
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

	GET_COMPONENT_FROM(m_pInactiveHeroText, CFader)->Get_FadeDesc() = tFadeDesc;
}

void CUI_HUD::SetActive_CharacterSelectWindow(_bool value)
{
	Set_ClassInfo(m_eCurClass);

	m_pPortClone[m_eCurClass]->Set_PosY(-240.f);
	m_pPortBGClone[m_eCurClass]->Set_PosY(-240.f);
	m_pClassIconClone[m_eCurClass]->Set_PosY(-240.f);
	m_pPortHighlights[m_eCurClass]->Set_PosY(-240.f);
	m_pPortUnderLines[m_eCurClass]->Set_ScaleX(100.f);

	Enable_Fade(m_pPortUnderLines[m_eCurClass], 0.3f);

	dynamic_cast<CUI_HpBar*>(m_pWrap[HpBar])->SetActive_HpBar(!value);

	if (value == true)
	{
		CUser::Get_Instance()->Set_FixCursor(false); // 픽창 마우스 고정 해제

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
		CUser::Get_Instance()->Set_FixCursor(true); //픽 후 마우스 고정

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
			DISABLE_GAMEOBJECT(m_pPortUnderLines[i]);
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

void CUI_HUD::Create_PortUnderLine()
{
	m_pPortUnderLine = CUI_Object::Create();

	m_pPortUnderLine->Set_PosY(-318.f);
	m_pPortUnderLine->Set_Scale(2.f);
	m_pPortUnderLine->Set_Color(_float4(0.773f, 0.714f, 0.596f, 1.f));

	for (int i = 0; i < 6; ++i)
	{
		m_pPortUnderLines[i] = m_pPortUnderLine->Clone();

		_float fPosX = -300.f + (i * 120.f);
		m_pPortUnderLines[i]->Set_PosX(fPosX);

		CREATE_GAMEOBJECT(m_pPortUnderLines[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pPortUnderLines[i]);
	}

	CREATE_GAMEOBJECT(m_pPortUnderLine, GROUP_UI);
	DELETE_GAMEOBJECT(m_pPortUnderLine);
}

void CUI_HUD::Create_TraingText()
{
	m_pChangeClassText = CUI_Object::Create();

	m_pChangeClassText->Set_Scale(20.f);
	m_pChangeClassText->Set_Pos(-25.f, -250.f);
	m_pChangeClassText->Set_Sort(0.85f);

	m_pChangeClassText->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KeyIcon/Keyboard/T_WhiteTKeyIcon.dds"));

	m_pChangeClassText->Set_FontRender(true);
	m_pChangeClassText->Set_FontStyle(true);
	m_pChangeClassText->Set_FontScale(0.25f);
	m_pChangeClassText->Set_FontOffset(10.f, -13.f);

	m_pChangeClassText->Set_FontText(TEXT("로 전투원 변경 가능"));

	m_pInactiveHeroText = CUI_Object::Create();

	m_pInactiveHeroText->Set_Scale(20.f);
	m_pInactiveHeroText->Set_Pos(450.f, -195.f);
	m_pInactiveHeroText->Set_Sort(0.85f);

	m_pInactiveHeroText->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KeyIcon/Keyboard/T_WhiteNum1KeyIcon.dds"));

	m_pInactiveHeroText->Set_FontRender(true);
	m_pInactiveHeroText->Set_FontStyle(true);
	m_pInactiveHeroText->Set_FontScale(0.25f);
	m_pInactiveHeroText->Set_FontOffset(10.f, -13.f);

	m_pInactiveHeroText->Set_FontText(TEXT("화신 해제"));

	CREATE_GAMEOBJECT(m_pChangeClassText, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pChangeClassText);

	CREATE_GAMEOBJECT(m_pInactiveHeroText, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pInactiveHeroText);
}

void CUI_HUD::Create_HeroGaugeText()
{
	m_pHeroGaugeText = CUI_Object::Create();

	m_pHeroGaugeText->Set_Scale(20.f);
	m_pHeroGaugeText->Set_Pos(550.f, -195.f);
	m_pHeroGaugeText->Set_Sort(0.85f);

	GET_COMPONENT_FROM(m_pHeroGaugeText, CTexture)->Remove_Texture(0);

	m_pHeroGaugeText->Set_FontRender(true);
	m_pHeroGaugeText->Set_FontStyle(true);
	m_pHeroGaugeText->Set_FontScale(0.25f);
	m_pHeroGaugeText->Set_FontOffset(-18.f, -13.f);

	CREATE_GAMEOBJECT(m_pHeroGaugeText, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pHeroGaugeText);
}

void CUI_HUD::Create_OxenJumpText()
{
	m_pOxenJumpText = CUI_Object::Create();

	m_pOxenJumpText->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KeyIcon/Keyboard/T_WhiteSpaceKeyIcon.png"));

	m_pOxenJumpText->Set_PosY(-50.f);
	m_pOxenJumpText->Set_Scale(20.f);

	m_pOxenJumpText->Set_FontRender(true);
	m_pOxenJumpText->Set_FontStyle(true);

	m_pOxenJumpText->Set_FontOffset(13.f, -13.f);
	m_pOxenJumpText->Set_FontScale(0.25f);

	m_pOxenJumpText->Set_FontText(TEXT("점프"));

	CREATE_GAMEOBJECT(m_pOxenJumpText, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pOxenJumpText);
}
