#include "CUI_Main.h"
#include "GameInstance.h"
#include "Texture.h"

#include "CUI_MainPlay.h"
#include "CUI_Object.h"

CUI_Main::CUI_Main()
{
}

CUI_Main::~CUI_Main()
{
}

HRESULT CUI_Main::Initialize_Prototype()
{
	m_Prototypes[Play] = CUI_MainPlay::Create();

	Read_UI("Lobby");
	
	m_pPrototypeUI[Btn] = m_pUIMap[TEXT("Lobby_Btn")];
	m_pPrototypeUI[Key] = m_pUIMap[TEXT("Lobby_Key")];
	m_pPrototypeUI[Goods] = m_pUIMap[TEXT("Lobby_Goods")];

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

void CUI_Main::Enable_MainUI()
{
	for (int i = 0; i < 3; ++i)
	{
		CREATE_GAMEOBJECT(m_pPrototypeUI[i], GROUP_UI);

		DELETE_GAMEOBJECT(m_pPrototypeUI[i]);

		CREATE_GAMEOBJECT(m_pTopBtn[i], GROUP_UI);
		CREATE_GAMEOBJECT(m_pGoodsUI[i], GROUP_UI);
		
		GET_COMPONENT_FROM(m_pTopBtn[i], CTexture)->Set_CurTextureIndex(i);
		
		_float fPosX = -500.f + (i * 100.f);
		m_pTopBtn[i]->Set_PosX(fPosX);
	}

	for (int i = 0; i < 2; ++i)
	{
		CREATE_GAMEOBJECT(m_pKeyUI[i], GROUP_UI);
		
		GET_COMPONENT_FROM(m_pKeyUI[i], CTexture)->Set_CurTextureIndex(i);
		
		_float fPosX = -580.f + (i * 360.f);
		m_pKeyUI[i]->Set_PosX(fPosX);
	}
}

void CUI_Main::Enable_MainWindow()
{
	CREATE_GAMEOBJECT(m_Prototypes[Play], GROUP_UI);
}
