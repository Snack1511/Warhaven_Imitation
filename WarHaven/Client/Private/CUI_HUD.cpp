#include "CUI_HUD.h"
#include "GameInstance.h"
#include "Texture.h"
#include "CButton.h"
#include "CFader.h"
#include "CShader.h"

#include "CUser.h"
#include "CUnit.h"
#include "CUI_Object.h"
#include "CUI_CrossHair.h"
#include "CUI_Portrait.h"
#include "CUI_Skill.h"
#include "CUI_HeroGauge.h"
#include "CUI_HpBar.h"
#include "CUI_Training.h"
#include "CUI_Dead.h"
#include "CUI_Black.h"
#include "Easing_Utillity.h"
#include "Functor.h"

#include "CPlayerInfo.h"
#include "CPlayer.h"
#include "CUI_Cursor.h"
#include "Loading_Manager.h"
#include "CUI_Renderer.h"

#include "CUI_CharacterWindow.h"
#include "CUI_EscMenu.h"

#include "CGameSystem.h"
#include "CSquad.h"



CUI_HUD::CUI_HUD()
{
}

CUI_HUD::~CUI_HUD()
{
}

HRESULT CUI_HUD::Initialize_Prototype()
{
	m_eLoadLevel = CLoading_Manager::Get_Instance()->Get_LoadLevel();

	Create_HUD();

	Create_PlayerNameText();

	Create_InactiveHeroText();
	Create_OxenJumpText();
	Create_HeroTransformUI();
	Create_EscMenu();

	if (m_eLoadLevel <= LEVEL_TYPE_CLIENT::LEVEL_BOOTCAMP)
	{
		Create_CharacterWindow();
	}
	else
	{
		Create_RevivalUI();
		Create_SquardInfo();
	}

	return S_OK;
}

HRESULT CUI_HUD::Start()
{
	Bind_Shader();

	SetActive_HUD(true);

	if (m_eLoadLevel <= LEVEL_BOOTCAMP)
	{
		if (m_pClassChangeText)
			m_pClassChangeText->SetActive(true);
	}

	__super::Start();

	return S_OK;
}

void CUI_HUD::My_Tick()
{
	__super::My_Tick();

	m_tStatus = CUser::Get_Instance()->Get_Player()->Get_Status();

	Update_HeorTransformGauge();

	Active_CharacterWindow();

	if (KEY(ESC, TAP))
	{
		Play_Sound(L"UI_Btn_Down_Main");

		_bool bEscActive = m_pUI_EscMenu->Is_Valid();
		if (!bEscActive)
		{
			m_pUI_EscMenu->SetActive(true);
		}
		else
		{
			m_pUI_EscMenu->SetActive(false);
		}
	}

	m_bActivationKDA = KEY(TAB, HOLD) ? true : false;
	CUser::Get_Instance()->SetActive_ScoreBoard(m_bActivationKDA);
}

CUI_Wrapper* CUI_HUD::Get_HUD(_uint eHUD)
{
	return m_pHUD[eHUD];
}

void CUI_HUD::SetActive_HUD(_bool value)
{
	for (int i = 0; i < HUD_End; ++i)
	{
		if (value == true)
		{
			ENABLE_GAMEOBJECT(m_pHUD[i]);
		}
		else
		{
			DISABLE_GAMEOBJECT(m_pHUD[i]);
		}
	}

	// CUser::Get_Instance()->Turn_HeroGaugeFire(value);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < Squard_End; ++j)
		{
			if (m_pArrSquardInfo[i][j])
				m_pArrSquardInfo[i][j]->SetActive(value);
		}
	}
}

