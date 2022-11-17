#include "CUI_MainMode.h"
#include "GameInstance.h"
#include "CUI_Object.h"
#include "Texture.h"

CUI_MainMode::CUI_MainMode()
{
}

CUI_MainMode::~CUI_MainMode()
{
}

HRESULT CUI_MainMode::Initialize_Prototype()
{
	Create_ModeBG();
	Create_StageBtn();

    return S_OK;
}

HRESULT CUI_MainMode::Start()
{
	__super::Start();

    return S_OK;
}

void CUI_MainMode::On_PointUpEvent_Start(const _uint& iEventNum)
{
}

void CUI_MainMode::On_PointUpEvent_Mode(const _uint& iEventNum)
{
}

void CUI_MainMode::SetActive_StageBtn(_bool value)
{
	for (int i = 0; i < 4; ++i)
	{
		if (value)
		{
			ENABLE_GAMEOBJECT(m_pStageSelectBtn[i]);
		}
		else
		{
			DISABLE_GAMEOBJECT(m_pStageSelectBtn[i]);
		}
	}
}

void CUI_MainMode::Create_ModeBG()
{
	m_pBG = CUI_Object::Create();
	m_pBG->Set_Scale(1280.f, 720.f);
	m_pBG->Set_Sort(0.5f);
	m_pBG->SetTexture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/T_LobbyBG2.dds"));

	CREATE_GAMEOBJECT(m_pBG, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pBG);
}

void CUI_MainMode::Create_StageBtn()
{
	m_pPrototypeStageBtn = CUI_Object::Create();
	m_pPrototypeStageBtn->Set_Scale(216.f, 340.f);
	m_pPrototypeStageBtn->Set_Sort(0.4f);

	m_pPrototypeStageBtn->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/Enable_Stage1.dds"));

	GET_COMPONENT_FROM(m_pPrototypeStageBtn, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/Enable_Stage2.dds"));
	GET_COMPONENT_FROM(m_pPrototypeStageBtn, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/Enable_Stage3.dds"));
	GET_COMPONENT_FROM(m_pPrototypeStageBtn, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/Enable_Stage4.dds"));

	for (int i = 0; i < 4; ++i)
	{
		m_pStageSelectBtn[i] = m_pPrototypeStageBtn->Clone();
	}

	CREATE_GAMEOBJECT(m_pPrototypeStageBtn, GROUP_UI);
	DELETE_GAMEOBJECT(m_pPrototypeStageBtn);

	for (int i = 0; i < 4; ++i)
	{
		_float fPosX = -360.f + (i * 240.f);
		m_pStageSelectBtn[i]->Set_Pos(fPosX, 0.f);

		GET_COMPONENT_FROM(m_pStageSelectBtn[i], CTexture)->Set_CurTextureIndex(i);

		CREATE_GAMEOBJECT(m_pStageSelectBtn[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pStageSelectBtn[i]);
	}
}