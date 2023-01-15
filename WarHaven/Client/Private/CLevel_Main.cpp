#include "CLevel_Main.h"
#include "Loading_Manager.h"
#include "GameInstance.h"

#include "CUI_LobbyBG.h"
#include "CUI_Main.h"
#include "CUI_Cursor.h"

#include "ImGui_Manager.h"
#include "CUser.h"

#include "CMainMenuPlayer.h"
#include "CMainMenuUnit.h"

#include "CEffects_Factory.h"

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
	//CEffects_Factory::Get_Instance()->On_EnterLevel();

	CUI_LobbyBG* pUI_Lobby = CUI_LobbyBG::Create();
	Ready_GameObject(pUI_Lobby, GROUP_UI);

	//CUI_Main* pUI_Main = CUI_Main::Create();
	//Ready_GameObject(pUI_Main, GROUP_UI);

	CUser::Get_Instance()->On_EnterLevel();


	// CMainMenuPlayer* pMain = CMainMenuPlayer::Create(CUser::Get_Instance()->Get_MainPlayerInfo());
	// pMain->Initialize();
	// Ready_GameObject(pMain, GROUP_PLAYER);

	LIGHTDESC			LightDesc;

	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = _float4(-1000.8f, -996.54f, -1009.94f);
	LightDesc.fRange = 100.f;
	LightDesc.vDiffuse = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vAmbient = _float4(0.15f, 0.15f, 0.15f, 1.f);
	LightDesc.vSpecular = _float4(0.5f, 0.5f, 0.5f, 1.f);

	if (FAILED(GAMEINSTANCE->Add_Light(LightDesc)))
		return E_FAIL;

	vector<CGameObject*> vecTemp;

	GAMEINSTANCE->Bake_StaticShadow(vecTemp, _float4(-999.f, -999.f, -999.f), 10.f, _float4(1.f, -10.f, 1.f, 0.f), false);

	__super::Enter();

	GAMEINSTANCE->Save_Memory();

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

HRESULT CLevel_Main::OnExit()
{
	CEffects_Factory::Get_Instance()->On_ExitLevel();
	return S_OK;
}
