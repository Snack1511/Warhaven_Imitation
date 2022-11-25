#include "CLevel_Main.h"
#include "Loading_Manager.h"
#include "GameInstance.h"

#include "CUI_LobbyBG.h"
#include "CUI_Main.h"
#include "CUI_Cursor.h"

#include "ImGui_Manager.h"
#include "CUser.h"
CLevel_Main::CLevel_Main()
{
}

CLevel_Main::~CLevel_Main()
{
}

CLevel_Main* CLevel_Main::Create()
{
	CLevel_Main* pLevel = new CLevel_Main();

	pLevel->Initialize();

	return pLevel;
}

HRESULT CLevel_Main::Initialize()
{
	return S_OK;
}

HRESULT CLevel_Main::SetUp_Prototypes()
{
	m_fLoadingFinish = 1.f;

	return S_OK;
}

HRESULT CLevel_Main::Enter()
{
	CUI_LobbyBG* pUI_Lobby = CUI_LobbyBG::Create();
	Ready_GameObject(pUI_Lobby, GROUP_UI);

	CUI_Main* pUI_Main = CUI_Main::Create();
	Ready_GameObject(pUI_Main, GROUP_UI);

	CUser::Get_Instance()->On_EnterLevel();


	__super::Enter();

	return S_OK;
}

void CLevel_Main::Tick()
{
#ifdef _DEBUG
	CImGui_Manager::Get_Instance()->Tick();

#endif

#ifdef RELEASE_IMGUI

#ifndef _DEBUG
	CImGui_Manager::Get_Instance()->Tick();

#endif // !_DEBUG
#endif

	if (true == CLoading_Manager::Get_Instance()->IsFinished())
	{
	}
}

void CLevel_Main::Late_Tick()
{
}

HRESULT CLevel_Main::Render()
{
#ifdef _DEBUG
	if (FAILED(CImGui_Manager::Get_Instance()->Render()))
		return E_FAIL;

#endif

#ifdef RELEASE_IMGUI

#ifndef _DEBUG
	if (FAILED(CImGui_Manager::Get_Instance()->Render()))
		return E_FAIL;

#endif // !_DEBUG
#endif

	return S_OK;
}

HRESULT CLevel_Main::Exit()
{
	__super::Exit();
	CUser::Get_Instance()->On_ExitLevel();

	return S_OK;
}
