#include "stdafx.h"
#include "CWindow_Select.h"

#include "ImGui_Manager.h"

CWindow_Select::CWindow_Select()
{
}

CWindow_Select::~CWindow_Select()
{
}

CWindow_Select* CWindow_Select::Create()
{
	CWindow_Select* pInstance = new CWindow_Select;

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWindow_Select");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CWindow_Select::Initialize()
{
	ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoResize;

	m_bEnable = true;
	SetUp_ImGuiDESC(typeid(CWindow_Select).name(), ImVec2(200.f, 300.f), window_flags);

	return S_OK;
}

void CWindow_Select::Tick()
{
}

HRESULT CWindow_Select::Render()
{
	if (FAILED(__super::Begin()))
		return E_FAIL;

	ImGui::Text("= MAIN_MENU =");
	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	if (ImGui::Button("FPS"))
	{
		CImGui_Manager::Get_Instance()->Turn_Window(CImGui_Manager::IMGUI_DEFAULT);
	}
	ImGui::Spacing();

	if (ImGui::Button("Map Tool"))
	{
		CImGui_Manager::Get_Instance()->Turn_Window(CImGui_Manager::IMGUI_MAPTOOL);
	}
	ImGui::Spacing();

	__super::End();

	return S_OK;
}
