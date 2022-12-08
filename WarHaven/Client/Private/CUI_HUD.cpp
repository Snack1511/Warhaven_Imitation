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

#include "CPlayer.h"
#include "CUI_Cursor.h"
#include "Loading_Manager.h"
#include "CUI_Renderer.h"

#include "CUI_CharacterWindow.h"

#include "CGameSystem.h"


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

	if (m_eLoadLevel <= LEVEL_TYPE_CLIENT::LEVEL_BOOTCAMP)
	{
		Create_CharacterWindow();
	}
	else
	{
		Create_OperWindow(m_eLoadLevel);
	}

	return S_OK;
}

HRESULT CUI_HUD::Start()
{
	Bind_Btn();
	Bind_Shader();

	if (m_eLoadLevel <= LEVEL_TYPE_CLIENT::LEVEL_BOOTCAMP)
	{
		SetActive_HUD(true);

		if (m_pClassChangeText)
			m_pClassChangeText->SetActive(true);
	}
	else
	{
		Set_FadeOperSelectChaderUI();
		SetActive_OperUI(true);
	}

	__super::Start();

	return S_OK;
}

void CUI_HUD::My_Tick()
{
	__super::My_Tick();

	m_tStatus = CUser::Get_Instance()->Get_Player()->Get_Status();

	Update_HeorTransformGauge();
	Update_OperWindow();

	Active_CharacterWindow();
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
		if (KEY(F, TAP))
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

#ifdef _DEBUG

		else if (KEY(T, TAP) && KEY(CTRL, HOLD))
		{
			CUser::Get_Instance()->Turn_HeroGaugeFire(false);
			SetActive_HUD(false);
			m_pUI_CharacterWindow->SetActive_CharacterWindow(true);
		}

#else

		else if (KEY(T, TAP))
		{
			SetActive_HUD(false);
			m_pUI_CharacterWindow->SetActive_CharacterWindow(true);
		}

#endif

	}
}

void CUI_HUD::On_PointDown_SelectBG(const _uint& iEventNum)
{
	m_iPrvSelectEventNum = m_iCurSelectEventNum;
	m_iCurSelectEventNum = iEventNum;

	if (m_iPrvSelectEventNum == iEventNum)
		return;

	for (int i = 0; i < ST_End; ++i)
	{
		m_pArrOperSelectUI[i][m_iPrvSelectEventNum]->DoScale(-10.f, 0.1f);
		m_pArrOperSelectUI[i][iEventNum]->DoScale(10.f, 0.1f);
	}

	CUser::Get_Instance()->Get_MainPlayerInfo()->Set_ChosenClass((CLASS_TYPE)iEventNum);
}

void CUI_HUD::On_PointDown_Point(const _uint& iEventNum)
{
	DISABLE_GAMEOBJECT(m_pArrTargetPoint[1]);

	_float4 vPos = m_pArrOperPointUI[PT_Point][iEventNum]->Get_Pos();
	m_pArrTargetPoint[1]->Set_Pos(vPos.x, vPos.y + 20.f);

	GET_COMPONENT_FROM(m_pBriefingUI[BU_Icon], CTexture)->Set_CurTextureIndex(1);
	m_pBriefingUI[BU_Icon]->Set_Scale(20.f, 15.f);
	m_pBriefingUI[BU_Icon]->Set_Color(_float4(0.f, 0.6f, 0.f, 1.f));
	m_pBriefingUI[BU_Icon]->Set_FontColor(_float4(0.f, 0.6f, 0.f, 1.f));
	m_pBriefingUI[BU_Icon]->Set_FontText(TEXT("목표 설정 완료"));

	ENABLE_GAMEOBJECT(m_pArrTargetPoint[1]);
}

void CUI_HUD::Set_Shader_Smoke(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fSmokeUV, sizeof(_float));
}

void CUI_HUD::Set_Shader_Timer(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fTimerRatio, sizeof(_float));
}

