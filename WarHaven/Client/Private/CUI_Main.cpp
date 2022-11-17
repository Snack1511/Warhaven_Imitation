#include "CUI_Main.h"
#include "GameInstance.h"
#include "Texture.h"

#include "CUI_MainPlay.h"
#include "CUI_MainMode.h"

#include "CUI_Object.h"
#include "CButton.h"

CUI_Main::CUI_Main()
{
}

CUI_Main::~CUI_Main()
{
}

HRESULT CUI_Main::Initialize_Prototype()
{
	// 윈도우 준비
	m_Prototypes[Play] = CUI_MainPlay::Create();

	Ready_MainUI();

	return S_OK;
}

HRESULT CUI_Main::Initialize()
{
	return S_OK;
}

HRESULT CUI_Main::Start()
{
	Enable_MainUI();
	Enable_MainWindow();

	__super::Start();

	return S_OK;
}

void CUI_Main::Ready_MainUI()
{
	Read_UI("Lobby");

	m_pPrototypeUI[Btn] = m_pUIMap[TEXT("Lobby_Btn")];
	m_pPrototypeUI[Key] = m_pUIMap[TEXT("Lobby_Key")];
	m_pPrototypeUI[Goods] = m_pUIMap[TEXT("Lobby_Goods")];

	for (int i = 0; i < MainEnd; ++i)
	{
		m_pPrototypeUI[i]->Set_Sort(0.9f);
	}

	GET_COMPONENT_FROM(m_pPrototypeUI[Btn], CTexture)->Remove_Texture(0);

	Read_Texture(m_pPrototypeUI[Btn], "/Lobby/LobbyText", "Text");

	GET_COMPONENT_FROM(m_pPrototypeUI[Key], CTexture)
		->Add_Texture(TEXT("../Bin/Resources/Textures/UI/KeyIcon/Keyboard/T_BlackEKeyIcon.dds"));

	for (int i = 0; i < 3; ++i)
	{
		m_pTopBtn[i] = m_pPrototypeUI[Btn]->Clone();
		m_pGoodsUI[i] = m_pPrototypeUI[Goods]->Clone();
	}

	for (int i = 0; i < 2; ++i)
	{
		m_pKeyUI[i] = m_pPrototypeUI[Key]->Clone();
	}
}

void CUI_Main::Enable_MainUI()
{
	for (int i = 0; i < 3; ++i)
	{
		CREATE_GAMEOBJECT(m_pPrototypeUI[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_pPrototypeUI[i]);
		m_pPrototypeUI[i] = nullptr;

		CREATE_GAMEOBJECT(m_pTopBtn[i], GROUP_UI);
		CREATE_GAMEOBJECT(m_pGoodsUI[i], GROUP_UI);

		GET_COMPONENT_FROM(m_pTopBtn[i], CTexture)->Set_CurTextureIndex(i);

		_float fPosX = -535.f + (i * 100.f);
		m_pTopBtn[i]->Set_PosX(fPosX);

		m_pTopBtn[i]->Set_Sort(0.95f);

		m_pGoodsUI[i]->Set_Sort(0.8f);
	}

	for (int i = 0; i < 2; ++i)
	{
		CREATE_GAMEOBJECT(m_pKeyUI[i], GROUP_UI);

		GET_COMPONENT_FROM(m_pKeyUI[i], CTexture)->Set_CurTextureIndex(i);

		_float fPosX = -600.f + (i * 360.f);
		m_pKeyUI[i]->Set_PosX(fPosX);

		m_pKeyUI[i]->Set_Sort(0.95f);
	}
}

void CUI_Main::Enable_MainWindow()
{
	CREATE_GAMEOBJECT(m_Prototypes[Play], GROUP_UI);
}
