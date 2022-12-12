#include "CUI_Oper.h"
#include "CUI_Object.h"
#include "GameInstance.h"
#include "CUI_Renderer.h"
#include "Texture.h"
#include "Loading_Manager.h"
#include "CUser.h"
#include "CShader.h"
#include "CGameSystem.h"
#include "CPlayerInfo.h"

#include "CPlayer.h"
#include "CTeamConnector.h"
CUI_Oper::CUI_Oper()
{
}

CUI_Oper::~CUI_Oper()
{
}

HRESULT CUI_Oper::Initialize_Prototype()
{
	m_eLoadLevel = CLoading_Manager::Get_Instance()->Get_LoadLevel();

	ShowCursor(false);

	Create_TextImg();
	Create_OperBG();
	Create_OperProfile();
	Create_CharacterSelect();
	Create_TeamIcon();
	Create_StrongHoldUI();
	Create_StrongHoldEffect();
	Create_OperTimer();
	Create_TargetText();
	Create_RespawnBtn();
	Create_TargetPoint();
	Create_BriefingUI();
	Create_BlackImg();

	Init_CharacterSelect();
	Init_TeamIcon();
	Init_StrongHoldUI();
	Init_StrongHoldEffect();

	return S_OK;
}

HRESULT CUI_Oper::Start()
{
	Bind_Shader();
	Bind_Btn();

	CUser::Get_Instance()->SetActive_HUD(false);

	SetActive_BG(true);

	__super::Start();

	CUser::Get_Instance()->Set_FixCursor(false);
	CUser::Get_Instance()->SetActive_Cursor(true);

	return S_OK;
}

void CUI_Oper::Set_Shader_Smoke(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fSmokeUV, sizeof(_float));
}

void CUI_Oper::Set_Shader_Timer(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fTimerRatio, sizeof(_float));
}

void CUI_Oper::On_PointDown_SelectBG(const _uint& iEventNum)
{
	m_iPrvSelectEventNum = m_iCurSelectEventNum;
	m_iCurSelectEventNum = iEventNum;

	if (m_iPrvSelectEventNum == iEventNum)
		return;

	for (int i = 0; i < CP_End; ++i)
	{
		m_pArrCharacterPort[i][m_iPrvSelectEventNum]->DoScale(-10.f, 0.1f);
		m_pArrCharacterPort[i][iEventNum]->DoScale(10.f, 0.1f);
	}

	CUser::Get_Instance()->Get_MainPlayerInfo()->Set_ChosenClass((CLASS_TYPE)iEventNum);
}

void CUI_Oper::On_PointDown_StrongHoldPoint(const _uint& iEventNum)
{
	// DISABLE_GAMEOBJECT(m_pArrTargetPoint[1]);

	m_bSelectTargetPoint = true;

	_float4 vPos = m_pArrStrongHoldUI[SP_BG][iEventNum]->Get_Pos();
	m_pArrTargetPoint[1]->Set_Pos(vPos.x, vPos.y + 20.f);

	Enable_Fade(m_pArrTargetPoint[1], 0.3f);

	GET_COMPONENT_FROM(m_pBriefingUI[BU_Icon], CTexture)->Set_CurTextureIndex(1);
	m_pBriefingUI[BU_Icon]->Set_Scale(20.f, 15.f);
	m_pBriefingUI[BU_Icon]->Set_Color(_float4(0.f, 0.6f, 0.f, 1.f));
	m_pBriefingUI[BU_Icon]->Set_FontColor(_float4(0.f, 0.6f, 0.f, 1.f));
	m_pBriefingUI[BU_Icon]->Set_FontText(TEXT("목표 설정 완료"));

	CUser::Get_Instance()->Set_TargetPointPos(iEventNum);
}

void CUI_Oper::On_PointDown_RespawnBtn(const _uint& iEventNum)
{
	m_bIsRespawn = false;
	m_iOperRespawn = 0;

	CUser::Get_Instance()->SetActive_PadenUI(true);
	CUser::Get_Instance()->SetActive_HUD(true);

	DISABLE_GAMEOBJECT(this);


	/* 부활 */
	CPlayer* pMainPlayer = CUser::Get_Instance()->Get_MainPlayerInfo()->Get_Player();
	_float4 vStartPos = pMainPlayer->Get_Team()->Find_RespawnPosition_Start();

	pMainPlayer->Respawn_Unit(vStartPos, CUser::Get_Instance()->Get_MainPlayerInfo()->Get_ChonsenClass());

}

void CUI_Oper::My_Tick()
{
	__super::My_Tick();

	if (m_pOperBG[OB_Smoke]->Is_Valid())
	{
		m_fSmokeUV += fDT(0) * 0.01f;
	}

	Progress_Oper();
}

void CUI_Oper::OnEnable()
{
	__super::OnEnable();

	SetActive_BG(true);
}

void CUI_Oper::OnDisable()
{
	__super::OnDisable();

	m_pRespawnBtn->SetActive(false);
	for (auto iter : m_pOperList)
	{
		iter->SetActive(false);
	}
}

void CUI_Oper::SetActive_BG(_bool value)
{
	m_pRealMap->SetActive(value);
	for (int i = 0; i < OB_End; ++i)
	{
		m_pOperBG[i]->SetActive(value);
	}
}

void CUI_Oper::SetActive_Profile(_bool value)
{
	for (int i = 0; i < 4; ++i)
	{
		m_pArrOperProfile[i]->SetActive(value);
	}
}

