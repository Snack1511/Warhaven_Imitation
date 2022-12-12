#include "CLevel_Main.h"
#include "Loading_Manager.h"
#include "GameInstance.h"

#include "CUI_LobbyBG.h"
#include "CUI_Main.h"
#include "CUI_Cursor.h"

#include "ImGui_Manager.h"
#include "CUser.h"

#include "CMainMenuUnit.h"

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


	CUnit::UNIT_MODEL_DATA tData =
	{
		L"../bin/resources/meshes/characters/Warrior/Warrior.fbx",
		L"../bin/resources/meshes/characters/Warrior/Body/SK_Warrior0001_Body_A00.fbx",
		L"../bin/resources/meshes/characters/Warrior/Head/SK_Warrior0001_Face_A00.fbx",
		L"../bin/resources/meshes/characters/Warrior/Head/SK_Warrior0002_Helmet_A00_Main.fbx",
		L"../bin/resources/meshes/weapons/longsword/SM_WP_LongSword0001_A00.fbx"

	};

	CMainMenuUnit* pMainMenuUnit = CMainMenuUnit::Create(tData);
	if (!pMainMenuUnit)
		return E_FAIL;
	pMainMenuUnit->Initialize();
	Ready_GameObject(pMainMenuUnit, GROUP_PLAYER);

	LIGHTDESC			LightDesc;

	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = _float4(-999.f, -999.f, -999.f);
	LightDesc.vPosition.y += 1.f;
	LightDesc.vPosition.z += 2.f;
	LightDesc.vPosition.x += 0.f;
	LightDesc.fRange = 30.f;
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(GAMEINSTANCE->Add_Light(LightDesc)))
		return E_FAIL;


	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = _float4(-999.f, -999.f, -999.f);
	LightDesc.vPosition.y += 1.f;
	LightDesc.vPosition.z -= 1.f;
	LightDesc.vPosition.x += 1.f;
	LightDesc.fRange = 30.f;
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(GAMEINSTANCE->Add_Light(LightDesc)))
		return E_FAIL;



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
