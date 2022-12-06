#include "CUI_CharacterWindow.h"
#include "CUI_Object.h"
#include "GameInstance.h"
#include "Texture.h"
#include "CFader.h"
#include "CUser.h"
#include "CUI_HUD.h"

CUI_CharacterWindow::CUI_CharacterWindow()
{
}

CUI_CharacterWindow::~CUI_CharacterWindow()
{
}

HRESULT CUI_CharacterWindow::Initialize_Prototype()
{
	Create_BG();
	Create_CharacterUI();
	Create_ClassInfo();

	return S_OK;
}

HRESULT CUI_CharacterWindow::Start()
{
	__super::Start();

	Bind_Btn();

	Init_CharacterWindow(m_eCurUnitClass);

	return S_OK;
}

void CUI_CharacterWindow::On_PointerEnter(const _uint& iEventNum)
{
	Enable_Fade(m_pArrCharacterUI[CU_Highlight][iEventNum], 0.3f);
}

void CUI_CharacterWindow::On_PointerExit(const _uint& iEventNum)
{
	Disable_Fade(m_pArrCharacterUI[CU_Highlight][iEventNum], 0.3f);
}

void CUI_CharacterWindow::On_PointerDown(const _uint& iEventNum)
{
	if (iEventNum > CT_DEFAULT_END)
		return;

	m_iPrvEventNum = m_iCurEventNum;
	m_iCurEventNum = iEventNum;

	if (m_iPrvEventNum == iEventNum)
		return;

	m_eCurUnitClass = (CLASS_TYPE)iEventNum;

	_float fDuration = 0.1f;

	for (int i = 0; i < CU_Line; ++i)
	{
		m_pArrCharacterUI[i][m_iPrvEventNum]->DoMoveY(-10.f, fDuration);
		m_pArrCharacterUI[i][iEventNum]->DoMoveY(10.f, fDuration);
	}

	m_pArrCharacterUI[CU_Line][m_iPrvEventNum]->Lerp_ScaleX(100.f, 2.f, fDuration);
	Disable_Fade(m_pArrCharacterUI[CU_Line][m_iPrvEventNum], fDuration);

	Enable_Fade(m_pArrCharacterUI[CU_Line][iEventNum], fDuration);
	m_pArrCharacterUI[CU_Line][iEventNum]->Lerp_ScaleX(2.f, 100.f, fDuration);

	GET_COMPONENT_FROM(m_pClassInfo[CI_Icon], CTexture)->Set_CurTextureIndex(iEventNum);

	switch (iEventNum)
	{
	case Client::WARRIOR:
		m_pClassInfo[CI_Icon]->Set_FontText(TEXT("블레이드"));
		break;
	case Client::SPEAR:
		m_pClassInfo[CI_Icon]->Set_FontText(TEXT("스파이크"));
		break;
	case Client::ARCHER:
		m_pClassInfo[CI_Icon]->Set_FontText(TEXT("아치"));
		break;
	case Client::PALADIN:
		m_pClassInfo[CI_Icon]->Set_FontText(TEXT("가디언"));
		break;
	case Client::PRIEST:
		m_pClassInfo[CI_Icon]->Set_FontText(TEXT("스모크"));
		break;
	case Client::ENGINEER:
		m_pClassInfo[CI_Icon]->Set_FontText(TEXT("워해머"));
		break;
	}

	CUser::Get_Instance()->Set_HUD(m_eCurUnitClass);
}

void CUI_CharacterWindow::SetActive_CharacterWindow(_bool value)
{
	CUser::Get_Instance()->Set_FixCursor(!value);

	m_pBG->SetActive(value);

	for (int i = 0; i < CU_End; ++i)
	{
		for (int j = 0; j < 6; ++j)
		{
			m_pArrCharacterUI[i][j]->SetActive(value);
		}
	}

	for (int i = 0; i < CI_End; ++i)
	{
		m_pClassInfo[i]->SetActive(value);
	}
}

void CUI_CharacterWindow::Create_BG()
{
	m_pBG = CUI_Object::Create();

	m_pBG->Set_Scale(1280.f, 720.f);
	m_pBG->Set_Sort(0.5f);
	m_pBG->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/T_LobbyBG.dds"));

	CREATE_GAMEOBJECT(m_pBG, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pBG);
}