void CUI_Oper::Progress_Oper()
{
	if (!m_bIsBriefing)
	{
		m_bIsBriefing = true;

		if (m_pRealMap)
		{
			m_pRealMap->DoScale(-3096.f, 0.3f);
		}

		m_pOperBG[OB_BG]->DoScale(-2596.f, 0.3f);
	}

	if (m_bIsRespawn)
	{
		if (m_iOperRespawn == 0)
		{
			m_iOperRespawn++;

			Enable_Fade(m_pRespawnBtn, 0.3f);

			for (int i = 0; i < Team_End; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					Enable_Fade(m_pArrTeamIcon[i][j], 0.3f);
				}
			}

			Enable_StrongHoldUI();
		}
	}
	else
	{
		m_fAccTime += fDT(0);

		if (m_iOperProgress == 0)
		{
			if (m_fAccTime > 0.5f)
			{
				m_fAccTime = 0.f;
				m_iOperProgress++;

				Enable_Fade(m_pOperBG[OB_Black], 0.15f);
			}
		}
		else if (m_iOperProgress == 1)
		{
			if (m_fAccTime > 0.3f)
			{
				m_fAccTime = 0.f;
				m_iOperProgress++;

				Enable_Fade(m_pTextImg[Text_Oper1], 0.3f);
				m_pTextImg[Text_Oper1]->DoScale(-512.f, 0.3f);
			}
		}
		else if (m_iOperProgress == 2)
		{
			if (m_fAccTime > 1.f)
			{
				m_fAccTime = 0.f;
				m_iOperProgress++;

				_float4 vPos = m_pTextImg[Text_Oper1]->Get_Pos();
				vPos.y += 200.f;
				m_pTextImg[Text_Oper1]->DoMove(vPos, 0.3f, 0);
				m_pTextImg[Text_Oper1]->DoScale(-256.f, 0.3f);

				for (int i = 0; i < 4; ++i)
				{
					Enable_Fade(m_pArrOperProfile[i], 0.3f);
				}
			}
		}
		else if (m_iOperProgress == 3)
		{
			if (m_fAccTime > 3.f)
			{
				m_fAccTime = 0.f;
				m_iOperProgress++;

				Disable_Fade(m_pOperBG[OB_Black], 0.3f);
				Disable_Fade(m_pTextImg[Text_Oper1], 0.3f);

				for (int i = 0; i < 4; ++i)
				{
					Disable_Fade(m_pArrOperProfile[i], 0.3f);
				}
			}
		}
		else if (m_iOperProgress == 4)
		{
			m_iOperProgress++;

			for (int i = 0; i < Team_End; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					Enable_Fade(m_pArrTeamIcon[i][j], 0.3f);
				}
			}

			Enable_StrongHoldUI();
		}
		else if (m_iOperProgress == 5)
		{
			m_iOperProgress++;

			for (int i = 0; i < 3; ++i)
			{
				Enable_Fade(m_pArrStrongHoldEffect[i], 1.f);
				m_pArrStrongHoldEffect[i]->DoScale(70.f, 1.f);
			}
		}
		else if (m_iOperProgress == 6)
		{
			if (m_fAccTime > 0.5f)
			{
				m_fAccTime = 0.f;
				m_iOperProgress++;

				for (int i = 3; i < 6; ++i)
				{
					Enable_Fade(m_pArrStrongHoldEffect[i], 1.f);
					m_pArrStrongHoldEffect[i]->DoScale(70.f, 1.f);
				}
			}
		}
		else if (m_iOperProgress == 7)
		{
			if (m_fAccTime > 1.f)
			{
				m_fAccTime = 0.f;
				m_iOperProgress++;

				_float fDuration = 0.3f;
				for (int i = 0; i < 2; ++i)
				{
					Enable_Fade(m_pArrCharacterSideBG[i], fDuration);
				}

				_float4 vPos0 = m_pArrCharacterSideBG[0]->Get_Pos();
				vPos0.x += 50.f;
				m_pArrCharacterSideBG[0]->DoMove(vPos0, fDuration, 0);

				_float4 vPos1 = m_pArrCharacterSideBG[1]->Get_Pos();
				vPos1.x -= 50.f;
				m_pArrCharacterSideBG[1]->DoMove(vPos1, fDuration, 0);

				for (int i = 0; i < CP_End; ++i)
				{
					for (int j = 0; j < 6; ++j)
					{
						Enable_Fade(m_pArrCharacterPort[i][j], fDuration);
						_float4 vPos = m_pArrCharacterPort[i][j]->Get_Pos();
						vPos.x += 50.f;
						m_pArrCharacterPort[i][j]->DoMove(vPos, fDuration, 0);
					}
				}

				for (int i = 0; i < CP_End; ++i)
				{
					m_pArrCharacterPort[i][0]->DoScale(10.f, fDuration);
				}

				Enable_Fade(m_pTextImg[Text_Oper2], fDuration);
				Enable_Fade(m_pTextImg[Text_SelectPoint], fDuration);

				for (int i = 0; i < TU_End; ++i)
				{
					m_fOperTime = m_fMaxOperTime;
					Enable_Fade(m_pTimer[i], fDuration);
				}

				Enable_Fade(m_pArrTargetPoint[0], fDuration);

				//Enable_Fade(m_pOperMapIcon, fDuration);
				//Enable_Fade(m_pOperMapBG, fDuration);

				for (int i = 0; i < BU_End; ++i)
				{
					Enable_Fade(m_pBriefingUI[i], fDuration);
				}
			}
		}
		else if (m_iOperProgress == 8)
		{
			_tchar  szTemp[MAX_STR] = {};
			swprintf_s(szTemp, TEXT("%04.1f"), m_fOperTime);
			m_pTimer[TU_Bar]->Set_FontText(szTemp);

			m_fTimerRatio = m_fOperTime / m_fMaxOperTime;
			m_fOperTime -= fDT(0);
			if (m_fOperTime < 0.f)
			{
				m_fOperTime = 0.f;
				m_iOperProgress++;
			}
		}
		else if (m_iOperProgress == 9)
		{
			m_fAccTime = 0.f;
			m_iOperProgress++;

			Enable_Fade(m_pBlackImg, 0.3f);
		}
		else if (m_iOperProgress == 10)
		{
			if (m_fAccTime > 0.3f)
			{
				m_fAccTime = 0.f;
				m_iOperProgress++;

				for (auto iter : m_pOperList)
				{
					iter->SetActive(false);
				}

				switch (m_eLoadLevel)
				{
				case Client::LEVEL_PADEN:
					CGameSystem::Get_Instance()->On_StartGame();
					break;
				case Client::LEVEL_HWARA:
					CGameSystem::Get_Instance()->On_StartGame();
					break;
				}

				CUser::Get_Instance()->SetActive_PadenUI(true);
				CUser::Get_Instance()->SetActive_HUD(true);
			}
		}
		else if (m_iOperProgress == 11)
		{
			m_iOperProgress++;
			Disable_Fade(m_pBlackImg, 0.3f);
		}
		else if (m_iOperProgress == 12)
		{
			if (m_fAccTime > 0.3f)
			{
				m_fAccTime = 0.f;

				DISABLE_GAMEOBJECT(this);

				CUser::Get_Instance()->SetActive_TargetPoint(true);

				CUser::Get_Instance()->Fix_CursorPosToCenter();
			}
		}
	}
}

