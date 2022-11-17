#include "CUI_MainPlay.h"
#include "Texture.h"
#include "GameInstance.h"
#include "CUI_Object.h"
#include "CUI_Renderer.h"

#include "Loading_Manager.h"

CUI_MainPlay::CUI_MainPlay()
{
}

CUI_MainPlay::~CUI_MainPlay()
{
}

HRESULT CUI_MainPlay::Initialize_Prototype()
{
	m_wstrName = TEXT("MainPlay_Btn");

	__super::Initialize_Prototype();

	GET_COMPONENT_FROM(m_pUI, CTexture)
		->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/PlayText/Text_Mode.png"));

	for (int i = 0; i < 2; ++i)
	{
		m_pPlayBtnUI[i] = m_pUI->Clone();
	}

	Create_ModeBG();
	Create_StageBtn();

	return S_OK;
}

HRESULT CUI_MainPlay::Start()
{
	__super::Start();

	DELETE_GAMEOBJECT(m_pUI);

	for (int i = 0; i < 2; ++i)
	{
		CREATE_GAMEOBJECT(m_pPlayBtnUI[i], GROUP_UI);
	}

	Set_ModeBtn();

	Bind_Btn();

	return S_OK;
}

void CUI_MainPlay::On_PointUpEvent_Start(const _uint& iEventNum)
{
	CLoading_Manager::Get_Instance()->Reserve_Load_Level(LEVEL_TEST);
}

void CUI_MainPlay::On_PointUpEvent_Mode(const _uint& iEventNum)
{
	for (int i = 0; i < 2; ++i)
	{
		DISABLE_GAMEOBJECT(m_pPlayBtnUI[i]);
	}

	ENABLE_GAMEOBJECT(m_pBG);

	for (int i = 0; i < 4; ++i)
	{
		ENABLE_GAMEOBJECT(m_pStageSelectBtn[i]);
	}
}

void CUI_MainPlay::Bind_Btn()
{
	m_pPlayBtnUI[0]->CallBack_PointUp += bind(&CUI_MainPlay::On_PointUpEvent_Start, this, placeholders::_1);
	m_pPlayBtnUI[1]->CallBack_PointUp += bind(&CUI_MainPlay::On_PointUpEvent_Mode, this, placeholders::_1);
}

void CUI_MainPlay::Set_ModeBtn()
{
	m_pPlayBtnUI[1]->Set_Scale(120.f, 60.f);
	m_pPlayBtnUI[1]->Set_Pos(-560.f, 78.f);

	GET_COMPONENT_FROM(m_pPlayBtnUI[1], CTexture)->Set_CurTextureIndex(1);
}

void CUI_MainPlay::Create_ModeBG()
{
	m_pBG = CUI_Object::Create();

	m_pBG->Set_Scale(1280.f, 720.f);
	m_pBG->Set_Sort(0.9f);

	m_pBG->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/T_LobbyBG2.dds"));

	CREATE_GAMEOBJECT(m_pBG, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pBG);
}

void CUI_MainPlay::Create_StageBtn()
{
	m_pPrototypeStageBtn = CUI_Object::Create();

	m_pPrototypeStageBtn->Set_Scale(216.f, 340.f);
	m_pPrototypeStageBtn->Set_Sort(0.4f);

	m_pPrototypeStageBtn->Set_MouseTarget(true);
	
	m_pPrototypeStageBtn->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/Enable_Stage1.dds"));

	GET_COMPONENT_FROM(m_pPrototypeStageBtn, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/Enable_Stage2.dds"));
	GET_COMPONENT_FROM(m_pPrototypeStageBtn, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/Enable_Stage3.dds"));
	GET_COMPONENT_FROM(m_pPrototypeStageBtn, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/Enable_Stage4.dds"));
	GET_COMPONENT_FROM(m_pPrototypeStageBtn, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/Disable_Stage1.dds"));
	GET_COMPONENT_FROM(m_pPrototypeStageBtn, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/Disable_Stage2.dds"));
	GET_COMPONENT_FROM(m_pPrototypeStageBtn, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/Disable_Stage3.dds"));
	GET_COMPONENT_FROM(m_pPrototypeStageBtn, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Lobby/ModeWindow/Disable_Stage4.dds"));

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

		// 훈련장 제외 모든 버튼 비활성화
		GET_COMPONENT_FROM(m_pStageSelectBtn[i], CTexture)->Set_CurTextureIndex(i + (1 * 4));

		// 훈련장 활성화
		if (i == 3)
		{
			GET_COMPONENT_FROM(m_pStageSelectBtn[i], CTexture)->Set_CurTextureIndex(i);
		}

		CREATE_GAMEOBJECT(m_pStageSelectBtn[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pStageSelectBtn[i]);
	}
}