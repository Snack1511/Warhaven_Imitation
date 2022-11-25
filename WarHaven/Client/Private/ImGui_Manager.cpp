#include "stdafx.h"
#include "Imgui_Manager.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_dx11.h"
#include "Imgui/imgui_impl_win32.h"

#include "GameInstance.h"

#include "CWindow_Default.h"
#include "CWindow_Select.h"
#include "CWindow_UI.h"
#include "CWindow_Map.h"
#include "CWindow_Effect.h"
#include "CWindow_PhysX.h"
#include "CWindow_Position.h"
#include "CWindow_Level.h"


IMPLEMENT_SINGLETON(CImGui_Manager)

CImGui_Manager::CImGui_Manager()
{
}

CImGui_Manager::~CImGui_Manager()
{
	Release();
}
void CImGui_Manager::Enable_Window(IMGUI_WINDOW_TYPE eType, _bool bEnable)
{
	m_arrWindows[eType]->Set_Enable(bEnable);
}

void CImGui_Manager::Turn_Window(IMGUI_WINDOW_TYPE eType)
{
	_bool bEnable = true;

	if (m_arrWindows[eType]->Is_Enable())
		bEnable = false;

	m_arrWindows[eType]->Set_Enable(bEnable);
}

HRESULT CImGui_Manager::Initialize()
{
	if (m_bIsInit)
	{
		return S_OK;
	}

	m_bIsInit = true;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(DEVICE, DEVICE_CONTEXT);

#pragma region 윈도우 등록
	m_arrWindows[IMGUI_DEFAULT] = CWindow_Default::Create();
	m_arrWindows[IMGUI_SELECT] = CWindow_Select::Create();
	m_arrWindows[IMGUI_UI] = CWindow_UI::Create();
	m_arrWindows[IMGUI_MAPTOOL] = CWindow_Map::Create();
	m_arrWindows[IMGUI_EFFECT] = CWindow_Effect::Create();
	m_arrWindows[IMGUI_PhysX] = CWindow_PhysX::Create();
	m_arrWindows[IMGUI_POSITION] = CWindow_Position::Create();
	m_arrWindows[IMGUI_LEVEL] = CWindow_Level::Create();
#pragma endregion 윈도우 등록

	return S_OK;
}

void CImGui_Manager::Tick()
{
	for (_uint i = 0; i < IMGUI_END; ++i)
	{
		if (nullptr == m_arrWindows[i])
			continue;
		if (m_arrWindows[i]->Is_Enable())
		{
			m_arrWindows[i]->Tick();
		}
	}
}

HRESULT CImGui_Manager::Render(void)
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();


	ImGui::NewFrame();

	for (_uint i = 0; i < IMGUI_END; ++i)
	{
		if (nullptr == m_arrWindows[i])
			continue;
		if (m_arrWindows[i]->Is_Enable())
		{
			if (FAILED(m_arrWindows[i]->Render()))
				return E_FAIL;
		}
	}

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());



	return S_OK;
}

void CImGui_Manager::Release()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	for (_uint i = 0; i < IMGUI_END; ++i)
	{
		SAFE_DELETE(m_arrWindows[i]);
	}
}
