#include "CUI_Main.h"
#include "GameInstance.h"
#include "CUI_Renderer.h"
#include "Texture.h"
#include "CShader.h"
#include "CGameSystem.h"
#include "CPlayerInfo_Main.h"

#include "CUI_MainPlay.h"
#include "CUI_Barracks.h"

#include "CUI_Object.h"
#include "CUser.h"
#include "CPlayer.h"
#include "CUnit.h"

CUI_Main::CUI_Main()
{
}

CUI_Main::~CUI_Main()
{
}

HRESULT CUI_Main::Initialize_Prototype()
{
	Create_TopBtn();
	Create_TopBtnEffect();
	Create_PlayerNameText();
	Create_MainWindow();

	return S_OK;
}

HRESULT CUI_Main::Start()
{
	__super::Start();

	Bind_Btn();
	Bind_Shader();

	SetActive_TopBtn(true);
	SetActive_PlayerNameText(true);
	SetActive_MainWindow(MW_Play);

	return S_OK;
}

void CUI_Main::On_PointEnter_TopBtn(const _uint& iEventNum)
{
	m_pArrTopBtn[iEventNum]->Set_FontColor(m_vColorWhite);
}

void CUI_Main::On_PointExit_TopBtn(const _uint& iEventNum)
{
	if (!m_pArrTopBtn[iEventNum]->Get_IsClick())
	{
		m_pArrTopBtn[iEventNum]->Set_FontColor(m_vColorGrey);
	}
}

void CUI_Main::On_PointDown_TopBtn(const _uint& iEventNum)
{
	m_iPrvEnvetNum = m_iCurEventNum;
	m_iCurEventNum = iEventNum;

	if (m_iPrvEnvetNum == iEventNum)
		return;

	m_pArrTopBtn[m_iPrvEnvetNum]->Set_IsClick(false);
	m_pArrTopBtn[m_iPrvEnvetNum]->Set_FontColor(m_vColorGrey);

	m_pArrTopBtn[iEventNum]->Set_IsClick(true);
	m_pArrTopBtn[iEventNum]->Set_FontColor(m_vColorWhite);

	_float4 vPos = m_pArrTopBtn[iEventNum]->Get_Pos();
	m_pTopBtnEffect->Set_Pos(vPos.x, vPos.y);

	ENABLE_GAMEOBJECT(m_pTopBtnEffect);

	SetActive_MainWindow((MainWindow)iEventNum);
}

void CUI_Main::Set_Shader_TopBtnEffect(CShader* pShader, const char* pConstName)
{
	m_fAccTime += fDT(0) * 0.25f;
	pShader->Set_RawValue("g_fValue", &m_fAccTime, sizeof(_float));
}

void CUI_Main::SetActive_TopBtn(_bool value)
{
	for (int i = 0; i < TB_End; ++i)
	{
		if (value == true)
		{
			ENABLE_GAMEOBJECT(m_pArrTopBtn[i]);
		}
		else
		{
			DISABLE_GAMEOBJECT(m_pArrTopBtn[i]);
		}
	}
}

void CUI_Main::SetActive_PlayerNameText(_bool value)
{
	for (int i = 0; i < PI_End; ++i)
	{
		if (value == true)
		{
			ENABLE_GAMEOBJECT(m_pPlayerInfo[i]);
		}
		else
		{
			DISABLE_GAMEOBJECT(m_pPlayerInfo[i]);
		}
	}
}

void CUI_Main::SetActive_MainWindow(MainWindow eWindow)
{
	m_eWindow = eWindow;

	for (int i = 0; i < MW_Profile; ++i)
	{
		DISABLE_GAMEOBJECT(m_pMainWindow[i]);
	}

	ENABLE_GAMEOBJECT(m_pMainWindow[m_eWindow]);

	switch (m_eWindow)
	{
	case MW_Play:
	{
		m_pArrTopBtn[TB_Play]->Set_IsClick(true);
		m_pArrTopBtn[TB_Play]->Set_FontColor(m_vColorWhite);

		_float4 vPos = m_pArrTopBtn[TB_Play]->Get_Pos();
		m_pTopBtnEffect->Set_Pos(vPos.x, vPos.y);

		ENABLE_GAMEOBJECT(m_pTopBtnEffect);
	}
	break;

	//case MW_Barracks:
	//{
	//	m_pArrTopBtn[TB_Barracks]->Set_IsClick(true);
	//	m_pArrTopBtn[TB_Barracks]->Set_FontColor(m_vColorWhite);

	//	_float4 vPos = m_pArrTopBtn[TB_Barracks]->Get_Pos();
	//	m_pTopBtnEffect->Set_Pos(vPos.x, vPos.y);

	//	ENABLE_GAMEOBJECT(m_pTopBtnEffect);
	//}
	break;
	}
}