void CUI_Oper::Enable_StrongHoldUI()
{
	for (int i = 0; i < 3; ++i)
	{
		m_pArrStrongHoldUI[SP_BG][i]->Set_Scale(115.f);
		m_pArrStrongHoldUI[SP_Outline][i]->Set_Scale(120.f);
		m_pArrStrongHoldUI[SP_Icon][i]->Set_Scale(115.f);
		m_pArrStrongHoldUI[SP_TEXT][i]->Set_Scale(140.f);
	}

	for (int i = 0; i < SP_End; ++i)
	{
		switch (m_eLoadLevel)
		{
		case LEVEL_PADEN:

			for (int j = 0; j < 3; ++j)
			{
				Enable_Fade(m_pArrStrongHoldUI[i][j], 0.3f);
				m_pArrStrongHoldUI[i][j]->DoScale(-70.f, 0.3f);
			}

			DISABLE_GAMEOBJECT(m_pArrStrongHoldUI[SP_Icon][0]);
			DISABLE_GAMEOBJECT(m_pArrStrongHoldUI[SP_Icon][2]);

			break;

		case LEVEL_HWARA:
			break;
		}
	}
}

void CUI_Oper::Create_TextImg()
{
	for (int i = 0; i < Text_End; ++i)
	{
		m_pTextImg[i] = CUI_Object::Create();

		m_pTextImg[i]->Set_FadeDesc(0.3f);

		if (i == Text_Oper1)
		{
			m_pTextImg[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Oper/OperMeeting.png"));

			m_pTextImg[i]->Set_PosY(50.f);
			m_pTextImg[i]->Set_Scale(1024.f);
			m_pTextImg[i]->Set_Sort(0.49f);
		}
		else if (i == Text_Oper2)
		{
			m_pTextImg[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Oper/OperMeeting2.png"));

			m_pTextImg[i]->Set_PosY(305.f);
			m_pTextImg[i]->Set_Scale(155.f, 50.f);
			m_pTextImg[i]->Set_Sort(0.49f);
		}
		else if (i == Text_SelectPoint)
		{
			m_pTextImg[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Oper/OperText.png"));

			m_pTextImg[i]->Set_PosY(-250.f);
			m_pTextImg[i]->Set_Scale(287.f, 50.f);
			m_pTextImg[i]->Set_Sort(0.49f);
		}

		m_pOperList.push_back(m_pTextImg[i]);

		CREATE_GAMEOBJECT(m_pTextImg[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pTextImg[i]);
	}
}

void CUI_Oper::Create_TargetText()
{
	for (int i = 0; i < TargetText_End; ++i)
	{
		m_pTargetText[i] = CUI_Object::Create();

		m_pTargetText[i]->Set_FadeDesc(0.3f);

		if (i == TargetText_BG)
		{
			m_pTargetText[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Oper/Briefing/T_BriefingBox.dds"));

			m_pTargetText[i]->Set_PosY(245.f);
			m_pTargetText[i]->Set_Scale(160.f, 25.f);
			m_pTargetText[i]->Set_Color(_float4(0.f, 0.f, 0.f, 0.6f));
			m_pTargetText[i]->Set_Sort(0.5f);
		}
		else if (i == TargetText_Icon)
		{
			GET_COMPONENT_FROM(m_pTargetText[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pTargetText[i], "/Oper/Briefing", "Icon");

			m_pTargetText[i]->Set_Sort(0.49f);
			m_pTargetText[i]->Set_Pos(-45.f, 246.f);
			m_pTargetText[i]->Set_Scale(32.f);
			m_pTargetText[i]->Set_Color(_float4(0.6f, 0.6f, 0.6f, 1.f));

			m_pTargetText[i]->Set_FontRender(true);
			m_pTargetText[i]->Set_FontStyle(true);
			m_pTargetText[i]->Set_FontOffset(10.f, -10.f);
			m_pTargetText[i]->Set_FontScale(0.2f);
			m_pTargetText[i]->Set_FontColor(_float4(_float4(0.6f, 0.6f, 0.6f, 1.f)));
			m_pTargetText[i]->Set_FontText(TEXT("공격 목표 없음"));
		}

		m_pOperList.push_back(m_pTargetText[i]);

		CREATE_GAMEOBJECT(m_pTargetText[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pTargetText[i]);
	}
}

void CUI_Oper::Create_OperBG()
{
	switch (m_eLoadLevel)
	{
	case Client::LEVEL_PADEN:

		m_pRealMap = CUI_Object::Create();

		m_pRealMap->Set_Scale(4096.f);
		m_pRealMap->Set_PosY(45.f);
		m_pRealMap->Set_Sort(0.515f);
		m_pRealMap->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Map/T_MinimapPaden.dds"));

		m_pOperList.push_back(m_pRealMap);

		CREATE_GAMEOBJECT(m_pRealMap, GROUP_UI);
		DISABLE_GAMEOBJECT(m_pRealMap);

		break;
	}

	for (int i = 0; i < OB_End; ++i)
	{
		m_pOperBG[i] = CUI_Object::Create();

		if (i == OB_BG)
		{
			m_pOperBG[i]->Set_Scale(4096.f);
			m_pOperBG[i]->Set_Sort(0.52f);

			switch (m_eLoadLevel)
			{
			case LEVEL_TEST:
				m_pOperBG[i]->Set_PosY(205.f);
				m_pOperBG[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Map/T_MinimapPaden.dds"));


				break;

			case Client::LEVEL_PADEN:
				m_pOperBG[i]->Set_PosY(205.f);
				m_pOperBG[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Map/T_MinimapPaden.dds"));
				break;
			case Client::LEVEL_HWARA:
				m_pOperBG[i]->Set_PosY(205.f);
				m_pOperBG[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Map/T_MinimapHwara.dds"));
				break;
			}
		}
		else if (i == OB_Smoke)
		{
			GET_COMPONENT_FROM(m_pOperBG[i], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_OperSmoke);

			m_pOperBG[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Oper/T_soft_smoke.dds"));
			m_pOperBG[i]->SetTexture(TEXT("../Bin/Resources/Textures/UI/Oper/T_Noise_02.dds"));

			m_pOperBG[i]->Set_Scale(4096.f);
			m_pOperBG[i]->Set_Sort(0.51f);
		}
		else if (i == OB_Black)
		{
			m_pOperBG[i]->Set_FadeDesc(0.15f, 0.3f);

			m_pOperBG[i]->Set_Texture(TEXT("../Bin/Resources/Textures/Black.png"));

			m_pOperBG[i]->Set_Scale(1280.f);
			m_pOperBG[i]->Set_Sort(0.5f);
			m_pOperBG[i]->Set_Color(_float4(1.f, 1.f, 1.f, 0.9f));
		}

		m_pOperList.push_back(m_pOperBG[i]);

		CREATE_GAMEOBJECT(m_pOperBG[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pOperBG[i]);
	}
}

void CUI_Oper::Create_OperProfile()
{
	m_pOperProfile = CUI_Object::Create();

	m_pOperProfile->Set_FadeDesc(0.5f, 0.3f);

	// 각 캐릭터에 해당하는 이미지 파일 불러오기
	m_pOperProfile->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Profile/T_ProfileCardDefaultArcher.dds"));

	m_pOperProfile->Set_Scale(156.f, 350.f);
	m_pOperProfile->Set_Sort(0.49f);

	CREATE_GAMEOBJECT(m_pOperProfile, GROUP_UI);
	DELETE_GAMEOBJECT(m_pOperProfile);

	for (int i = 0; i < 4; ++i)
	{
		m_pArrOperProfile[i] = m_pOperProfile->Clone();

		_float fPosX = -375.f + (i * 250.f);
		m_pArrOperProfile[i]->Set_PosX(fPosX);

		m_pOperList.push_back(m_pArrOperProfile[i]);

		CREATE_GAMEOBJECT(m_pArrOperProfile[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pArrOperProfile[i]);
	}
}

void CUI_Oper::Create_CharacterSelect()
{
	m_CharacterSideBG = CUI_Object::Create();

	m_CharacterSideBG->Set_FadeDesc(0.3f);

	m_CharacterSideBG->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Oper/T_ScrollBG.png"));
	m_CharacterSideBG->Set_Scale(250.f, 753.f);
	m_CharacterSideBG->Set_Sort(0.49f);

	CREATE_GAMEOBJECT(m_CharacterSideBG, GROUP_UI);
	DELETE_GAMEOBJECT(m_CharacterSideBG);

	for (int i = 0; i < 2; ++i)
	{
		m_pArrCharacterSideBG[i] = m_CharacterSideBG->Clone();

		_float fPosX = -565 + (i * 1130.f);
		m_pArrCharacterSideBG[i]->Set_PosX(fPosX);

		if (i == 1)
		{
			m_pArrCharacterSideBG[i]->Set_RotationZ(180.f);
		}

		m_pOperList.push_back(m_pArrCharacterSideBG[i]);

		CREATE_GAMEOBJECT(m_pArrCharacterSideBG[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pArrCharacterSideBG[i]);
	}

	for (int i = 0; i < CP_End; ++i)
	{
		m_pCharacterPort[i] = CUI_Object::Create();

		m_pCharacterPort[i]->Set_FadeDesc(0.3f);

		if (i == CP_PortBG)
		{
			m_pCharacterPort[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Portrait/T_RoundPortraitBG.dds"));

			m_pCharacterPort[i]->Set_Scale(65.f);
			m_pCharacterPort[i]->Set_Sort(0.48f);
		}
		else if (i == CP_Port)
		{
			GET_COMPONENT_FROM(m_pCharacterPort[CP_Port], CTexture)->Remove_Texture(0);
			Read_Texture(m_pCharacterPort[CP_Port], "/HUD/Portrait", "Class");

			m_pCharacterPort[CP_Port]->Set_Scale(65.f);
			m_pCharacterPort[CP_Port]->Set_Sort(0.47f);
		}
		else if (i == CP_SelectBG)
		{
			m_pCharacterPort[CP_SelectBG]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Oper/T_SelectedBG.png"));

			m_pCharacterPort[CP_SelectBG]->Set_Scale(213.f, 65.f);
			m_pCharacterPort[CP_SelectBG]->Set_Sort(0.49f);
			m_pCharacterPort[CP_SelectBG]->Set_MouseTarget(true);
		}
		else if (i == CP_Class)
		{
			GET_COMPONENT_FROM(m_pCharacterPort[CP_Class], CTexture)->Remove_Texture(0);
			Read_Texture(m_pCharacterPort[CP_Class], "/Oper", "Class");

			m_pCharacterPort[CP_Class]->Set_Scale(32.f);
			m_pCharacterPort[CP_Class]->Set_Sort(0.48f);

			m_pCharacterPort[CP_Class]->Set_FontRender(true);
			m_pCharacterPort[CP_Class]->Set_FontStyle(true);
			m_pCharacterPort[CP_Class]->Set_FontOffset(20.f, -15.f);
			m_pCharacterPort[CP_Class]->Set_FontScale(0.3f);
		}

		CREATE_GAMEOBJECT(m_pCharacterPort[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_pCharacterPort[i]);

		for (int j = 0; j < 6; ++j)
		{
			m_pArrCharacterPort[i][j] = m_pCharacterPort[i]->Clone();

			if (i == CP_Port)
			{
				GET_COMPONENT_FROM(m_pArrCharacterPort[CP_Port][j], CTexture)->Set_CurTextureIndex(j);
			}
			else if (i == CP_Class)
			{
				GET_COMPONENT_FROM(m_pArrCharacterPort[CP_Class][j], CTexture)->Set_CurTextureIndex(j);
			}

			m_pOperList.push_back(m_pArrCharacterPort[i][j]);

			CREATE_GAMEOBJECT(m_pArrCharacterPort[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrCharacterPort[i][j]);
		}
	}
}

void CUI_Oper::Init_CharacterSelect()
{
	_float fTopPosY = 250.f;
	_float fMidPosY = 150.f;
	_float fBotPosY = 50.f;

	_float fTopPosXSelectBG = -480.f;
	_float fMidPosXSelectBG = -515.f;
	_float fBotPosXSelectBG = -530.f;

	m_pArrCharacterPort[CP_SelectBG][0]->Set_Pos(fTopPosXSelectBG, fTopPosY);
	m_pArrCharacterPort[CP_SelectBG][1]->Set_Pos(fMidPosXSelectBG, fMidPosY);
	m_pArrCharacterPort[CP_SelectBG][2]->Set_Pos(fBotPosXSelectBG, fBotPosY);
	m_pArrCharacterPort[CP_SelectBG][3]->Set_Pos(fBotPosXSelectBG, -fBotPosY);
	m_pArrCharacterPort[CP_SelectBG][4]->Set_Pos(fMidPosXSelectBG, -fMidPosY);
	m_pArrCharacterPort[CP_SelectBG][5]->Set_Pos(fTopPosXSelectBG, -fTopPosY);

	_float fTopPosCharX = -555.f;
	_float fMidPosCharX = -590.f;
	_float fBotPosCharX = -605.f;

	m_pArrCharacterPort[CP_PortBG][0]->Set_Pos(fTopPosCharX, fTopPosY);
	m_pArrCharacterPort[CP_PortBG][1]->Set_Pos(fMidPosCharX, fMidPosY);
	m_pArrCharacterPort[CP_PortBG][2]->Set_Pos(fBotPosCharX, fBotPosY);
	m_pArrCharacterPort[CP_PortBG][3]->Set_Pos(fBotPosCharX, -fBotPosY);
	m_pArrCharacterPort[CP_PortBG][4]->Set_Pos(fMidPosCharX, -fMidPosY);
	m_pArrCharacterPort[CP_PortBG][5]->Set_Pos(fTopPosCharX, -fTopPosY);

	m_pArrCharacterPort[CP_Port][0]->Set_Pos(fTopPosCharX, fTopPosY);
	m_pArrCharacterPort[CP_Port][1]->Set_Pos(fMidPosCharX, fMidPosY);
	m_pArrCharacterPort[CP_Port][2]->Set_Pos(fBotPosCharX, fBotPosY);
	m_pArrCharacterPort[CP_Port][3]->Set_Pos(fBotPosCharX, -fBotPosY);
	m_pArrCharacterPort[CP_Port][4]->Set_Pos(fMidPosCharX, -fMidPosY);
	m_pArrCharacterPort[CP_Port][5]->Set_Pos(fTopPosCharX, -fTopPosY);

	_float fTopPosIconX = -505.f;
	_float fMidPosIconX = -540.f;
	_float fBotPosIconX = -555.f;

	m_pArrCharacterPort[CP_Class][0]->Set_FontText(TEXT("블레이드"));
	m_pArrCharacterPort[CP_Class][0]->Set_Pos(fTopPosIconX, fTopPosY);

	m_pArrCharacterPort[CP_Class][1]->Set_FontText(TEXT("스파이크"));
	m_pArrCharacterPort[CP_Class][1]->Set_Pos(fMidPosIconX, fMidPosY);

	m_pArrCharacterPort[CP_Class][2]->Set_FontText(TEXT("아치"));
	m_pArrCharacterPort[CP_Class][2]->Set_Pos(fBotPosIconX, fBotPosY);

	m_pArrCharacterPort[CP_Class][3]->Set_FontText(TEXT("가디언"));
	m_pArrCharacterPort[CP_Class][3]->Set_Pos(fBotPosIconX, -fBotPosY);

	m_pArrCharacterPort[CP_Class][4]->Set_FontText(TEXT("스모크"));
	m_pArrCharacterPort[CP_Class][4]->Set_Pos(fMidPosIconX, -fMidPosY);

	m_pArrCharacterPort[CP_Class][5]->Set_FontText(TEXT("워해머"));
	m_pArrCharacterPort[CP_Class][5]->Set_Pos(fTopPosIconX, -fTopPosY);
}

void CUI_Oper::Create_TeamIcon()
{
	for (int i = 0; i < Team_End; ++i)
	{
		m_pTeamIcon[i] = CUI_Object::Create();

		m_pTeamIcon[i]->Set_FadeDesc(0.3f);

		if (i == Team_Icon)
		{
			GET_COMPONENT_FROM(m_pTeamIcon[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pTeamIcon[i], "/Oper/Team", "Circle");

			m_pTeamIcon[i]->Set_Scale(35.f);
			m_pTeamIcon[i]->Set_Sort(0.5f);
		}
		else if (i == Team_Outline)
		{
			m_pTeamIcon[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Oper/PointOutline1.dds"));

			m_pTeamIcon[i]->Set_Scale(40.f);
			m_pTeamIcon[i]->Set_Sort(0.49f);
		}

		CREATE_GAMEOBJECT(m_pTeamIcon[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_pTeamIcon[i]);

		for (int j = 0; j < 2; ++j)
		{
			m_pArrTeamIcon[i][j] = m_pTeamIcon[i]->Clone();

			if (i == Team_Icon)
			{
				GET_COMPONENT_FROM(m_pArrTeamIcon[i][j], CTexture)->Set_CurTextureIndex(j);
			}
			else if (i == Team_Outline)
			{
				if (j == 0)
				{
					m_pArrTeamIcon[i][j]->Set_Color(m_vColorBlue);
				}
				else if (j == 1)
				{
					m_pArrTeamIcon[i][j]->Set_Color(m_vColorRed);
				}
			}

			m_pOperList.push_back(m_pArrTeamIcon[i][j]);

			CREATE_GAMEOBJECT(m_pArrTeamIcon[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrTeamIcon[i][j]);
		}
	}
}

void CUI_Oper::Init_TeamIcon()
{
	for (int i = 0; i < Team_End; ++i)
	{
		switch (m_eLoadLevel)
		{
		case LEVEL_TEST:

			m_pArrTeamIcon[i][0]->Set_Pos(-325.f, 127.f);
			m_pArrTeamIcon[i][1]->Set_Pos(325.f, 127.f);

			break;

		case Client::LEVEL_PADEN:

			m_pArrTeamIcon[i][0]->Set_Pos(-325.f, 127.f);
			m_pArrTeamIcon[i][1]->Set_Pos(325.f, 127.f);

			break;
		case Client::LEVEL_HWARA:
			break;
		}
	}
}

void CUI_Oper::Create_StrongHoldUI()
{
	for (int i = 0; i < SP_End; ++i)
	{
		m_pStrongHoldUI[i] = CUI_Object::Create();

		m_pStrongHoldUI[i]->Set_FadeDesc(0.3f);

		if (i == SP_BG)
		{
			GET_COMPONENT_FROM(m_pStrongHoldUI[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pStrongHoldUI[i], "/Oper", "PointGauge");

			m_pStrongHoldUI[i]->Set_Scale(50.f);
			m_pStrongHoldUI[i]->Set_Sort(0.49f);
			m_pStrongHoldUI[i]->Set_Color(_float4(0.4f, 0.4f, 0.4f, 1.f));
		}
		else if (i == SP_Outline)
		{
			GET_COMPONENT_FROM(m_pStrongHoldUI[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pStrongHoldUI[i], "/Oper", "PointOutline");

			m_pStrongHoldUI[i]->Set_Scale(120.f);
			m_pStrongHoldUI[i]->Set_Sort(0.48f);
			m_pStrongHoldUI[i]->Set_Color(_float4(0.9f, 0.9f, 0.9f, 0.5f));
			m_pStrongHoldUI[i]->Set_MouseTarget(true);
		}
		else if (i == SP_Icon)
		{
			GET_COMPONENT_FROM(m_pStrongHoldUI[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pStrongHoldUI[i], "/Oper", "PointIcon");

			m_pStrongHoldUI[i]->Set_Scale(115.f);
			m_pStrongHoldUI[i]->Set_Sort(0.485f);
			m_pStrongHoldUI[i]->Set_Color(_float4(0.7f, 0.7f, 0.7f, 1.f));
		}
		else if (i == SP_TEXT)
		{
			GET_COMPONENT_FROM(m_pStrongHoldUI[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pStrongHoldUI[i], "/Oper", "PointText");

			m_pStrongHoldUI[i]->Set_Scale(140.f);
			m_pStrongHoldUI[i]->Set_Sort(0.48f);
		}

		CREATE_GAMEOBJECT(m_pStrongHoldUI[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_pStrongHoldUI[i]);

		for (int j = 0; j < 3; ++j)
		{
			m_pArrStrongHoldUI[i][j] = m_pStrongHoldUI[i]->Clone();

			m_pOperList.push_back(m_pArrStrongHoldUI[i][j]);

			CREATE_GAMEOBJECT(m_pArrStrongHoldUI[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrStrongHoldUI[i][j]);
		}
	}
}

void CUI_Oper::Create_StrongHoldEffect()
{
	m_pStrongHoldEffect = CUI_Object::Create();

	m_pStrongHoldEffect->Set_FadeDesc(0.3f, 1.f, true);

	m_pStrongHoldEffect->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Circle/T_256Circle.dds"));
	m_pStrongHoldEffect->Set_PosY(105.f);
	m_pStrongHoldEffect->Set_Scale(30.f);
	m_pStrongHoldEffect->Set_Sort(0.495f);
	m_pStrongHoldEffect->Set_Color(_float4(1.f, 1.f, 1.f, 0.4f));

	CREATE_GAMEOBJECT(m_pStrongHoldEffect, RENDER_UI);
	DELETE_GAMEOBJECT(m_pStrongHoldEffect);

	for (int i = 0; i < 6; ++i)
	{
		m_pArrStrongHoldEffect[i] = m_pStrongHoldEffect->Clone();

		m_pOperList.push_back(m_pArrStrongHoldEffect[i]);

		CREATE_GAMEOBJECT(m_pArrStrongHoldEffect[i], RENDER_UI);
		DISABLE_GAMEOBJECT(m_pArrStrongHoldEffect[i]);
	}
}

void CUI_Oper::Init_StrongHoldUI()
{
	switch (m_eLoadLevel)
	{
	case LEVEL_PADEN:

		m_pArrStrongHoldUI[SP_BG][0]->Set_PosY(-4.f);
		m_pArrStrongHoldUI[SP_Outline][0]->Set_PosY(-3.f);
		m_pArrStrongHoldUI[SP_TEXT][0]->Set_PosY(-8.f);

		m_pArrStrongHoldUI[SP_BG][1]->Set_PosY(-195.f);
		m_pArrStrongHoldUI[SP_Outline][1]->Set_PosY(-195.f);
		m_pArrStrongHoldUI[SP_Icon][1]->Set_PosY(-195.f);
		m_pArrStrongHoldUI[SP_TEXT][1]->Set_PosY(-200.f);

		m_pArrStrongHoldUI[SP_BG][2]->Set_PosY(170.f);
		m_pArrStrongHoldUI[SP_Outline][2]->Set_PosY(170.f);
		m_pArrStrongHoldUI[SP_Icon][2]->Set_PosY(170.f);
		m_pArrStrongHoldUI[SP_TEXT][2]->Set_PosY(166.f);

		for (int i = 0; i < SP_End; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				GET_COMPONENT_FROM(m_pArrStrongHoldUI[i][j], CTexture)->Set_CurTextureIndex(j);
			}
		}

		break;

	case LEVEL_HWARA:
		break;
	}
}

void CUI_Oper::Init_StrongHoldEffect()
{
	switch (m_eLoadLevel)
	{
	case LEVEL_PADEN:
		m_pArrStrongHoldEffect[0]->Set_PosY(-4.f);
		m_pArrStrongHoldEffect[1]->Set_PosY(-195.f);
		m_pArrStrongHoldEffect[2]->Set_PosY(170.f);
		m_pArrStrongHoldEffect[3]->Set_PosY(-4.f);
		m_pArrStrongHoldEffect[4]->Set_PosY(-195.f);
		m_pArrStrongHoldEffect[5]->Set_PosY(170.f);

		break;

	case LEVEL_HWARA:
		break;
	}
}

void CUI_Oper::Create_RespawnBtn()
{
	m_pRespawnBtn = CUI_Object::Create();

	m_pRespawnBtn->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Oper/T_TimeAnnounceBox.png"));

	m_pRespawnBtn->Set_FadeDesc(0.3f);

	m_pRespawnBtn->Set_MouseTarget(true);

	m_pRespawnBtn->Set_Sort(0.50f);
	m_pRespawnBtn->Set_PosY(-275.f);
	m_pRespawnBtn->Set_Scale(200.f, 60.f);

	m_pRespawnBtn->Set_FontRender(true);
	m_pRespawnBtn->Set_FontStyle(true);
	m_pRespawnBtn->Set_FontCenter(true);
	m_pRespawnBtn->Set_FontOffset(5.f, 3.f);
	m_pRespawnBtn->Set_FontScale(0.25f);
	m_pRespawnBtn->Set_FontText(TEXT("선택한 거점에서 합류"));
	m_pRespawnBtn->Set_FontColor(_float4(0.f, 0.f, 0.f, 1.f));

	CREATE_GAMEOBJECT(m_pRespawnBtn, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pRespawnBtn);
}

void CUI_Oper::Create_OperTimer()
{
	for (int i = 0; i < TU_End; ++i)
	{
		m_pTimer[i] = CUI_Object::Create();

		m_pTimer[i]->Set_FadeDesc(0.3f);

		m_pTimer[i]->Set_PosY(275.f);
		m_pTimer[i]->Set_Scale(242.f, 10.f);

		if (i == TU_BG)
		{
			m_pTimer[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/HpBar/T_HPBarBG.png"));
			m_pTimer[i]->Set_Color(_float4(0.f, 0.f, 0.f, 0.5f));
			m_pTimer[i]->Set_Sort(0.49f);
		}
		else if (TU_Bar)
		{
			GET_COMPONENT_FROM(m_pTimer[i], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_HorizontalGauge);

			m_pTimer[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/HpBar/T_HPBarGrey.dds"));

			m_pTimer[i]->Set_Sort(0.48f);
			m_pTimer[i]->Set_FontRender(true);
			m_pTimer[i]->Set_FontStyle(true);
			m_pTimer[i]->Set_FontOffset(-22.f, -13.f);
			m_pTimer[i]->Set_FontScale(0.25f);
		}

		m_pOperList.push_back(m_pTimer[i]);

		CREATE_GAMEOBJECT(m_pTimer[i], RENDER_UI);
		DISABLE_GAMEOBJECT(m_pTimer[i]);
	}
}

void CUI_Oper::Create_BlackImg()
{
	m_pBlackImg = CUI_Object::Create();
	m_pBlackImg->Set_Texture(TEXT("../Bin/Resources/Textures/Black.png"));
	m_pBlackImg->Set_Sort(0.f);
	m_pBlackImg->Set_Scale(1280.f, 720.f);
	m_pBlackImg->Set_FadeDesc(0.3f);

	CREATE_GAMEOBJECT(m_pBlackImg, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pBlackImg);
}

void CUI_Oper::Create_TargetPoint()
{
	m_pTargetPoint = CUI_Object::Create();

	m_pTargetPoint->Set_FadeDesc(0.3f);

	m_pTargetPoint->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Oper/T_PingGreenLeader.dds"));
	m_pTargetPoint->Set_Pos(-45.f, -300.f);
	m_pTargetPoint->Set_Sort(0.47f);

	CREATE_GAMEOBJECT(m_pTargetPoint, GROUP_UI);
	DELETE_GAMEOBJECT(m_pTargetPoint);


	for (int i = 0; i < 2; ++i)
	{
		m_pArrTargetPoint[i] = m_pTargetPoint->Clone();

		if (i == 0)
		{
			m_pArrTargetPoint[0]->Set_Scale(30.f);
			m_pArrTargetPoint[0]->Set_FontRender(true);
			m_pArrTargetPoint[0]->Set_FontStyle(true);
			m_pArrTargetPoint[0]->Set_FontOffset(20.f, -13.f);
			m_pArrTargetPoint[0]->Set_FontScale(0.25f);

			m_pArrTargetPoint[0]->Set_FontText(TEXT("공격 목표"));
		}
		else if (i == 1)
		{
			m_pArrTargetPoint[1]->Set_Scale(32.f);
		}

		m_pOperList.push_back(m_pArrTargetPoint[i]);

		CREATE_GAMEOBJECT(m_pArrTargetPoint[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pArrTargetPoint[i]);
	}
}

void CUI_Oper::Create_BriefingUI()
{
	for (int i = 0; i < BU_End; ++i)
	{
		m_pBriefingUI[i] = CUI_Object::Create();

		m_pOperList.push_back(m_pBriefingUI[i]);

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

void CUI_Oper::Bind_Shader()
{
	GET_COMPONENT_FROM(m_pOperBG[OB_Smoke], CShader)->CallBack_SetRawValues += bind(&CUI_Oper::Set_Shader_Smoke, this, placeholders::_1, "g_fValue");
	GET_COMPONENT_FROM(m_pTimer[TU_Bar], CShader)->CallBack_SetRawValues += bind(&CUI_Oper::Set_Shader_Timer, this, placeholders::_1, "g_fValue");
}

void CUI_Oper::Bind_Btn()
{
	for (int i = 0; i < 6; ++i)
	{
		m_pArrCharacterPort[CP_SelectBG][i]->CallBack_PointDown += bind(&CUI_Oper::On_PointDown_SelectBG, this, i);
	}

	for (int i = 0; i < 3; ++i)
	{
		m_pArrStrongHoldUI[SP_Outline][i]->CallBack_PointDown += bind(&CUI_Oper::On_PointDown_StrongHoldPoint, this, i);
	}

	m_pRespawnBtn->CallBack_PointDown += bind(&CUI_Oper::On_PointDown_RespawnBtn, this, 0);
}