void CUI_HUD::Create_HUD()
{
	m_pHUD[HUD_Crosshair] = CUI_Crosshair::Create();
	m_pHUD[HUD_Port] = CUI_Portrait::Create();
	m_pHUD[HUD_HP] = CUI_HpBar::Create();
	m_pHUD[HUD_HeroGauge] = CUI_HeroGauge::Create();
	m_pHUD[HUD_Skill] = CUI_Skill::Create();

	for (int i = 0; i < HUD_End; ++i)
	{
		CREATE_GAMEOBJECT(m_pHUD[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pHUD[i]);
	}
}

void CUI_HUD::Create_CharacterWindow()
{
	m_pUI_CharacterWindow = CUI_CharacterWindow::Create();

	CREATE_GAMEOBJECT(m_pUI_CharacterWindow, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pUI_CharacterWindow);

	if (m_pUI_CharacterWindow)
	{
		Create_ClassChangeText();
	}
}

void CUI_HUD::Active_CharacterWindow()
{
	if (m_pUI_CharacterWindow)
	{

		if (KEY(F, TAP) && !m_pHUD[0]->Is_Valid())
		{
			m_pUI_CharacterWindow->SetActive_CharacterWindow(false);
			SetActive_HUD(true);

			CPlayer* pPlayer = CUser::Get_Instance()->Get_PlayerObejects();
			pPlayer->Change_UnitClass(m_eCurClass);
			pPlayer->Get_CurrentUnit()->On_Respawn();

			if (CUser::Get_Instance()->Get_Player()->Get_OwnerPlayer()->AbleHero())
			{
				CUser::Get_Instance()->Turn_HeroGaugeFire(true);
			}

			Set_HUD(m_eCurClass);
		}

		else if (KEY(T, TAP) && KEY(CTRL, HOLD))
		{
			CUser::Get_Instance()->Turn_HeroGaugeFire(false);
			SetActive_HUD(false);
			m_pUI_CharacterWindow->SetActive_CharacterWindow(true);
		}

	}
}

void CUI_HUD::Create_EscMenu()
{
	m_pUI_EscMenu = CUI_EscMenu::Create();

	CREATE_GAMEOBJECT(m_pUI_EscMenu, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pUI_EscMenu);
}

void CUI_HUD::Set_Shader_HeroTransformGauge(CShader* pShader, const char* pConstName)
{
	/*_bool bFlip = true;
	pShader->Set_RawValue("g_bFlip", &bFlip, sizeof(_bool));*/
	pShader->Set_RawValue("g_fValue", &m_fHeroTransformGaugeRatio, sizeof(_float));
}

void CUI_HUD::SetActive_OxenJumpText(_bool value)
{
	m_pOxenJumpText->SetActive(value);
}

void CUI_HUD::SetActive_HeroTransformGauge(_bool value)
{
	for (int i = 0; i < HT_End; ++i)
	{
		if (value == true)
		{
			ENABLE_GAMEOBJECT(m_pHeroTransformUI[i]);
		}
		else
		{
			DISABLE_GAMEOBJECT(m_pHeroTransformUI[i]);
		}
	}
}

void CUI_HUD::SetActive_SquardInfo(_bool value)
{
	if (m_eLoadLevel < LEVEL_PADEN)
		return;

	map<_hashcode, CPlayer*> mapPlayers = PLAYER->Get_OwnerPlayer()->Get_Squad()->Get_AllPlayers();

	auto iter = mapPlayers.begin();

	vector<CPlayer*> vecPlayer;
	for (auto& iter : mapPlayers)
	{
		vecPlayer.push_back(iter.second);
	}

	for (int i = 1; i < vecPlayer.size(); ++i)
	{
		_uint iTextureNum = vecPlayer[i]->Get_PlayerInfo()->Get_ChonsenClass();
		wstring wstrPlayerName = vecPlayer[i]->Get_PlayerName();

		_uint iIdx = (i - 1);

		m_pArrSquardInfo[iIdx][Squard_Port]->Set_TextureIndex(iTextureNum);
		m_pArrSquardInfo[iIdx][Squard_Num]->Set_TextureIndex(i);
		m_pArrSquardInfo[iIdx][Squard_Num]->Set_FontText(wstrPlayerName);

		for (int j = 0; j < Squard_End; ++j)
		{
			_float fPosY = -165.f - (iIdx * 35.f);
			m_pArrSquardInfo[iIdx][j]->Set_PosY(fPosY);

			m_pArrSquardInfo[iIdx][j]->SetActive(value);
		}
	}
}

void CUI_HUD::SetActive_RevivalUI(_bool value)
{
	m_pRevivalUI->SetActive(value);
}

void CUI_HUD::Set_Crosshair_Pos(_float4 vPosition)
{
	static_cast<CUI_Crosshair*>(m_pHUD[HUD_Crosshair])->Set_Position(vPosition);
}

_bool CUI_HUD::Is_OnHeroGauge()
{
	return m_pPlayerNameText->Is_Valid();
}

void CUI_HUD::Set_HUD(CLASS_TYPE eClass)
{
	m_ePrvClass = m_eCurClass;
	m_eCurClass = eClass;

	if (eClass > ENGINEER)
	{
		if (CUser::Get_Instance()->Get_Player()->Get_OwnerPlayer()->AbleHero())
		{
			Enable_Fade(m_pInactiveHeroText, 1.2f);
			dynamic_cast<CUI_Portrait*>(m_pHUD[HUD_Port])->Set_HeroPort(1);
		}

		CUser::Get_Instance()->Get_Player()->Get_OwnerPlayer()->AbleHero() = false;
		//CUser::Get_Instance()->Get_Player()->Get_OwnerPlayer()->IsHero() = true; ?????? ????????????
	}
	else
	{
		Disable_Fade(m_pInactiveHeroText, 1.2f);
		//CUser::Get_Instance()->Get_Player()->Get_OwnerPlayer()->IsHero() = false;
	}

	dynamic_cast<CUI_Crosshair*>(m_pHUD[HUD_Crosshair])->Set_Crosshair(m_eCurClass);
	dynamic_cast<CUI_Portrait*>(m_pHUD[HUD_Port])->Set_UserPort(m_eCurClass);
	dynamic_cast<CUI_Skill*>(m_pHUD[HUD_Skill])->Set_SkillUI(m_eCurClass);
}

void CUI_HUD::Create_SquardInfo()
{
	for (int i = 0; i < Squard_End; ++i)
	{
		m_pSquardInfo[i] = CUI_Object::Create();

		m_pSquardInfo[i]->Set_PosX(-595.f);

		switch (i)
		{
		case Squard_BG:

			m_pSquardInfo[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Portrait/T_RoundPortraitBGSmall.dds"));

			m_pSquardInfo[i]->Set_Scale(32.f);
			m_pSquardInfo[i]->Set_Sort(0.5f);

			break;

		case Squard_Port:

			GET_COMPONENT_FROM(m_pSquardInfo[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pSquardInfo[i], "/HUD/Portrait", "Class");

			m_pSquardInfo[i]->Set_Scale(32.f);
			m_pSquardInfo[i]->Set_Sort(0.5f);

			break;

		case Squard_Num:

			GET_COMPONENT_FROM(m_pSquardInfo[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pSquardInfo[i], "/Oper", "Num");

			m_pSquardInfo[i]->Set_PosX(-565.f);
			m_pSquardInfo[i]->Set_Scale(16.f);
			m_pSquardInfo[i]->Set_Sort(0.5f);

			m_pSquardInfo[i]->Set_FontRender(true);
			m_pSquardInfo[i]->Set_FontStyle(true);

			m_pSquardInfo[i]->Set_FontOffset(5.f, -12.f);
			m_pSquardInfo[i]->Set_FontScale(0.2f);

			break;
		}

		CREATE_GAMEOBJECT(m_pSquardInfo[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pSquardInfo[i]);

		for (int j = 0; j < 3; ++j)
		{
			m_pArrSquardInfo[j][i] = m_pSquardInfo[i]->Clone();

			CREATE_GAMEOBJECT(m_pArrSquardInfo[j][i], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrSquardInfo[j][i]);
		}
	}
}

void CUI_HUD::Create_ClassChangeText()
{
	m_pClassChangeText = CUI_Object::Create();

	m_pClassChangeText->Set_Scale(20.f);
	m_pClassChangeText->Set_Pos(-85.f, -250.f);
	m_pClassChangeText->Set_Sort(0.85f);

	m_pClassChangeText->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KeyIcon/Keyboard/White/T_WhiteTKeyIcon.dds"));

	m_pClassChangeText->Set_FontRender(true);
	m_pClassChangeText->Set_FontStyle(true);
	m_pClassChangeText->Set_FontCenter(true);
	m_pClassChangeText->Set_FontScale(0.25f);

	m_pClassChangeText->Set_FontOffset(85.f, 5.f);

	m_pClassChangeText->Set_FontText(TEXT("?? ?????? ???? ????"));

	CREATE_GAMEOBJECT(m_pClassChangeText, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pClassChangeText);
}

void CUI_HUD::Create_OxenJumpText()
{
	m_pOxenJumpText = CUI_Object::Create();

	m_pOxenJumpText->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KeyIcon/Keyboard/White/T_WhiteSpaceKeyIcon.png"));

	m_pOxenJumpText->Set_PosY(-50.f);
	m_pOxenJumpText->Set_Scale(20.f);

	m_pOxenJumpText->Set_FontRender(true);
	m_pOxenJumpText->Set_FontStyle(true);

	m_pOxenJumpText->Set_FontOffset(13.f, -13.f);
	m_pOxenJumpText->Set_FontScale(0.25f);

	m_pOxenJumpText->Set_FontText(TEXT("????"));

	CREATE_GAMEOBJECT(m_pOxenJumpText, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pOxenJumpText);
}

void CUI_HUD::Create_PlayerNameText()
{
	m_pPlayerNameText = CUI_Object::Create();

	m_pPlayerNameText->Set_Scale(20.f);
	m_pPlayerNameText->Set_Pos(-535.f, -283.f);
	m_pPlayerNameText->Set_Sort(0.85f);

	m_pPlayerNameText->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Circle/T_256Circle.dds"));

	m_pPlayerNameText->Set_FontRender(true);
	m_pPlayerNameText->Set_FontStyle(true);
	m_pPlayerNameText->Set_FontScale(0.25f);
	m_pPlayerNameText->Set_FontOffset(13.f, -13.f);

	wstring wstrPlayerName = CUser::Get_Instance()->Get_MainPlayerInfo()->Get_PlayerName();
	m_pPlayerNameText->Set_FontText(wstrPlayerName);

	CREATE_GAMEOBJECT(m_pPlayerNameText, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pPlayerNameText);
}

void CUI_HUD::Create_InactiveHeroText()
{
	m_pInactiveHeroText = CUI_Object::Create();

	m_pInactiveHeroText->Set_FadeDesc(0.3f);

	m_pInactiveHeroText->Set_Scale(20.f);
	m_pInactiveHeroText->Set_Pos(445.f, -191.f);
	m_pInactiveHeroText->Set_Sort(0.85f);

	m_pInactiveHeroText->Set_FadeDesc(1.2f);

	m_pInactiveHeroText->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KeyIcon/Keyboard/White/Num1.dds"));

	m_pInactiveHeroText->Set_FontRender(true);
	m_pInactiveHeroText->Set_FontStyle(true);
	m_pInactiveHeroText->Set_FontScale(0.25f);
	m_pInactiveHeroText->Set_FontOffset(10.f, -13.f);

	m_pInactiveHeroText->Set_FontText(TEXT("???? ????"));

	CREATE_GAMEOBJECT(m_pInactiveHeroText, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pInactiveHeroText);
}

void CUI_HUD::Bind_Shader()
{
	if (m_pHeroTransformUI[HT_Bar])
	{
		GET_COMPONENT_FROM(m_pHeroTransformUI[HT_Bar], CShader)->CallBack_SetRawValues += bind(&CUI_HUD::Set_Shader_HeroTransformGauge, this, placeholders::_1, "g_fValue");
	}
}

void CUI_HUD::Create_HeroTransformUI()
{
	for (int i = 0; i < HT_End; ++i)
	{
		m_pHeroTransformUI[i] = CUI_Object::Create();

		m_pHeroTransformUI[i]->Set_PosY(-100.f);
		m_pHeroTransformUI[i]->Set_Scale(200.f, 10.f);

		CREATE_GAMEOBJECT(m_pHeroTransformUI[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pHeroTransformUI[i]);
	}

	m_pHeroTransformUI[HT_BG]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/HpBar/T_HPBarBG.png"));
	m_pHeroTransformUI[HT_BG]->Set_Sort(0.5f);
	m_pHeroTransformUI[HT_BG]->Set_Color(_float4(0.f, 0.f, 0.f, 0.4f));

	m_pHeroTransformUI[HT_Bar]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/HpBar/T_HPBarGrey.dds"));
	m_pHeroTransformUI[HT_Bar]->Set_Sort(0.49f);

	GET_COMPONENT_FROM(m_pHeroTransformUI[HT_Bar], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_HorizontalGauge);
}

void CUI_HUD::Create_RevivalUI()
{
	m_pRevivalUI = CUI_Object::Create();

	m_pRevivalUI->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Revive/T_ReviveDeco.dds"));
	m_pRevivalUI->Set_Sort(0.5f);
	m_pRevivalUI->Set_PosY(-150.f);
	m_pRevivalUI->Set_Scale(148.f);

	m_pRevivalUI->Set_FontRender(true);
	m_pRevivalUI->Set_FontStyle(true);
	m_pRevivalUI->Set_FontCenter(true);
	m_pRevivalUI->Set_FontOffset(5.f, 5.f);
	m_pRevivalUI->Set_FontScale(0.4f);
	m_pRevivalUI->Set_FontText(TEXT("???? ??"));

	CREATE_GAMEOBJECT(m_pRevivalUI, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pRevivalUI);
}

void CUI_HUD::Update_HeorTransformGauge()
{
	if (m_pHeroTransformUI[HT_Bar])
	{
		if (m_pHeroTransformUI[HT_Bar]->Is_Valid())
		{
			m_fHeroTransformValue += fDT(0);
			m_fHeroTransformGaugeRatio = (m_fHeroTransformValue / m_fMaxHeroTransformValue);

			if (m_fHeroTransformValue > m_fMaxHeroTransformValue)
			{
				m_fHeroTransformValue = m_fMaxHeroTransformValue;

				for (int i = 0; i < HT_End; ++i)
				{
					DISABLE_GAMEOBJECT(m_pHeroTransformUI[i]);
				}

				m_fHeroTransformValue = 0.f;
			}
		}
	}
}