void CUI_CharacterWindow::Create_CharacterUI()
{
	for (int i = 0; i < CU_End; ++i)
	{
		m_pCharacterUI[i] = CUI_Object::Create();

		CREATE_GAMEOBJECT(m_pCharacterUI[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_pCharacterUI[i]);

		m_pCharacterUI[i]->Set_PosY(-250.f);
		m_pCharacterUI[i]->Set_Scale(100.f, 140.f);

		if (i == CU_BG)
		{
			m_pCharacterUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/CharacterWindow/BG.dds"));

			m_pCharacterUI[i]->Set_Sort(0.49f);
			m_pCharacterUI[i]->Set_MouseTarget(true);
		}
		else if (i == CU_Port)
		{
			GET_COMPONENT_FROM(m_pCharacterUI[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pCharacterUI[i], "/CharacterWindow", "Portrait");

			m_pCharacterUI[i]->Set_Sort(0.48f);
		}
		else if (i == CU_Highlight)
		{
			m_pCharacterUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/CharacterWindow/Highlight.png"));
			m_pCharacterUI[i]->Set_Color(_float4(1.f, 1.f, 1.f, 0.3f));

			m_pCharacterUI[i]->Set_PosY(-248.f);
			m_pCharacterUI[i]->Set_Scale(100.f, 144.f);
			m_pCharacterUI[i]->Set_Sort(0.47f);

			Set_FadeDesc(m_pCharacterUI[i], 0.3f);
		}
		else if (i == CU_Icon)
		{
			GET_COMPONENT_FROM(m_pCharacterUI[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pCharacterUI[i], "/CharacterWindow", "Icon");

			m_pCharacterUI[i]->Set_Sort(0.48f);
			m_pCharacterUI[i]->Set_PosY(-195.f);
			m_pCharacterUI[i]->Set_Scale(20.f);
		}
		else if (i == CU_Line)
		{
			m_pCharacterUI[i]->Set_Color(m_vColorGold);

			m_pCharacterUI[i]->Set_Sort(0.49f);
			m_pCharacterUI[i]->Set_PosY(-318.f);
			m_pCharacterUI[i]->Set_Scale(2.f);

			Set_FadeDesc(m_pCharacterUI[i], 0.1f);
		}

		for (int j = 0; j < 6; ++j)
		{
			m_pArrCharacterUI[i][j] = m_pCharacterUI[i]->Clone();

			_float fPosX = -300.f + (j * 120.f);
			m_pArrCharacterUI[i][j]->Set_PosX(fPosX);

			if (i == CU_Port)
			{
				GET_COMPONENT_FROM(m_pArrCharacterUI[i][j], CTexture)->Set_CurTextureIndex(j);
			}
			else if (i == CU_Icon)
			{
				GET_COMPONENT_FROM(m_pArrCharacterUI[i][j], CTexture)->Set_CurTextureIndex(j);

				m_pArrCharacterUI[i][j]->Set_PosX(fPosX + 35.f);
			}

			CREATE_GAMEOBJECT(m_pArrCharacterUI[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrCharacterUI[i][j]);
		}
	}
}

void CUI_CharacterWindow::Set_FadeDesc(CUI_Object* pUI, _float fSpeed)
{
	FADEDESC tFadeDesc;
	ZeroMemory(&tFadeDesc, sizeof(FADEDESC));

	tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_DISABLE;
	tFadeDesc.eFadeStyle = FADEDESC::FADE_STYLE_DEFAULT;

	tFadeDesc.bFadeInFlag = FADE_NONE;
	tFadeDesc.bFadeOutFlag = FADE_NONE;

	tFadeDesc.fFadeInStartTime = 0.f;
	tFadeDesc.fFadeInTime = fSpeed;

	tFadeDesc.fFadeOutStartTime = 0.f;
	tFadeDesc.fFadeOutTime = fSpeed;

	GET_COMPONENT_FROM(pUI, CFader)->Get_FadeDesc() = tFadeDesc;
}

void CUI_CharacterWindow::Create_ClassInfo()
{
	for (int i = 0; i < CI_End; ++i)
	{
		m_pClassInfo[i] = CUI_Object::Create();

		CREATE_GAMEOBJECT(m_pClassInfo[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pClassInfo[i]);

		m_pClassInfo[i]->Set_PosX(-550.f);

		if (i == CI_BG)
		{
			m_pClassInfo[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/CharacterWindow/ClassInfo_BG.dds"));

			m_pClassInfo[i]->Set_PosY(200.f);
			m_pClassInfo[i]->Set_Scale(75.f, 165.f);
			m_pClassInfo[i]->Set_Sort(0.49f);
		}
		else if (i == CI_Icon)
		{
			GET_COMPONENT_FROM(m_pClassInfo[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pClassInfo[i], "/CharacterWindow", "Icon");

			m_pClassInfo[i]->Set_PosY(220.f);
			m_pClassInfo[i]->Set_Scale(54.f);
			m_pClassInfo[i]->Set_Sort(0.48f);

			m_pClassInfo[i]->Set_FontRender(true);
			m_pClassInfo[i]->Set_FontStyle(true);
			m_pClassInfo[i]->Set_FontScale(0.5f);
			m_pClassInfo[i]->Set_FontOffset(50.f, -30.f);
		}
	}
}

void CUI_CharacterWindow::OnEnable()
{
	__super::OnEnable();

	SetActive_CharacterWindow(true);
}

void CUI_CharacterWindow::OnDisable()
{
	__super::OnDisable();

	SetActive_CharacterWindow(false);
}

void CUI_CharacterWindow::Bind_Btn()
{
	for (int i = 0; i < 6; ++i)
	{
		m_pArrCharacterUI[CU_BG][i]->CallBack_PointEnter += bind(&CUI_CharacterWindow::On_PointerEnter, this, i);
		m_pArrCharacterUI[CU_BG][i]->CallBack_PointExit += bind(&CUI_CharacterWindow::On_PointerExit, this, i);
		m_pArrCharacterUI[CU_BG][i]->CallBack_PointDown += bind(&CUI_CharacterWindow::On_PointerDown, this, i);
	}
}

void CUI_CharacterWindow::Init_CharacterWindow(CLASS_TYPE eCurUnitClass)
{
	m_iCurEventNum = eCurUnitClass;

	for (int i = 0; i < CU_Line; ++i)
	{
		m_pArrCharacterUI[i][m_iCurEventNum]->Set_PosY(-240.f);
		if (i == CU_Icon)
		{
			m_pArrCharacterUI[i][m_iCurEventNum]->Set_PosY(-185.f);
		}
	}

	m_pArrCharacterUI[CU_Line][m_iCurEventNum]->Set_ScaleX(100.f);
	Enable_Fade(m_pArrCharacterUI[CU_Line][m_iCurEventNum], 0.1f);
}
