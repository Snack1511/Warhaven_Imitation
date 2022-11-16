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

	return S_OK;
}

HRESULT CUI_MainPlay::Initialize()
{
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

	Set_StartBtn();
	Set_ModeBtn();

	return S_OK;
}

void CUI_MainPlay::On_PointUpEvent_Start(const _uint& iEventNum)
{
	CLoading_Manager::Get_Instance()->Reserve_Load_Level(LEVEL_TEST);
}

void CUI_MainPlay::On_PointUpEvent_Mode(const _uint& iEventNum)
{
	// 버튼들 확대
	
	// 활성화 될 창 페이드인 되면서 등장


}

void CUI_MainPlay::Set_StartBtn()
{
	m_pPlayBtnUI[0]->CallBack_PointUp += bind(&CUI_MainPlay::On_PointUpEvent_Start, this, placeholders::_1);
}

void CUI_MainPlay::Set_ModeBtn()
{
	m_pPlayBtnUI[1]->Set_Scale(120.f, 60.f);
	m_pPlayBtnUI[1]->Set_Pos(-560.f, 78.f);

	GET_COMPONENT_FROM(m_pPlayBtnUI[1], CTexture)->Set_CurTextureIndex(1);


	m_pPlayBtnUI[1]->CallBack_PointUp += bind(&CUI_MainPlay::On_PointUpEvent_Mode, this, placeholders::_1);
}
