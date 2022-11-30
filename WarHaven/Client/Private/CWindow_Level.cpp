#include "stdafx.h"
#include "CWindow_Level.h"

#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "Loading_Manager.h"
#include "CGame_Manager_HR.h"

CWindow_Level::CWindow_Level()
{
}

CWindow_Level::~CWindow_Level()
{
	CGame_Manager_HR::Destroy_Instance();
}

CWindow_Level* CWindow_Level::Create()
{
	CWindow_Level* pInstance = new CWindow_Level;

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWindow_Level");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CWindow_Level::Initialize()
{
	ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoResize;

	m_bEnable = true;
	SetUp_ImGuiDESC(typeid(CWindow_Level).name(), ImVec2(200.f, 300.f), window_flags);

	CGame_Manager_HR::Get_Instance()->Initialize();

	return S_OK;
}

void CWindow_Level::Tick()
{
}

HRESULT CWindow_Level::Render()
{
	if (FAILED(__super::Begin()))
		return E_FAIL;

	ImGui::Text("= LEVEL CHANGE =");
	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);


	for (_uint i = 0; i < LEVEL_TYPE_CLIENT::LEVEL_END; ++i)
	{
		if (i == 1)
			continue;

		string strName;

		switch (i)
		{
		case 0:
			strName = "LOGO_LEVEL";
			break;

		case 2:
			strName = "MAINMENU";
			break;

		case 3:
			strName = "TEST";
			break;

		case 4:
			strName = "BOOTCAMP";
			break;

		case 5:
			strName = "PADEN";
			break;

		default:
			break;
		}

		if (ImGui::Button(strName.c_str()))
		{
			if ((LEVEL_TYPE_CLIENT)2 == CGame_Manager_HR::Get_Instance()->Get_CurrentLevel()) //메인메뉴일때만 이동
			{
				CLoading_Manager::Get_Instance()->Reserve_Load_Level((LEVEL_TYPE_CLIENT)i);
				CGame_Manager_HR::Get_Instance()->Set_CurrentLevel((LEVEL_TYPE_CLIENT)i);
			}
			else
			{
				CLoading_Manager::Get_Instance()->Reserve_Load_Level((LEVEL_TYPE_CLIENT)2);
				CGame_Manager_HR::Get_Instance()->Set_CurrentLevel((LEVEL_TYPE_CLIENT)2);
			}
		}

	}


	__super::End();

	return S_OK;
}
