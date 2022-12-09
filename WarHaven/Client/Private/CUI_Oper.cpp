#include "CUI_Oper.h"
#include "CUI_Object.h"
#include "GameInstance.h"
#include "CUI_Renderer.h"
#include "Texture.h"
#include "Loading_Manager.h"
#include "CUser.h"

CUI_Oper::CUI_Oper()
{
}

CUI_Oper::~CUI_Oper()
{
}

HRESULT CUI_Oper::Initialize_Prototype()
{
	m_eLoadLevel = CLoading_Manager::Get_Instance()->Get_LoadLevel();

	Create_TextImg();
	Create_OperBG();
	Create_OperProfile();
	Create_OperCharacterSelect();
	Create_TeamIcon();
	Create_StrongHoldUI();
	Create_StrongHoldEffect();

	return S_OK;
}

HRESULT CUI_Oper::Start()
{
	Init_OperCharacterSelect();
	Init_TeamIcon();
	Init_StrongHoldUI();
	StrongHoldEffect();

	CUser::Get_Instance()->SetActive_HUD(false);

	SetActive_OperBG(true);

	return S_OK;
}

void CUI_Oper::My_Tick()
{
	__super::My_Tick();

	Progress_Oper();
}

void CUI_Oper::OnEnable()
{
	__super::OnEnable();
}

void CUI_Oper::OnDisable()
{
	__super::OnDisable();

	SetActive_OperBG(false);
	SetActive_OperProfile(false);
}

void CUI_Oper::SetActive_OperBG(_bool value)
{
	for (int i = 0; i < OB_End; ++i)
	{
		m_pOperBG[i]->SetActive(value);
	}
}

void CUI_Oper::SetActive_OperProfile(_bool value)
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
		m_pOperBG[OB_BG]->DoScale(-2596.f, 0.3f);
	}
	else
	{
		m_fAccTime += fDT(0);

		switch (m_iOperProgress)
		{
		case 0:

			if (m_fAccTime > 0.5f)
			{
				m_fAccTime = 0.f;
				m_iOperProgress++;

				Enable_Fade(m_pOperBG[OB_Black], 0.15f);
			}

			break;

		case 1:

			if (m_fAccTime > 0.3f)
			{
				m_fAccTime = 0.f;
				m_iOperProgress++;

				Enable_Fade(m_pTextImg[Text_Oper1], 0.3f);
				m_pTextImg[Text_Oper1]->DoScale(-512.f, 0.3f);
			}

			break;

		case 2:

			if (m_fAccTime > 1.f)
			{
				m_fAccTime = 0.f;
				m_iOperProgress++;

				m_pTextImg[Text_Oper1]->DoMoveY(200.f, 0.3f);
				m_pTextImg[Text_Oper1]->DoScale(-256.f, 0.3f);

				for (int i = 0; i < 4; ++i)
				{
					Enable_Fade(m_pArrOperProfile[i], 0.3f);
				}
			}

			break;

		case 3:

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

			break;

		case 4:

			m_fAccTime = 0.f;
			m_iOperProgress++;

			for (int i = 0; i < Team_End; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					Enable_Fade(m_pArrTeamIcon[i][j], 0.3f);
				}
			}

			Enable_StrongHoldUI();

			break;

		case 5:

			m_fAccTime = 0.f;
			m_iOperProgress++;

			for (int i = 0; i < 2; ++i)
			{
				Enable_Fade(m_pArrStrongHoldEffect[i], 1.f);
				m_pArrStrongHoldEffect[i]->DoScale(70.f, 1.f);
			}

			break;

		case 6:

			if (m_fAccTime > 0.5f)
			{
				m_fAccTime = 0.f;
				m_iOperProgress++;

				for (int i = 2; i < 4; ++i)
				{
					Enable_Fade(m_pArrStrongHoldEffect[i], 1.f);
					m_pArrStrongHoldEffect[i]->DoScale(70.f, 1.f);
				}
			}

			break;
		}
	}
}