void CUI_Main::Create_TopBtn()
{
	for (int i = 0; i < TB_End; ++i)
	{
		m_pArrTopBtn[i] = CUI_Object::Create();
		m_pArrTopBtn[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Alpha0.png"));

		_float fPosX = -550.f + (i * 95.f);

		m_pArrTopBtn[i]->Set_Pos(fPosX, 300.f);
		m_pArrTopBtn[i]->Set_Scale(75.f, 35.f);
		m_pArrTopBtn[i]->Set_Sort(0.5f);

		m_pArrTopBtn[i]->Set_MouseTarget(true);

		m_pArrTopBtn[i]->Set_FontRender(true);
		m_pArrTopBtn[i]->Set_FontStyle(true);
		m_pArrTopBtn[i]->Set_FontCenter(true);
		m_pArrTopBtn[i]->Set_FontScale(0.4f);
		m_pArrTopBtn[i]->Set_FontColor(_float4(0.5f, 0.5f, 0.5f, 1.f));

		if (i == TB_Play)
		{
			m_pArrTopBtn[i]->Set_FontText(TEXT("플레이"));

		}
		else if (i == TB_Barracks)
		{
			m_pArrTopBtn[i]->Set_FontText(TEXT("병영"));
		}
		else if (i == TB_Profile)
		{
			m_pArrTopBtn[i]->Set_FontText(TEXT("프로필"));
		}

		CREATE_GAMEOBJECT(m_pArrTopBtn[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pArrTopBtn[i]);
	}
}

void CUI_Main::Create_TopBtnEffect()
{
	m_pTopBtnEffect = CUI_Object::Create();

	m_pTopBtnEffect->Set_Scale(90.f, 120.f);
	m_pTopBtnEffect->Set_Sort(0.95f);

	m_pTopBtnEffect->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/Effect/T_BGMainTab.dds"));
	m_pTopBtnEffect->SetTexture(TEXT("../Bin/Resources/Textures/UI/Lobby/Effect/T_SelectPT3.png"));
	m_pTopBtnEffect->SetTexture(TEXT("../Bin/Resources/Textures/UI/Lobby/Effect/T_soft_smoke.dds"));

	GET_COMPONENT_FROM(m_pTopBtnEffect, CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_LobbyEffect);

	CREATE_GAMEOBJECT(m_pTopBtnEffect, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pTopBtnEffect);
}

void CUI_Main::Create_PlayerNameText()
{
	for (int i = 0; i < PI_End; ++i)
	{
		m_pPlayerInfo[i] = CUI_Object::Create();

		if (i == PI_Level)
		{
			m_pPlayerInfo[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/T_IconLevelBG.dds"));
			m_pPlayerInfo[i]->Set_Pos(480.f, 300.f);
			m_pPlayerInfo[i]->Set_Scale(32.f);
			m_pPlayerInfo[i]->Set_Sort(0.4f);

			m_pPlayerInfo[i]->Set_FontRender(true);
			m_pPlayerInfo[i]->Set_FontStyle(true);
			m_pPlayerInfo[i]->Set_FontCenter(true);
			m_pPlayerInfo[i]->Set_FontScale(0.25f);
			m_pPlayerInfo[i]->Set_FontOffset(5.f, 5.f);

			m_pPlayerInfo[i]->Set_FontText(TEXT("1"));
		}
		else if (i == PI_Name)
		{
			m_pPlayerInfo[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Alpha0.png"));
			m_pPlayerInfo[i]->Set_Pos(500.f, 313.f);
			m_pPlayerInfo[i]->Set_FontRender(true);
			m_pPlayerInfo[i]->Set_FontStyle(true);
			m_pPlayerInfo[i]->Set_FontScale(0.25f);

			CPlayerInfo_Main* pMainPlayerInfo = dynamic_cast<CPlayerInfo_Main*>(CGameSystem::Get_Instance()->Find_PlayerInfo(HASHCODE(CPlayerInfo_Main)));
			wstring mainPlayerName = pMainPlayerInfo->MainPlayerName();
			m_pPlayerInfo[i]->Set_FontText(mainPlayerName);
		}

		CREATE_GAMEOBJECT(m_pPlayerInfo[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pPlayerInfo[i]);
	}
}

void CUI_Main::Create_MainWindow()
{
	m_pMainWindow[MW_Play] = CUI_MainPlay::Create();
	//m_pMainWindow[MW_Barracks] = CUI_Barracks::Create();

	CREATE_GAMEOBJECT(m_pMainWindow[MW_Play], GROUP_UI);
	DISABLE_GAMEOBJECT(m_pMainWindow[MW_Play]);

	//CREATE_GAMEOBJECT(m_pMainWindow[MW_Barracks], GROUP_UI);
	//DISABLE_GAMEOBJECT(m_pMainWindow[MW_Barracks]);
}

void CUI_Main::Bind_Btn()
{
	for (int i = 0; i < TB_End; ++i)
	{
		m_pArrTopBtn[i]->CallBack_PointEnter += bind(&CUI_Main::On_PointEnter_TopBtn, this, i);
		m_pArrTopBtn[i]->CallBack_PointExit += bind(&CUI_Main::On_PointExit_TopBtn, this, i);
		m_pArrTopBtn[i]->CallBack_PointDown += bind(&CUI_Main::On_PointDown_TopBtn, this, i);
	}
}

void CUI_Main::Bind_Shader()
{
	GET_COMPONENT_FROM(m_pTopBtnEffect, CShader)->CallBack_SetRawValues += bind(&CUI_Main::Set_Shader_TopBtnEffect, this, placeholders::_1, "g_fValue");

}
