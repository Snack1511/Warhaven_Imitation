#include "CUI_Info.h"
#include "Loading_Manager.h"
#include "CUI_Object.h"
#include "GameInstance.h"

CUI_Info::CUI_Info()
{
}

CUI_Info::~CUI_Info()
{
}

HRESULT CUI_Info::Initialize_Prototype()
{
	Create_InfoUI();

	return S_OK;
}

HRESULT CUI_Info::Start()
{
	__super::Start();

	m_eLoadLevel = CLoading_Manager::Get_Instance()->Get_LoadLevel();

	Init_InfoUI();

	return S_OK;
}

void CUI_Info::SetActive_Info(_bool value)
{
	for (int i = 0; i < Info_End; ++i)
		Enable_Fade(m_pInfoUI[i], 0.3f);
}

void CUI_Info::My_Tick()
{
	__super::My_Tick();
}

void CUI_Info::My_LateTick()
{
	__super::My_LateTick();
}

void CUI_Info::OnEnable()
{
	__super::OnEnable();
}

void CUI_Info::OnDisable()
{
	__super::OnDisable();
}

void CUI_Info::Create_InfoUI()
{
	for (int i = 0; i < Info_End; ++i)
	{
		m_pInfoUI[i] = CUI_Object::Create();

		m_pInfoUI[i]->Set_FadeDesc(0.1f, 0.3f, 3.f, true);
		m_pInfoUI[i]->Set_Sort(0.49f);

		switch (i)
		{
		case Info_BG:

			m_pInfoUI[i]->Set_Sort(0.5f);
			m_pInfoUI[i]->Set_Scale(300.f, 150.f);
			m_pInfoUI[i]->Set_PosX(fPosX);
			m_pInfoUI[i]->Set_Color(_float4(20.f, 20.f, 20.f, 0.5f));

			break;

		case Info_Line:

			m_pInfoUI[i]->Set_Scale(265.f, 2.f);
			m_pInfoUI[i]->Set_PosX(fPosX);

			break;

		case Info_Name:

			m_pInfoUI[i]->Set_Scale(87.f, 60.f);
			m_pInfoUI[i]->Set_PosX(fPosX - 110.f);
			m_pInfoUI[i]->Set_PosY(45);

			break;

		case Info_Mode:

			m_pInfoUI[i]->Set_Scale(66.f, 35.f);
			m_pInfoUI[i]->Set_PosX(fPosX - 110.f);
			m_pInfoUI[i]->Set_PosY(17);
			m_pInfoUI[i]->Set_Color(RGB(200, 200, 200));

			break;
		}
	}
}

void CUI_Info::Init_InfoUI()
{
	for (int i = 0; i < Info_End; ++i)
	{
		CREATE_GAMEOBJECT(m_pInfoUI[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pInfoUI[i]);
	}

	switch (m_eLoadLevel)
	{
	case Client::LEVEL_PADEN:

		m_pInfoUI[Info_Name]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Text/Text_Paden.png"));
		m_pInfoUI[Info_Mode]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Text/Text_PadenInfo.png"));

		break;

	case Client::LEVEL_HWARA:

		m_pInfoUI[Info_Name]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Text/Text_Hwara.png"));
		m_pInfoUI[Info_Mode]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Text/Text_HwaraInfo.png"));

		break;
	}
}
