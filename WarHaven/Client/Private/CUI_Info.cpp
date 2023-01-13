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
	Create_InfoText();

	return S_OK;
}

HRESULT CUI_Info::Start()
{
	__super::Start();

	m_eLoadLevel = CLoading_Manager::Get_Instance()->Get_LoadLevel();

	Init_InfoUI();
	Init_InfoText();

	return S_OK;
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

	for (int i = 0; i < Info_End; ++i)
		Enable_Fade(m_pInfoUI[i], 0.3f);

	switch (m_eLoadLevel)
	{
	case Client::LEVEL_PADEN:

		for (int i = 0; i < Text_End; ++i)
			Enable_Fade(m_pInfoText[i], 0.3f);

		break;

	case Client::LEVEL_HWARA:

		for (int i = 0; i < Text_End; ++i)
			Enable_Fade(m_pInfoText[i], 0.3f);

		break;
	}
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

		m_pInfoUI[i]->Set_FadeDesc(0.1f, 0.3f, m_fDisableTime, true);
		m_pInfoUI[i]->Set_Sort(0.49f);

		switch (i)
		{
		case Info_BG:

			m_pInfoUI[i]->Set_Sort(0.5f);
			m_pInfoUI[i]->Set_Scale(300.f, 125.f);
			m_pInfoUI[i]->Set_PosX(fPosX);
			m_pInfoUI[i]->Set_Color(_float4(0.f, 0.f, 0.f, 0.8f));

			break;

		case Info_Line:

			m_pInfoUI[i]->Set_Scale(275.f, 2.f);
			m_pInfoUI[i]->Set_PosX(fPosX);

			break;

		case Info_Name:

			m_pInfoUI[i]->Set_Scale(52.f, 36.f);
			m_pInfoUI[i]->Set_PosX(fPosX - 75.f);
			m_pInfoUI[i]->Set_PosY(38);

			break;

		case Info_Mode:

			m_pInfoUI[i]->Set_Scale(57.f, 30.f);
			m_pInfoUI[i]->Set_PosX(fPosX - 71.f);
			m_pInfoUI[i]->Set_PosY(15);
			m_pInfoUI[i]->Set_Color(RGB(200, 200, 200));

			break;

		case Info_Point:

			m_pInfoUI[i]->Set_Scale(42.f);
			m_pInfoUI[i]->Set_PosX(fPosX - 120.f);
			m_pInfoUI[i]->Set_PosY(30);

			break;
		}
	}
}

void CUI_Info::Create_InfoText()
{
	for (int i = 0; i < Text_End; ++i)
	{
		m_pInfoText[i] = CUI_Object::Create();

		m_pInfoText[i]->Set_FadeDesc(0.1f, 0.3f, m_fDisableTime, true);
		m_pInfoText[i]->Set_Sort(0.49f);

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
		m_pInfoUI[Info_Point]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Text/Point_Paden0.png"));

		break;

	case Client::LEVEL_HWARA:

		m_pInfoUI[Info_Name]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Text/Text_Hwara.png"));
		m_pInfoUI[Info_Mode]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Text/Text_HwaraInfo.png"));
		m_pInfoUI[Info_Point]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Text/Point_Hwara0.png"));

		break;
	}
}

void CUI_Info::Init_InfoText()
{
	for (int i = 0; i < Text_End; ++i)
	{
		CREATE_GAMEOBJECT(m_pInfoText[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pInfoText[i]);
	}

	switch (m_eLoadLevel)
	{
	case Client::LEVEL_PADEN:

		m_pInfoText[Text_0]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Text/Paden_MapInfo0.png"));
		m_pInfoText[Text_0]->Set_Scale(129.f, 30.f);
		m_pInfoText[Text_0]->Set_PosX(fPosX - 83.f);
		m_pInfoText[Text_0]->Set_PosY(-22.f);
		m_pInfoText[Text_0]->Set_Color(m_vColorGold);

		m_pInfoText[Text_1]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Text/Paden_MapInfo1.png"));
		m_pInfoText[Text_1]->Set_Scale(145.f, 30.f);
		m_pInfoText[Text_1]->Set_PosX(fPosX + 38.f);
		m_pInfoText[Text_1]->Set_PosY(-22.f);

		m_pInfoText[Text_2]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Text/Paden_MapInfo2.png"));
		m_pInfoText[Text_2]->Set_Scale(183.f, 30.f);
		m_pInfoText[Text_2]->Set_PosX(fPosX - 55.f);
		m_pInfoText[Text_2]->Set_PosY(-45.f);
		m_pInfoText[Text_2]->Set_Color(m_vColorGold);

		break;

	case Client::LEVEL_HWARA:

		m_pInfoText[Text_0]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Text/Hwara_MapInfo0.png"));
		m_pInfoText[Text_0]->Set_Scale(99.f, 30.f);
		m_pInfoText[Text_0]->Set_PosX(fPosX - 95.f);
		m_pInfoText[Text_0]->Set_PosY(-22.f);
		m_pInfoText[Text_0]->Set_Color(m_vColorGold);

		m_pInfoText[Text_1]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Text/Hwara_MapInfo1.png"));
		m_pInfoText[Text_1]->Set_Scale(73, 30.f);
		m_pInfoText[Text_1]->Set_PosX(fPosX - 25.f);
		m_pInfoText[Text_1]->Set_PosY(-22.f);

		m_pInfoText[Text_2]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Text/Hwara_MapInfo2.png"));
		m_pInfoText[Text_2]->Set_Scale(171, 30.f);
		m_pInfoText[Text_2]->Set_PosX(fPosX - 59.f);
		m_pInfoText[Text_2]->Set_PosY(-45.f);
		m_pInfoText[Text_2]->Set_Color(m_vColorGold);

		break;
	}
}