void CUI_HUD::Set_Shader_HeroTransformGauge(CShader* pShader, const char* pConstName)
{
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

_bool CUI_HUD::Is_OnHeroGauge()
{
	return m_pPlayerNameText->Is_Valid();
}

void CUI_HUD::Set_SkillCoolTime(_uint iSkillType, _float fCoolTime, _float fMaxCoolTime)
{
	//dynamic_cast<CUI_Skill*>(m_pWrap[Skill])->Set_CoolTime(iSkillType, fCoolTime, fMaxCoolTime);
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
		//CUser::Get_Instance()->Get_Player()->Get_OwnerPlayer()->IsHero() = true; 임시로 주석걸어놓음
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

void CUI_HUD::Bind_Btn()
{
	for (int i = 0; i < 6; ++i)
	{
		if (m_pArrOperSelectUI[ST_BG][i])
			m_pArrOperSelectUI[ST_BG][i]->CallBack_PointDown += bind(&CUI_HUD::On_PointDown_SelectBG, this, i);
	}

	for (int i = 0; i < 3; ++i)
	{
		if (m_pArrOperPointUI[PT_Point][i])
			m_pArrOperPointUI[PT_Point][i]->CallBack_PointDown += bind(&CUI_HUD::On_PointDown_Point, this, i);
	}
}



void CUI_HUD::Create_BriefingUI()
{
	for (int i = 0; i < BU_End; ++i)
	{
		m_pBriefingUI[i] = CUI_Object::Create();

		CREATE_GAMEOBJECT(m_pBriefingUI[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pBriefingUI[i]);
	}

	m_pBriefingUI[BU_BG]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Oper/Briefing/T_BriefingBox.dds"));
	m_pBriefingUI[BU_BG]->Set_PosY(245.f);
	m_pBriefingUI[BU_BG]->Set_Scale(160.f, 25.f);
	m_pBriefingUI[BU_BG]->Set_Color(_float4(0.f, 0.f, 0.f, 0.6f));
	m_pBriefingUI[BU_BG]->Set_Sort(0.5f);

	GET_COMPONENT_FROM(m_pBriefingUI[BU_Icon], CTexture)->Remove_Texture(0);
	Read_Texture(m_pBriefingUI[BU_Icon], "/Oper/Briefing", "Icon");

	m_pBriefingUI[BU_Icon]->Set_Sort(0.49f);
	m_pBriefingUI[BU_Icon]->Set_Pos(-45.f, 246.f);
	m_pBriefingUI[BU_Icon]->Set_Scale(32.f);
	m_pBriefingUI[BU_Icon]->Set_Color(_float4(0.6f, 0.6f, 0.6f, 1.f));

	m_pBriefingUI[BU_Icon]->Set_FontRender(true);
	m_pBriefingUI[BU_Icon]->Set_FontStyle(true);
	m_pBriefingUI[BU_Icon]->Set_FontOffset(10.f, -10.f);
	m_pBriefingUI[BU_Icon]->Set_FontScale(0.2f);
	m_pBriefingUI[BU_Icon]->Set_FontColor(_float4(_float4(0.6f, 0.6f, 0.6f, 1.f)));
	m_pBriefingUI[BU_Icon]->Set_FontText(TEXT("공격 목표 없음"));
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

	GET_COMPONENT_FROM(m_pInactiveHeroText, CFader)->Get_FadeDesc() = tFadeDesc;
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

	m_pClassChangeText->Set_FontText(TEXT("로 전투원 변경 가능"));

	CREATE_GAMEOBJECT(m_pClassChangeText, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pClassChangeText);
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

void CUI_HUD::SetActive_OperUI(_bool value)
{
	if (value == true)
	{
		ENABLE_GAMEOBJECT(m_pOperWindow);
		ENABLE_GAMEOBJECT(m_pSmokeBG);

		m_pOperWindow->DoScale(-2596.f, 0.3f);
	}
	else
	{
		DISABLE_GAMEOBJECT(CUser::Get_Instance()->Get_Cursor());

		DISABLE_GAMEOBJECT(m_pOperWindow);
		DISABLE_GAMEOBJECT(m_pOperTextImg);
	}
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

	wstring wstrPlayerName = CUser::Get_Instance()->Get_Player()->Get_OwnerPlayer()->Get_PlayerName();
	m_pPlayerNameText->Set_FontText(wstrPlayerName);

	CREATE_GAMEOBJECT(m_pPlayerNameText, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pPlayerNameText);
}

void CUI_HUD::Create_InactiveHeroText()
{
	m_pInactiveHeroText = CUI_Object::Create();

	m_pInactiveHeroText->Set_Scale(20.f);
	m_pInactiveHeroText->Set_Pos(445.f, -191.f);
	m_pInactiveHeroText->Set_Sort(0.85f);

	m_pInactiveHeroText->Set_FadeDesc(1.2f);

	m_pInactiveHeroText->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KeyIcon/Keyboard/White/Num1.dds"));

	m_pInactiveHeroText->Set_FontRender(true);
	m_pInactiveHeroText->Set_FontStyle(true);
	m_pInactiveHeroText->Set_FontScale(0.25f);
	m_pInactiveHeroText->Set_FontOffset(10.f, -13.f);

	m_pInactiveHeroText->Set_FontText(TEXT("화신 해제"));

	CREATE_GAMEOBJECT(m_pInactiveHeroText, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pInactiveHeroText);
}

void CUI_HUD::Update_OperWindow()
{
	if (!m_pOperWindow)
		return;

	if (m_pSmokeBG->Is_Valid())
	{
		m_fSmokeUV += fDT(0) * 0.01f;
	}

	if (m_pOperWindow->Is_Valid())
	{
		m_fAccTime += fDT(0);

		if (m_iOperWindowCnt == 0)
		{
			if (m_fAccTime > 0.5f)
			{
				m_fAccTime = 0.f;

				if (!m_pOperBlackBG->Is_Valid())
				{
					Enable_Fade(m_pOperBlackBG, 0.15f);

					m_iOperWindowCnt++;
				}
			}
		}
		else if (m_iOperWindowCnt == 1)
		{
			if (m_pOperBlackBG->Is_Valid())
			{
				_float fDuration = 0.3f;
				if (m_fAccTime > fDuration)
				{
					m_fAccTime = 0.f;

					if (!m_pOperTextImg->Is_Valid())
					{
						Enable_Fade(m_pOperTextImg, fDuration);
						m_pOperTextImg->DoScale(-512.f, fDuration);
					}

					m_iOperWindowCnt++;
				}
			}
		}
		else if (m_iOperWindowCnt == 2)
		{
			if (m_pOperTextImg->Is_Valid())
			{
				if (m_fAccTime > 1.f)
				{
					m_fAccTime = 0.f;

					_float fDuration = 0.3f;
					m_pOperTextImg->DoMoveY(200.f, fDuration);
					m_pOperTextImg->DoScale(-256.f, fDuration);

					for (int i = 0; i < 4; ++i)
					{
						Enable_Fade(m_pArrOperProfile[i], fDuration);
					}

					m_iOperWindowCnt++;
				}
			}
		}
		else if (m_iOperWindowCnt == 3)
		{
			if (m_pArrOperProfile[0]->Is_Valid())
			{
				if (m_fAccTime > 3.f)
				{
					m_fAccTime = 0.f;

					_float fDuration = 0.3f;
					Disable_Fade(m_pOperBlackBG, fDuration);
					Disable_Fade(m_pOperTextImg, fDuration);
					Disable_Fade(m_pSquardTextImg, fDuration);

					for (int i = 0; i < 4; ++i)
					{
						Disable_Fade(m_pArrOperProfile[i], fDuration);
					}

					m_iOperWindowCnt++;
				}
			}
		}
		else if (m_iOperWindowCnt == 4)
		{
			for (int i = 0; i < TU_End; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					Enable_Fade(m_pArrTeamUI[i][j], 0.3f);
				}
			}

			Enable_OperPointUI();
		}
		else if (m_iOperWindowCnt == 5)
		{
			if (m_fAccTime > 0.3f)
			{
				_float fDuration = 1.f;

				Enable_Fade(m_pArrOperPointCircleEffect[0], fDuration);
				Enable_Fade(m_pArrOperPointCircleEffect[2], fDuration);

				m_pArrOperPointCircleEffect[0]->DoScale(70.f, fDuration);
				m_pArrOperPointCircleEffect[2]->DoScale(70.f, fDuration);

				m_fAccTime = 0.f;
				m_iOperWindowCnt++;
			}
		}
		else if (m_iOperWindowCnt == 6)
		{
			if (m_fAccTime > 0.5f)
			{
				_float fDuration = 1.f;

				Enable_Fade(m_pArrOperPointCircleEffect[1], fDuration);
				Enable_Fade(m_pArrOperPointCircleEffect[3], fDuration);

				m_pArrOperPointCircleEffect[1]->DoScale(70.f, fDuration);
				m_pArrOperPointCircleEffect[3]->DoScale(70.f, fDuration);

				m_fAccTime = 0.f;
				m_iOperWindowCnt++;
			}
		}
		else if (m_iOperWindowCnt == 7)
		{
			if (m_fAccTime > 1.f)
			{
				_float fDuration = 0.3f;
				for (int i = 0; i < 2; ++i)
				{
					Enable_Fade(m_pArrOperSideBG[i], fDuration);
				}
				m_pArrOperSideBG[0]->DoMoveX(50.f, fDuration);
				m_pArrOperSideBG[1]->DoMoveX(-50.f, fDuration);

				for (int i = 0; i < ST_End; ++i)
				{
					for (int j = 0; j < 6; ++j)
					{
						Enable_Fade(m_pArrOperSelectUI[i][j], fDuration);
						m_pArrOperSelectUI[i][j]->DoMoveX(50.f, fDuration);
					}
				}

				for (int i = 0; i < ST_End; ++i)
				{
					m_pArrOperSelectUI[i][0]->DoScale(10.f, fDuration);
				}

				Enable_Fade(m_pOperMapIcon, fDuration);
				Enable_Fade(m_pOperMapBG, fDuration);
				Enable_Fade(m_pOperTextImg2, fDuration);
				Enable_Fade(m_pOperAttackPointText, fDuration);
				Enable_Fade(m_pArrTargetPoint[0], fDuration);

				for (int i = 0; i < TT_End; ++i)
				{
					m_fOperTime = m_fMaxOperTime;
					Enable_Fade(m_pOperTimer[i], fDuration);
				}

				for (int i = 0; i < BU_End; ++i)
				{
					Enable_Fade(m_pBriefingUI[i], fDuration);
				}

				m_fAccTime = 0.f;
				m_iOperWindowCnt++;
			}
		}
		else if (m_iOperWindowCnt == 8)
		{
			_tchar  szTemp[MAX_STR] = {};
			swprintf_s(szTemp, TEXT("%04.1f"), m_fOperTime);
			m_pOperTimer[TT_Bar]->Set_FontText(szTemp);

			m_fTimerRatio = m_fOperTime / m_fMaxOperTime;
			m_fOperTime -= fDT(0);
			if (m_fOperTime < 0.f)
			{
				m_fOperTime = 0.f;
				On_OperTimeOver();
			}
		}
	}
}

void CUI_HUD::Bind_Shader()
{
	if (m_pSmokeBG)
	{
		GET_COMPONENT_FROM(m_pSmokeBG, CShader)->CallBack_SetRawValues += bind(&CUI_HUD::Set_Shader_Smoke, this, placeholders::_1, "g_fValue");
	}

	if (m_pOperTimer[TT_Bar])
	{
		GET_COMPONENT_FROM(m_pOperTimer[TT_Bar], CShader)->CallBack_SetRawValues += bind(&CUI_HUD::Set_Shader_Timer, this, placeholders::_1, "g_fValue");
	}

	if (m_pHeroTransformUI[HT_Bar])
	{
		GET_COMPONENT_FROM(m_pHeroTransformUI[HT_Bar], CShader)->CallBack_SetRawValues += bind(&CUI_HUD::Set_Shader_HeroTransformGauge, this, placeholders::_1, "g_fValue");
	}
}

void CUI_HUD::Create_OperWindow(LEVEL_TYPE_CLIENT eLoadLevel)
{
	m_pOperWindow = CUI_Object::Create();
	m_pOperWindow->Set_Scale(4096.f);
	m_pOperWindow->Set_Sort(0.52f);

	switch (eLoadLevel)
	{
	case Client::LEVEL_PADEN:
		m_pOperWindow->Set_PosY(205.f);
		m_pOperWindow->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Map/T_MinimapPaden.dds"));
		break;

	case Client::LEVEL_HWARA:
		m_pOperWindow->Set_PosY(0.f);
		m_pOperWindow->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Map/T_MinimapHwara.dds"));
		break;
	}

	m_pSmokeBG = CUI_Object::Create();
	m_pSmokeBG->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Oper/T_soft_smoke.dds"));
	m_pSmokeBG->SetTexture(TEXT("../Bin/Resources/Textures/UI/Oper/T_Noise_02.dds"));
	m_pSmokeBG->Set_Scale(4096.f);
	m_pSmokeBG->Set_Sort(0.51f);

	GET_COMPONENT_FROM(m_pSmokeBG, CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_OperSmoke);

	m_pOperBlackBG = CUI_Object::Create();
	m_pOperBlackBG->Set_Texture(TEXT("../Bin/Resources/Textures/Black.png"));
	m_pOperBlackBG->Set_Scale(1280.f);
	m_pOperBlackBG->Set_Sort(0.5f);
	m_pOperBlackBG->Set_Color(_float4(1.f, 1.f, 1.f, 0.9f));

	m_pOperTextImg = CUI_Object::Create();
	m_pOperTextImg->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Oper/OperMeeting.png"));
	m_pOperTextImg->Set_PosY(50.f);
	m_pOperTextImg->Set_Scale(1024.f);
	m_pOperTextImg->Set_Sort(0.49f);

	m_pOperTextImg2 = CUI_Object::Create();
	m_pOperTextImg2->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Oper/OperMeeting2.png"));
	m_pOperTextImg2->Set_PosY(305.f);
	m_pOperTextImg2->Set_Scale(155.f, 50.f);
	m_pOperTextImg2->Set_Sort(0.49f);

	m_pSquardTextImg = CUI_Object::Create();
	m_pSquardTextImg->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Oper/SquadText.png"));
	m_pSquardTextImg->Set_PosY(-20.f);
	m_pSquardTextImg->Set_Scale(204.f, 38.f);
	m_pSquardTextImg->Set_Sort(0.48f);

	FADEDESC tFadeDesc;
	ZeroMemory(&tFadeDesc, sizeof(FADEDESC));
	tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_NONE;
	tFadeDesc.eFadeStyle = FADEDESC::FADE_STYLE_DEFAULT;
	tFadeDesc.bFadeInFlag = FADE_NONE;
	tFadeDesc.bFadeOutFlag = FADE_NONE;
	tFadeDesc.fFadeInStartTime = 0.f;
	tFadeDesc.fFadeInTime = 0.15f;
	tFadeDesc.fFadeOutStartTime = 0.f;
	tFadeDesc.fFadeOutTime = 0.3f;
	GET_COMPONENT_FROM(m_pOperBlackBG, CFader)->Get_FadeDesc() = tFadeDesc;
	GET_COMPONENT_FROM(m_pOperTextImg, CFader)->Get_FadeDesc() = tFadeDesc;

	CREATE_GAMEOBJECT(m_pOperWindow, RENDER_UI);
	DISABLE_GAMEOBJECT(m_pOperWindow);

	CREATE_GAMEOBJECT(m_pSmokeBG, RENDER_UI);
	DISABLE_GAMEOBJECT(m_pSmokeBG);

	CREATE_GAMEOBJECT(m_pOperBlackBG, RENDER_UI);
	DISABLE_GAMEOBJECT(m_pOperBlackBG);

	CREATE_GAMEOBJECT(m_pOperTextImg, RENDER_UI);
	DISABLE_GAMEOBJECT(m_pOperTextImg);

	CREATE_GAMEOBJECT(m_pOperTextImg2, RENDER_UI);
	DISABLE_GAMEOBJECT(m_pOperTextImg2);

	CREATE_GAMEOBJECT(m_pSquardTextImg, RENDER_UI);
	DISABLE_GAMEOBJECT(m_pSquardTextImg);

	Create_OperProfile();
	Create_OperSideBG();
	Create_OperSelectCharacter();
	Create_OperMap();
	Create_OperPoint();
	Create_OperPointEffect();
	Create_OperTimer();
	Create_BriefingUI();
	Create_TeamUI();
}

void CUI_HUD::Set_FadeOperSelectChaderUI()
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

	GET_COMPONENT_FROM(m_pOperSelectUI[ST_Char], CFader)->Get_FadeDesc() = tFadeDesc;
	GET_COMPONENT_FROM(m_pOperSelectUI[ST_Port], CFader)->Get_FadeDesc() = tFadeDesc;
	GET_COMPONENT_FROM(m_pOperSelectUI[ST_Icon], CFader)->Get_FadeDesc() = tFadeDesc;

	for (int i = 0; i < PT_End; ++i)
	{
		GET_COMPONENT_FROM(m_pOperPointUI[i], CFader)->Get_FadeDesc() = tFadeDesc;
	}

	GET_COMPONENT_FROM(m_pOperSideBG, CFader)->Get_FadeDesc() = tFadeDesc;
	GET_COMPONENT_FROM(m_pOperTextImg2, CFader)->Get_FadeDesc() = tFadeDesc;
	GET_COMPONENT_FROM(m_pOperMapIcon, CFader)->Get_FadeDesc() = tFadeDesc;
	GET_COMPONENT_FROM(m_pOperMapBG, CFader)->Get_FadeDesc() = tFadeDesc;

	GET_COMPONENT_FROM(m_pOperAttackPointText, CFader)->Get_FadeDesc() = tFadeDesc;
	GET_COMPONENT_FROM(m_pTargetPoint, CFader)->Get_FadeDesc() = tFadeDesc;
	GET_COMPONENT_FROM(m_pSquardTextImg, CFader)->Get_FadeDesc() = tFadeDesc;

	for (int i = 0; i < TT_End; ++i)
	{
		GET_COMPONENT_FROM(m_pOperTimer[i], CFader)->Get_FadeDesc() = tFadeDesc;
	}

	for (int i = 0; i < BU_End; ++i)
	{
		GET_COMPONENT_FROM(m_pBriefingUI[i], CFader)->Get_FadeDesc() = tFadeDesc;
	}

	for (int i = 0; i < TU_End; ++i)
	{
		GET_COMPONENT_FROM(m_pTeamUI[i], CFader)->Get_FadeDesc() = tFadeDesc;
	}

	FADEDESC tFadeDescBG;
	ZeroMemory(&tFadeDesc, sizeof(FADEDESC));
	tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_NONE;
	tFadeDesc.eFadeStyle = FADEDESC::FADE_STYLE_DEFAULT;
	tFadeDesc.bFadeInFlag = FADE_NONE;
	tFadeDesc.bFadeOutFlag = FADE_NONE;
	tFadeDesc.fFadeInStartTime = 0.f;
	tFadeDesc.fFadeInTime = 0.3f;
	tFadeDesc.fFadeOutStartTime = 0.f;
	tFadeDesc.fFadeOutTime = 0.3f;

	GET_COMPONENT_FROM(m_pOperSelectUI[ST_BG], CFader)->Get_FadeDesc() = tFadeDescBG;
}

void CUI_HUD::Create_OperProfile()
{
	m_pOperProfile = CUI_Object::Create();
	m_pOperProfile->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Profile/T_ProfileCardDefaultArcher.dds"));
	m_pOperProfile->Set_Scale(156.f, 350.f);
	m_pOperProfile->Set_Sort(0.49f);

	FADEDESC tFadeDesc;
	ZeroMemory(&tFadeDesc, sizeof(FADEDESC));
	tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_DISABLE;
	tFadeDesc.eFadeStyle = FADEDESC::FADE_STYLE_DEFAULT;
	tFadeDesc.bFadeInFlag = FADE_NONE;
	tFadeDesc.bFadeOutFlag = FADE_NONE;
	tFadeDesc.fFadeInStartTime = 0.f;
	tFadeDesc.fFadeInTime = 0.5f;
	tFadeDesc.fFadeOutStartTime = 0.f;
	tFadeDesc.fFadeOutTime = 0.3f;

	CREATE_GAMEOBJECT(m_pOperProfile, RENDER_UI);
	DELETE_GAMEOBJECT(m_pOperProfile);

	for (int i = 0; i < 4; ++i)
	{
		m_pArrOperProfile[i] = m_pOperProfile->Clone();

		_float fPosX = -375.f + (i * 250.f);
		m_pArrOperProfile[i]->Set_PosX(fPosX);

		GET_COMPONENT_FROM(m_pArrOperProfile[i], CFader)->Get_FadeDesc() = tFadeDesc;

		CREATE_GAMEOBJECT(m_pArrOperProfile[i], RENDER_UI);
		DISABLE_GAMEOBJECT(m_pArrOperProfile[i]);
	}
}

void CUI_HUD::Create_OperSideBG()
{
	m_pOperSideBG = CUI_Object::Create();
	m_pOperSideBG->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Oper/T_ScrollBG.png"));
	m_pOperSideBG->Set_Scale(250.f, 753.f);
	m_pOperSideBG->Set_Sort(0.49f);

	CREATE_GAMEOBJECT(m_pOperSideBG, RENDER_UI);
	DELETE_GAMEOBJECT(m_pOperSideBG);

	for (int i = 0; i < 2; ++i)
	{
		m_pArrOperSideBG[i] = m_pOperSideBG->Clone();

		_float fPosX = -565 + (i * 1130.f);
		m_pArrOperSideBG[i]->Set_PosX(fPosX);

		if (i == 1)
		{
			m_pArrOperSideBG[1]->Set_RotationZ(180.f);
		}

		CREATE_GAMEOBJECT(m_pArrOperSideBG[i], RENDER_UI);
		DISABLE_GAMEOBJECT(m_pArrOperSideBG[i]);
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

void CUI_HUD::Enable_OperPointUI()
{
	for (int i = 0; i < PT_End; ++i)
	{
		switch (m_eLoadLevel)
		{
		case LEVEL_PADEN:

			for (int j = 0; j < 2; ++j)
			{
				Enable_Fade(m_pArrOperPointUI[i][j], 0.3f);
				m_pArrOperPointUI[i][j]->DoScale(-70.f, 0.3f);
			}

			DISABLE_GAMEOBJECT(m_pArrOperPointUI[PT_Icon][0]);

			break;
		}
	}

	m_fAccTime = 0.f;
	m_iOperWindowCnt++;
}

void CUI_HUD::Update_HeorTransformGauge()
{
	if (m_pHeroTransformUI[HT_Bar])
	{
		if (m_pHeroTransformUI[HT_Bar]->Is_Valid())
		{
			m_fHeroTransformValue += fDT(0);
			m_fHeroTransformGaugeRatio = m_fHeroTransformValue / m_fMaxHeroTransformValue;

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

void CUI_HUD::On_OperTimeOver()
{
	/* 작전회의 시간 모두 지나가고 호출되는 함수 */

	//1. 검은화면 fade in out

	//검은화면 fade in 되면 아래 호출 (임시로 적어놈)
	CGameSystem::Get_Instance()->On_StartGame();

	Disable_AllOperUIs();

	SetActive_HUD(true);
}

void CUI_HUD::Disable_AllOperUIs()
{
#define	DISABLE_OPERUI_ARR(name) for (auto& elem : name) if (elem)DISABLE_GAMEOBJECT(elem);
#define	DISABLE_OPERUI_DARR(name) for (auto& elem : name) for (auto& elem2 : elem) if (elem2) DISABLE_GAMEOBJECT(elem2);


	DISABLE_OPERUI_ARR(m_pArrOperSideBG);
	DISABLE_OPERUI_DARR(m_pArrOperSelectUI);
	DISABLE_OPERUI_DARR(m_pArrOperPointUI);
	//DISABLE_OPERUI_DARR(m_pArrBootCampUI);
	DISABLE_OPERUI_ARR(m_pArrTargetPoint);

	if (m_pOperWindow)
		DISABLE_GAMEOBJECT(m_pOperWindow);
	if (m_pSmokeBG)
		DISABLE_GAMEOBJECT(m_pSmokeBG);
	if (m_pOperTextImg2)
		DISABLE_GAMEOBJECT(m_pOperTextImg2);
	if (m_pSquardTextImg)
		DISABLE_GAMEOBJECT(m_pSquardTextImg);
	if (m_pOperMapIcon)
		DISABLE_GAMEOBJECT(m_pOperMapIcon);
	if (m_pOperMapBG)
		DISABLE_GAMEOBJECT(m_pOperMapBG);
	if (m_pOperAttackPointText)
		DISABLE_GAMEOBJECT(m_pOperAttackPointText);
	DISABLE_OPERUI_DARR(m_pArrTeamUI);

	DISABLE_OPERUI_ARR(m_pOperTimer);
	DISABLE_OPERUI_ARR(m_pBriefingUI);



}

void CUI_HUD::Create_OperSelectCharacter()
{
	for (int i = 0; i < ST_End; ++i)
	{
		m_pOperSelectUI[i] = CUI_Object::Create();

		CREATE_GAMEOBJECT(m_pOperSelectUI[i], RENDER_UI);
		DELETE_GAMEOBJECT(m_pOperSelectUI[i]);
	}

	m_pOperSelectUI[ST_BG]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Oper/T_SelectedBG.png"));
	m_pOperSelectUI[ST_BG]->Set_Scale(213.f, 65.f);
	m_pOperSelectUI[ST_BG]->Set_Sort(0.49f);
	m_pOperSelectUI[ST_BG]->Set_MouseTarget(true);

	m_pOperSelectUI[ST_Char]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Portrait/T_RoundPortraitBG.dds"));
	m_pOperSelectUI[ST_Char]->Set_Scale(65.f);
	m_pOperSelectUI[ST_Char]->Set_Sort(0.48f);

	m_pOperSelectUI[ST_Port]->Set_Scale(65.f);
	m_pOperSelectUI[ST_Port]->Set_Sort(0.47f);

	m_pOperSelectUI[ST_Icon]->Set_Scale(32.f);
	m_pOperSelectUI[ST_Icon]->Set_Sort(0.48f);
	m_pOperSelectUI[ST_Icon]->Set_FontRender(true);
	m_pOperSelectUI[ST_Icon]->Set_FontStyle(true);
	m_pOperSelectUI[ST_Icon]->Set_FontOffset(20.f, -15.f);
	m_pOperSelectUI[ST_Icon]->Set_FontScale(0.3f);

	GET_COMPONENT_FROM(m_pOperSelectUI[ST_Port], CTexture)->Remove_Texture(0);
	Read_Texture(m_pOperSelectUI[ST_Port], "/HUD/Portrait", "Class");

	GET_COMPONENT_FROM(m_pOperSelectUI[ST_Icon], CTexture)->Remove_Texture(0);
	Read_Texture(m_pOperSelectUI[ST_Icon], "/Oper", "Class");

	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < ST_End; ++j)
		{
			m_pArrOperSelectUI[j][i] = m_pOperSelectUI[j]->Clone();

			CREATE_GAMEOBJECT(m_pArrOperSelectUI[j][i], RENDER_UI);
			DISABLE_GAMEOBJECT(m_pArrOperSelectUI[j][i]);
		}

		GET_COMPONENT_FROM(m_pArrOperSelectUI[ST_Port][i], CTexture)->Set_CurTextureIndex(i);
		GET_COMPONENT_FROM(m_pArrOperSelectUI[ST_Icon][i], CTexture)->Set_CurTextureIndex(i);
	}

#pragma region 유아이 위치 지정

	_float fTopPosY = 250.f;
	_float fMidPosY = 150.f;
	_float fBotPosY = 50.f;

	_float fTopPosXSelectBG = -480.f;
	_float fMidPosXSelectBG = -515.f;
	_float fBotPosXSelectBG = -530.f;

	m_pArrOperSelectUI[ST_BG][0]->Set_Pos(fTopPosXSelectBG, fTopPosY);
	m_pArrOperSelectUI[ST_BG][1]->Set_Pos(fMidPosXSelectBG, fMidPosY);
	m_pArrOperSelectUI[ST_BG][2]->Set_Pos(fBotPosXSelectBG, fBotPosY);
	m_pArrOperSelectUI[ST_BG][3]->Set_Pos(fBotPosXSelectBG, -fBotPosY);
	m_pArrOperSelectUI[ST_BG][4]->Set_Pos(fMidPosXSelectBG, -fMidPosY);
	m_pArrOperSelectUI[ST_BG][5]->Set_Pos(fTopPosXSelectBG, -fTopPosY);

	_float fTopPosCharX = -555.f;
	_float fMidPosCharX = -590.f;
	_float fBotPosCharX = -605.f;

	m_pArrOperSelectUI[ST_Char][0]->Set_Pos(fTopPosCharX, fTopPosY);
	m_pArrOperSelectUI[ST_Char][1]->Set_Pos(fMidPosCharX, fMidPosY);
	m_pArrOperSelectUI[ST_Char][2]->Set_Pos(fBotPosCharX, fBotPosY);
	m_pArrOperSelectUI[ST_Char][3]->Set_Pos(fBotPosCharX, -fBotPosY);
	m_pArrOperSelectUI[ST_Char][4]->Set_Pos(fMidPosCharX, -fMidPosY);
	m_pArrOperSelectUI[ST_Char][5]->Set_Pos(fTopPosCharX, -fTopPosY);

	m_pArrOperSelectUI[ST_Port][0]->Set_Pos(fTopPosCharX, fTopPosY);
	m_pArrOperSelectUI[ST_Port][1]->Set_Pos(fMidPosCharX, fMidPosY);
	m_pArrOperSelectUI[ST_Port][2]->Set_Pos(fBotPosCharX, fBotPosY);
	m_pArrOperSelectUI[ST_Port][3]->Set_Pos(fBotPosCharX, -fBotPosY);
	m_pArrOperSelectUI[ST_Port][4]->Set_Pos(fMidPosCharX, -fMidPosY);
	m_pArrOperSelectUI[ST_Port][5]->Set_Pos(fTopPosCharX, -fTopPosY);

	_float fTopPosIconX = -505.f;
	_float fMidPosIconX = -540.f;
	_float fBotPosIconX = -555.f;

	m_pArrOperSelectUI[ST_Icon][0]->Set_FontText(TEXT("블레이드"));
	m_pArrOperSelectUI[ST_Icon][1]->Set_FontText(TEXT("스파이크"));
	m_pArrOperSelectUI[ST_Icon][2]->Set_FontText(TEXT("아치"));
	m_pArrOperSelectUI[ST_Icon][3]->Set_FontText(TEXT("가디언"));
	m_pArrOperSelectUI[ST_Icon][4]->Set_FontText(TEXT("스모크"));
	m_pArrOperSelectUI[ST_Icon][5]->Set_FontText(TEXT("워해머"));

	m_pArrOperSelectUI[ST_Icon][0]->Set_Pos(fTopPosIconX, fTopPosY);
	m_pArrOperSelectUI[ST_Icon][1]->Set_Pos(fMidPosIconX, fMidPosY);
	m_pArrOperSelectUI[ST_Icon][2]->Set_Pos(fBotPosIconX, fBotPosY);
	m_pArrOperSelectUI[ST_Icon][3]->Set_Pos(fBotPosIconX, -fBotPosY);
	m_pArrOperSelectUI[ST_Icon][4]->Set_Pos(fMidPosIconX, -fMidPosY);
	m_pArrOperSelectUI[ST_Icon][5]->Set_Pos(fTopPosIconX, -fTopPosY);

#pragma endregion	
}

void CUI_HUD::Create_OperMap()
{
	m_pOperMapIcon = CUI_Object::Create();
	m_pOperMapIcon->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Oper/T_MapIcon2.dds"));
	m_pOperMapIcon->Set_Pos(540.f, 150.f);
	m_pOperMapIcon->Set_Scale(32.f);
	m_pOperMapIcon->Set_Sort(0.47f);
	m_pOperMapIcon->Set_FontRender(true);
	m_pOperMapIcon->Set_FontStyle(true);
	m_pOperMapIcon->Set_FontOffset(-100.f, -15.f);
	m_pOperMapIcon->Set_FontScale(0.3f);
	m_pOperMapIcon->Set_FontText(TEXT("지도"));

	m_pOperMapBG = CUI_Object::Create();
	m_pOperMapBG->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Oper/T_SwitchHandle.png"));
	m_pOperMapBG->Set_Sort(0.48f);
	m_pOperMapBG->Set_Scale(65.f);
	m_pOperMapBG->Set_Pos(540.f, 150.f);

	CREATE_GAMEOBJECT(m_pOperMapIcon, RENDER_UI);
	DISABLE_GAMEOBJECT(m_pOperMapIcon);

	CREATE_GAMEOBJECT(m_pOperMapBG, RENDER_UI);
	DISABLE_GAMEOBJECT(m_pOperMapBG);
}

void CUI_HUD::Create_OperPoint()
{
	for (int i = 0; i < PT_End; ++i)
	{
		m_pOperPointUI[i] = CUI_Object::Create();

		CREATE_GAMEOBJECT(m_pOperPointUI[i], RENDER_UI);
		DELETE_GAMEOBJECT(m_pOperPointUI[i]);
	}

	GET_COMPONENT_FROM(m_pOperPointUI[PT_Point], CTexture)->Remove_Texture(0);
	Read_Texture(m_pOperPointUI[PT_Point], "/Oper", "PointOutline");

	GET_COMPONENT_FROM(m_pOperPointUI[PT_Gauge], CTexture)->Remove_Texture(0);
	Read_Texture(m_pOperPointUI[PT_Gauge], "/Oper", "PointGauge");

	GET_COMPONENT_FROM(m_pOperPointUI[PT_Icon], CTexture)->Remove_Texture(0);
	Read_Texture(m_pOperPointUI[PT_Icon], "/Oper", "PointIcon");

	GET_COMPONENT_FROM(m_pOperPointUI[PT_Text], CTexture)->Remove_Texture(0);
	Read_Texture(m_pOperPointUI[PT_Text], "/Oper", "PointText");

	m_pOperPointUI[PT_Point]->Set_Scale(120.f);
	m_pOperPointUI[PT_Point]->Set_Sort(0.48f);
	m_pOperPointUI[PT_Point]->Set_Color(_float4(0.9f, 0.9f, 0.9f, 0.5f));
	m_pOperPointUI[PT_Point]->Set_MouseTarget(true);

	m_pOperPointUI[PT_Gauge]->Set_Scale(115.f);
	m_pOperPointUI[PT_Gauge]->Set_Sort(0.49f);
	m_pOperPointUI[PT_Gauge]->Set_Color(_float4(0.4f, 0.4f, 0.4f, 1.f));

	m_pOperPointUI[PT_Icon]->Set_Scale(115.f);
	m_pOperPointUI[PT_Icon]->Set_Sort(0.485f);
	m_pOperPointUI[PT_Icon]->Set_Color(_float4(0.7f, 0.7f, 0.7f, 1.f));

	m_pOperPointUI[PT_Text]->Set_Scale(140.f);
	m_pOperPointUI[PT_Text]->Set_Sort(0.48f);

	for (int i = 0; i < PT_End; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			m_pArrOperPointUI[i][j] = m_pOperPointUI[i]->Clone();

			CREATE_GAMEOBJECT(m_pArrOperPointUI[i][j], RENDER_UI);
			DISABLE_GAMEOBJECT(m_pArrOperPointUI[i][j]);
		}
	}

	switch (m_eLoadLevel)
	{
	case LEVEL_PADEN:

		// 목표 거점
		m_pArrOperPointUI[PT_Point][0]->Set_PosY(133.f);
		m_pArrOperPointUI[PT_Gauge][0]->Set_PosY(132.f);
		m_pArrOperPointUI[PT_Text][0]->Set_PosY(128.f);

		// 중간 거점
		m_pArrOperPointUI[PT_Point][1]->Set_PosY(-161.f);
		m_pArrOperPointUI[PT_Gauge][1]->Set_PosY(-161.f);
		m_pArrOperPointUI[PT_Icon][1]->Set_PosY(-161.f);

		m_pArrOperPointUI[PT_Text][1]->Set_PosY(-166.f);

		for (int i = 0; i < 3; ++i)
		{
			GET_COMPONENT_FROM(m_pArrOperPointUI[PT_Point][i], CTexture)->Set_CurTextureIndex(i);
			GET_COMPONENT_FROM(m_pArrOperPointUI[PT_Gauge][i], CTexture)->Set_CurTextureIndex(i);
			GET_COMPONENT_FROM(m_pArrOperPointUI[PT_Icon][i], CTexture)->Set_CurTextureIndex(i);
			GET_COMPONENT_FROM(m_pArrOperPointUI[PT_Text][i], CTexture)->Set_CurTextureIndex(i);

			if (i > 0)
			{
				GET_COMPONENT_FROM(m_pArrOperPointUI[PT_Point][i], CTexture)->Set_CurTextureIndex(1);
				GET_COMPONENT_FROM(m_pArrOperPointUI[PT_Gauge][i], CTexture)->Set_CurTextureIndex(1);
				GET_COMPONENT_FROM(m_pArrOperPointUI[PT_Icon][i], CTexture)->Set_CurTextureIndex(1);
				GET_COMPONENT_FROM(m_pArrOperPointUI[PT_Text][i], CTexture)->Set_CurTextureIndex(1);
			}
		}

		break;
	}

#pragma region Etc

	m_pOperAttackPointText = CUI_Object::Create();
	m_pOperAttackPointText->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Oper/OperText.png"));
	m_pOperAttackPointText->Set_PosY(-250.f);
	m_pOperAttackPointText->Set_Scale(287.f, 50.f);
	m_pOperAttackPointText->Set_Sort(0.49f);

	m_pTargetPoint = CUI_Object::Create();
	m_pTargetPoint->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Oper/T_PingGreenLeader.dds"));
	m_pTargetPoint->Set_Pos(-45.f, -300.f);
	m_pTargetPoint->Set_Sort(0.47f);

	for (int i = 0; i < 2; ++i)
	{
		m_pArrTargetPoint[i] = m_pTargetPoint->Clone();

		CREATE_GAMEOBJECT(m_pArrTargetPoint[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pArrTargetPoint[i]);
	}

	m_pArrTargetPoint[0]->Set_Scale(30.f);
	m_pArrTargetPoint[0]->Set_FontRender(true);
	m_pArrTargetPoint[0]->Set_FontStyle(true);
	m_pArrTargetPoint[0]->Set_FontOffset(20.f, -13.f);
	m_pArrTargetPoint[0]->Set_FontScale(0.25f);
	m_pArrTargetPoint[0]->Set_FontText(TEXT("공격 목표"));

	m_pArrTargetPoint[1]->Set_Scale(32.f);

	CREATE_GAMEOBJECT(m_pOperAttackPointText, RENDER_UI);
	DISABLE_GAMEOBJECT(m_pOperAttackPointText);

	CREATE_GAMEOBJECT(m_pTargetPoint, RENDER_UI);
	DELETE_GAMEOBJECT(m_pTargetPoint);

#pragma endregion
}

void CUI_HUD::Create_OperPointEffect()
{
	m_pOperPointCircleEffect = CUI_Object::Create();
	m_pOperPointCircleEffect->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Circle/T_256Circle.dds"));
	m_pOperPointCircleEffect->Set_PosY(105.f);
	m_pOperPointCircleEffect->Set_Scale(30.f);
	m_pOperPointCircleEffect->Set_Sort(0.495f);
	m_pOperPointCircleEffect->Set_Color(_float4(1.f, 1.f, 1.f, 0.4f));

	FADEDESC tFadeDesc;
	ZeroMemory(&tFadeDesc, sizeof(FADEDESC));
	tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_DISABLE;
	tFadeDesc.eFadeStyle = FADEDESC::FADE_STYLE_DEFAULT;
	tFadeDesc.bFadeInFlag = FADE_NONE;
	tFadeDesc.bFadeOutFlag = FADE_TIME;
	tFadeDesc.fFadeInStartTime = 0.f;
	tFadeDesc.fFadeInTime = 0.3f;
	tFadeDesc.fFadeOutStartTime = 0.f;
	tFadeDesc.fFadeOutTime = 1.f;
	GET_COMPONENT_FROM(m_pOperPointCircleEffect, CFader)->Get_FadeDesc() = tFadeDesc;

	CREATE_GAMEOBJECT(m_pOperPointCircleEffect, RENDER_UI);
	DELETE_GAMEOBJECT(m_pOperPointCircleEffect);

	for (int i = 0; i < 4; ++i)
	{
		m_pArrOperPointCircleEffect[i] = m_pOperPointCircleEffect->Clone();

		CREATE_GAMEOBJECT(m_pArrOperPointCircleEffect[i], RENDER_UI);
		DISABLE_GAMEOBJECT(m_pArrOperPointCircleEffect[i]);
	}

	m_pArrOperPointCircleEffect[0]->Set_PosY(133.f);
	m_pArrOperPointCircleEffect[1]->Set_PosY(133.f);
	m_pArrOperPointCircleEffect[2]->Set_PosY(-161.f);
	m_pArrOperPointCircleEffect[3]->Set_PosY(-161.f);
}

void CUI_HUD::Create_TeamUI()
{
	for (int i = 0; i < TU_End; ++i)
	{
		m_pTeamUI[i] = CUI_Object::Create();

		if (i == TU_Icon)
		{
			GET_COMPONENT_FROM(m_pTeamUI[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pTeamUI[i], "/Oper/Team", "Circle");

			m_pTeamUI[i]->Set_Scale(35.f);
			m_pTeamUI[i]->Set_Sort(0.5f);
		}
		else if (i == TU_Outline)
		{
			m_pTeamUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Oper/PointOutline1.dds"));
			m_pTeamUI[i]->Set_Scale(40.f);
			m_pTeamUI[i]->Set_Sort(0.49f);
		}

		CREATE_GAMEOBJECT(m_pTeamUI[i], RENDER_UI);
		DELETE_GAMEOBJECT(m_pTeamUI[i]);
	}

	for (int i = 0; i < TU_End; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			m_pArrTeamUI[i][j] = m_pTeamUI[i]->Clone();

			if (i == TU_Icon)
			{
				GET_COMPONENT_FROM(m_pArrTeamUI[i][j], CTexture)->Set_CurTextureIndex(j);
			}

			if (i == TU_Outline)
			{
				if (j == 0)
					m_pArrTeamUI[i][j]->Set_Color(_float4(0.f, 0.8f, 1.f, 1.f));
				else if (j == 1)
					m_pArrTeamUI[i][j]->Set_Color(_float4(1.f, 0.2f, 0.f, 1.f));
			}

			CREATE_GAMEOBJECT(m_pArrTeamUI[i][j], RENDER_UI);
			DISABLE_GAMEOBJECT(m_pArrTeamUI[i][j]);
		}
	}

	switch (m_eLoadLevel)
	{
	case Client::LEVEL_PADEN:
	{
		for (int i = 0; i < TU_End; ++i)
		{
			m_pArrTeamUI[i][0]->Set_Pos(-325.f, 127.f);
			m_pArrTeamUI[i][1]->Set_Pos(325.f, 127.f);
		}
	}
	break;
	case Client::LEVEL_HWARA:
	{

	}
	break;
	}
}

void CUI_HUD::Create_OperTimer()
{
	for (int i = 0; i < TT_End; ++i)
	{
		m_pOperTimer[i] = CUI_Object::Create();

		m_pOperTimer[i]->Set_PosY(275.f);
		m_pOperTimer[i]->Set_Scale(242.f, 10.f);

		CREATE_GAMEOBJECT(m_pOperTimer[i], RENDER_UI);
		DISABLE_GAMEOBJECT(m_pOperTimer[i]);
	}

	m_pOperTimer[TT_BG]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/HpBar/T_HPBarBG.png"));
	m_pOperTimer[TT_BG]->Set_Color(_float4(0.f, 0.f, 0.f, 0.5f));
	m_pOperTimer[TT_BG]->Set_Sort(0.49f);

	m_pOperTimer[TT_Bar]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/HpBar/T_HPBarGrey.dds"));
	m_pOperTimer[TT_Bar]->Set_Sort(0.48f);
	m_pOperTimer[TT_Bar]->Set_FontRender(true);
	m_pOperTimer[TT_Bar]->Set_FontStyle(true);
	m_pOperTimer[TT_Bar]->Set_FontOffset(-22.f, -13.f);
	m_pOperTimer[TT_Bar]->Set_FontScale(0.25f);

	GET_COMPONENT_FROM(m_pOperTimer[TT_Bar], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_HorizontalGauge);
}