void CUI_Oper::Enable_StrongHoldUI()
{
	for (int i = 0; i < SP_End; ++i)
	{
		switch (m_eLoadLevel)
		{
		case LEVEL_TEST:

			for (int j = 0; j < 2; ++j)
			{
				Enable_Fade(m_pArrStrongHoldUI[i][j], 0.3f);
				m_pArrStrongHoldUI[i][j]->DoScale(-70.f, 0.3f);
			}

			DISABLE_GAMEOBJECT(m_pArrStrongHoldUI[SP_Icon][0]);

			break;

		case LEVEL_PADEN:

			for (int j = 0; j < 2; ++j)
			{
				Enable_Fade(m_pArrStrongHoldUI[i][j], 0.3f);
				m_pArrStrongHoldUI[i][j]->DoScale(-70.f, 0.3f);
			}

			DISABLE_GAMEOBJECT(m_pArrStrongHoldUI[SP_Icon][0]);

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

		if (i == Text_Oper1)
		{
			m_pTextImg[i]->Set_FadeDesc(0.3f);

			m_pTextImg[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Oper/OperMeeting.png"));

			m_pTextImg[i]->Set_PosY(50.f);
			m_pTextImg[i]->Set_Scale(1024.f);
			m_pTextImg[i]->Set_Sort(0.49f);
		}

		CREATE_GAMEOBJECT(m_pTextImg[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pTextImg[i]);
	}
}

void CUI_Oper::Create_OperBG()
{
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

		CREATE_GAMEOBJECT(m_pArrOperProfile[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pArrOperProfile[i]);
	}
}

void CUI_Oper::Create_OperCharacterSelect()
{
	m_pOperSideBG = CUI_Object::Create();

	m_pOperSideBG->Set_FadeDesc(0.3f);

	m_pOperSideBG->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Oper/T_ScrollBG.png"));

	m_pOperSideBG->Set_Scale(250.f, 753.f);
	m_pOperSideBG->Set_Sort(0.49f);

	CREATE_GAMEOBJECT(m_pOperSideBG, GROUP_UI);
	DELETE_GAMEOBJECT(m_pOperSideBG);

	for (int i = 0; i < 2; ++i)
	{
		m_pArrOperSideBG[i] = m_pOperSideBG->Clone();

		_float fPosX = -565 + (i * 1130.f);
		m_pArrOperSideBG[i]->Set_PosX(fPosX);

		if (i == 1)
		{
			m_pArrOperSideBG[i]->Set_RotationZ(180.f);
		}

		CREATE_GAMEOBJECT(m_pArrOperSideBG[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_pArrOperSideBG[i]);
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

			CREATE_GAMEOBJECT(m_pArrCharacterPort[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrCharacterPort[i][j]);
		}
	}
}

void CUI_Oper::Init_OperCharacterSelect()
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

			m_pStrongHoldUI[i]->Set_Scale(115.f);
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

	for (int i = 0; i < 4; ++i)
	{
		m_pArrStrongHoldEffect[i] = m_pStrongHoldEffect->Clone();

		CREATE_GAMEOBJECT(m_pArrStrongHoldEffect[i], RENDER_UI);
		DISABLE_GAMEOBJECT(m_pArrStrongHoldEffect[i]);
	}
}

void CUI_Oper::Init_StrongHoldUI()
{
	switch (m_eLoadLevel)
	{
	case LEVEL_TEST:

		m_pArrStrongHoldUI[SP_BG][0]->Set_PosY(132.f);
		m_pArrStrongHoldUI[SP_Outline][0]->Set_PosY(133.f);
		m_pArrStrongHoldUI[SP_TEXT][0]->Set_PosY(128.f);

		m_pArrStrongHoldUI[SP_BG][1]->Set_PosY(-161.f);
		m_pArrStrongHoldUI[SP_Outline][1]->Set_PosY(-161.f);
		m_pArrStrongHoldUI[SP_Icon][1]->Set_PosY(-161.f);
		m_pArrStrongHoldUI[SP_TEXT][1]->Set_PosY(-166.f);

		for (int i = 0; i < SP_End; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				GET_COMPONENT_FROM(m_pArrStrongHoldUI[i][j], CTexture)->Set_CurTextureIndex(j);
			}
		}

		break;

	case LEVEL_PADEN:

		m_pArrStrongHoldUI[SP_BG][0]->Set_PosY(132.f);
		m_pArrStrongHoldUI[SP_Outline][0]->Set_PosY(133.f);
		m_pArrStrongHoldUI[SP_TEXT][0]->Set_PosY(128.f);

		m_pArrStrongHoldUI[SP_BG][1]->Set_PosY(-161.f);
		m_pArrStrongHoldUI[SP_Outline][1]->Set_PosY(-161.f);
		m_pArrStrongHoldUI[SP_Icon][1]->Set_PosY(-161.f);
		m_pArrStrongHoldUI[SP_TEXT][1]->Set_PosY(-166.f);

		for (int i = 0; i < SP_End; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				GET_COMPONENT_FROM(m_pArrStrongHoldUI[i][i], CTexture)->Set_CurTextureIndex(j);
			}
		}

		break;

	case LEVEL_HWARA:
		break;
	}
}

void CUI_Oper::StrongHoldEffect()
{
	switch (m_eLoadLevel)
	{
	case LEVEL_TEST:

		m_pArrStrongHoldEffect[0]->Set_PosY(133.f);
		m_pArrStrongHoldEffect[1]->Set_PosY(-161.f);
		m_pArrStrongHoldEffect[2]->Set_PosY(133.f);
		m_pArrStrongHoldEffect[3]->Set_PosY(-161.f);

		break;

	case LEVEL_PADEN:
		m_pArrStrongHoldEffect[0]->Set_PosY(133.f);
		m_pArrStrongHoldEffect[1]->Set_PosY(-161.f);
		m_pArrStrongHoldEffect[2]->Set_PosY(133.f);
		m_pArrStrongHoldEffect[3]->Set_PosY(-161.f);

		break;

	case LEVEL_HWARA:
		break;
	}
}
