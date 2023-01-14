#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "GameInstance.h"

#include "Loading_Manager.h"

#include "CLevel_Loading.h"
#include "CUser.h"

#include "CCamera_Free.h"
#include "CCamera_Default.h"
#include "CCamera_Follow.h"


#include "ImGui_Manager.h"
#include "CState_Manager.h"
#include "CEffects_Factory.h"
#include "Transform.h"

#include "CGameSystem.h"
#include "CUI_Cursor.h"

#include "CTable_Conditions.h"
#include "CGame_Manager_HR.h"
#include "CGame_Manager_MJ.h"
IMPLEMENT_SINGLETON(CMainApp);

CMainApp::CMainApp()
{
}

CMainApp::~CMainApp()
{
	Release();
}

HRESULT CMainApp::Initialize()
{
#ifdef _DEBUG
	ShowCursor(true);
#else
	ShowCursor(false);
#endif // _DEBUG


	m_pGameInstance = CGameInstance::Get_Instance();

	if (nullptr == m_pGameInstance)
		return E_FAIL;

	if (FAILED(SetUp_Engine()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderFiles()))
		return E_FAIL;

	if (FAILED(SetUp_Font()))
		return E_FAIL;

	if (FAILED(CLoading_Manager::Get_Instance()->Initialize()))
		return E_FAIL;

	if (FAILED(SetUp_Statics()))
		return E_FAIL;

	if (FAILED(Default_Initialize()))
		return E_FAIL;

	if (FAILED(CGameSystem::Get_Instance()->Initialize()))
		return E_FAIL;

	if (FAILED(CState_Manager::Get_Instance()->Initialize()))
		return E_FAIL;

	if (FAILED(CLoading_Manager::Get_Instance()->Reserve_Load_Level(LEVEL_MAINMENU)))
		return E_FAIL;

	//Sound
	if (FAILED(Load_SoundFile()))
		return E_FAIL;


	return S_OK;
}

HRESULT CMainApp::Progress()
{

	if (FAILED(CLoading_Manager::Get_Instance()->Load_Level()))
	{
		Call_MsgBox(L"Failed to Load_Level : CLoading_Manager");
		return E_FAIL;
	}


	if (FAILED(m_pGameInstance->Tick_Engine()))
	{
		Call_MsgBox(L"Failed to Tick_Engine : CMainApp");
		return E_FAIL;
	}

	if (FAILED(Render()))
	{
		Call_MsgBox(L"Failed to Render : CMainApp");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainApp::Render()
{
	if (!m_pGameInstance->Can_Update())
		return S_OK;

	if (FAILED(m_pGameInstance->Clear_BackBuffer_View(_float4(0.9f, 0.9f, 0.9f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Clear_DepthStencil_View()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Render_Engine()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Present()))
		return E_FAIL;


	return S_OK;
}

void CMainApp::Release()
{
	CEffects_Factory::Get_Instance()->Destroy_Instance();
	
	CLoading_Manager::Get_Instance()->Destroy_Instance();
	CUser::Get_Instance()->Destroy_Instance();
	CGame_Manager_HR::Get_Instance()->Destroy_Instance();
	CGame_Manager_MJ::Get_Instance()->Destroy_Instance();

	CGameSystem::Get_Instance()->Destroy_Instance();


#ifdef _DEBUG
	CImGui_Manager::Get_Instance()->Destroy_Instance();


#endif

#ifdef RELEASE_IMGUI
#ifndef _DEBUG

	CImGui_Manager::Get_Instance()->Destroy_Instance();


#endif
#endif



	CState_Manager::Get_Instance()->Destroy_Instance();
	SAFE_DESTROY(m_pGameInstance);

}

HRESULT CMainApp::Default_Initialize()
{
	Col_Check();

	if (FAILED(CEffects_Factory::Get_Instance()->Initialize()))
		return E_FAIL;

	CUser::Get_Instance()->Initialize();

#ifdef _DEBUG
	if (FAILED(CImGui_Manager::Get_Instance()->Initialize()))
		return E_FAIL;
#endif

#ifdef RELEASE_IMGUI

#ifndef _DEBUG
	if (FAILED(CImGui_Manager::Get_Instance()->Initialize()))
		return E_FAIL;
#endif // !_DEBUG


#endif



	return S_OK;
}

HRESULT CMainApp::SetUp_Engine()
{
	GRAPHICDESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.hInst = g_hInst;
	GraphicDesc.iWinCX = g_iWinCX;
	GraphicDesc.iWinCY = g_iWinCY;
	GraphicDesc.WinMode = GRAPHICDESC::MODE_WIN;

	SOUNDDESC	SoundDesc;
	SoundDesc.iChannelNumbers[CHANNEL_BGM] = 1;
	SoundDesc.iChannelNumbers[CHANNEL_UI] = 4;
	SoundDesc.iChannelNumbers[CHANNEL_ENVIRONMENT] = 9;
	SoundDesc.iChannelNumbers[CHANNEL_EFFECTS] = 13;
	SoundDesc.iChannelNumbers[CHANNEL_VOICE] = 5;

	TIMEDESC	TimeDesc;
	TimeDesc.iNumTimeScale = 1;

	if (FAILED(m_pGameInstance->Initialize_Engine(GraphicDesc, SoundDesc, TimeDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::SetUp_Levels()
{
	return S_OK;
}

HRESULT CMainApp::SetUp_Statics()
{
	CUI_Cursor* pCursor = CUI_Cursor::Create();
	pCursor->Initialize();
	CREATE_STATIC(pCursor, HASHCODE(CUI_Cursor));
	DISABLE_GAMEOBJECT(pCursor);
	CUser::Get_Instance()->Set_Cursor(pCursor);

	/*Default Cam*/
	CCamera* pDefaultCam = CCamera_Default::Create();
	pDefaultCam->Initialize();
	CREATE_STATIC(pDefaultCam, HASHCODE(CCamera_Default));
	CGameInstance::Get_Instance()->Add_Camera(L"DefaultCam", pDefaultCam);
	CGameInstance::Get_Instance()->Change_Camera(L"DefaultCam");


	/* Free Camera */
	CCamera_Free* pFreeCam = CCamera_Free::Create();
	pFreeCam->Initialize();
	CREATE_STATIC(pFreeCam, HASHCODE(CCamera_Free));
	DISABLE_GAMEOBJECT(pFreeCam);
	CGameInstance::Get_Instance()->Add_Camera(L"FreeCam", pFreeCam);



	return S_OK;
}

HRESULT CMainApp::SetUp_ShaderFiles()
{
	if (FAILED(CGameInstance::Get_Instance()->Load_EffectFile(L"../bin/shaderfiles/Shader_VtxTex.hlsl")))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Load_EffectFile(L"../bin/shaderfiles/Shader_VtxCubeTex.hlsl")))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Load_EffectFile(L"../bin/shaderfiles/Shader_VtxnorTex.hlsl")))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Load_EffectFile(L"../bin/shaderfiles/Shader_VtxModel.hlsl")))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Load_EffectFile(L"../bin/shaderfiles/Shader_VtxAnimModel.hlsl")))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Load_EffectFile(L"../bin/shaderfiles/Shader_VtxInstance.hlsl")))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Load_EffectFile(L"../bin/shaderfiles/Shader_VtxRectInstance.hlsl")))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Load_EffectFile(L"../bin/shaderfiles/Shader_VtxTriInstance.hlsl")))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Load_EffectFile(L"../bin/shaderfiles/Shader_VtxModelEffects.hlsl")))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Load_EffectFile(L"../bin/shaderfiles/Shader_VtxDecalCube.hlsl")))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::SetUp_Font()
{
	if (FAILED(CGameInstance::Get_Instance()->Add_Font(TEXT("WarKR_Regular"), TEXT("../Bin/Resources/Fonts/WarhavenKR_Regular.spritefont"))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Font(TEXT("WarKR_Bold"), TEXT("../Bin/Resources/Fonts/WarhavenKR_Bold.spritefont"))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Font(TEXT("War_Regular"), TEXT("../Bin/Resources/Fonts/WarHavenRegular.spritefont"))))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Font(TEXT("War_Bold"), TEXT("../Bin/Resources/Fonts/WarHavenBold.spritefont"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(L"DefaultFont", L"../bin/resources/fonts/128.spritefont")))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Load_SoundFile()
{
	if (FAILED(m_pGameInstance->Load_SoundFile(L"../bin/resources/sounds/BGM")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_SoundFile(L"../bin/resources/sounds/UI")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_SoundFile(L"../bin/resources/sounds/Env")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_SoundFile(L"../bin/resources/sounds/Effect")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_SoundFile(L"../bin/resources/sounds/Voice/Warrior")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_SoundFile(L"../bin/resources/sounds/Voice/Warhammer")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_SoundFile(L"../bin/resources/sounds/Voice/Paladin")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_SoundFile(L"../bin/resources/sounds/Voice/Archer")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_SoundFile(L"../bin/resources/sounds/Voice/Priest")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_SoundFile(L"../bin/resources/sounds/Voice/Valkyrie")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_SoundFile(L"../bin/resources/sounds/Voice/Qanda")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_SoundFile(L"../bin/resources/sounds/Voice/Lancer")))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Col_Check()
{
	/* TRIGGER */
	GAMEINSTANCE->Check_Group(COL_BLUETEAM, COL_TRIGGER);
	GAMEINSTANCE->Check_Group(COL_REDTEAM, COL_TRIGGER);

	/* Cannon */
	GAMEINSTANCE->Check_Group(COL_REDTEAM, COL_CANNON);
	GAMEINSTANCE->Check_Group(COL_BLUETEAM, COL_CANNON);

	/* REVIVE */
	GAMEINSTANCE->Check_Group(COL_BLUETEAM, COL_REVIVE);
	GAMEINSTANCE->Check_Group(COL_REDTEAM, COL_REVIVE);

	GAMEINSTANCE->Check_Group(COL_BLUETEAM, COL_BLUECURE);
	GAMEINSTANCE->Check_Group(COL_REDTEAM, COL_BLUECURE);

	GAMEINSTANCE->Check_Group(COL_BLUETEAM, COL_REDCURE);
	GAMEINSTANCE->Check_Group(COL_REDTEAM, COL_REDCURE);

	/* Cure */
	//GAMEINSTANCE->Check_Group(COL_BLUECURE, COL_BLUEHITBOX_BODY);
	//GAMEINSTANCE->Check_Group(COL_BLUECURE, COL_REDHITBOX_BODY);
	//GAMEINSTANCE->Check_Group(COL_REDCURE, COL_REDHITBOX_BODY);
	//GAMEINSTANCE->Check_Group(COL_REDCURE, COL_BLUEHITBOX_BODY);

	//GAMEINSTANCE->Check_Group(COL_BLUEHITBOX_BODY, COL_REDHITBOX_BODY);
	//GAMEINSTANCE->Check_Group(COL_REDHITBOX_BODY, COL_BLUEHITBOX_BODY);

	//GAMEINSTANCE->Check_Group(COL_BLUECURE, COL_BLUEHITBOX_BODY);
	//GAMEINSTANCE->Check_Group(COL_BLUECURE, COL_REDHITBOX_BODY);
	//GAMEINSTANCE->Check_Group(COL_REDCURE, COL_REDHITBOX_BODY);
	//GAMEINSTANCE->Check_Group(COL_REDCURE, COL_BLUEHITBOX_BODY);


	//GAMEINSTANCE->Check_Group(COL_REDTEAM, COL_BLUECURE);
	//GAMEINSTANCE->Check_Group(COL_BLUETEAM, COL_BLUECURE);
	//GAMEINSTANCE->Check_Group(COL_REDTEAM, COL_REDCURE);
	//GAMEINSTANCE->Check_Group(COL_BLUETEAM, COL_REDCURE);



	/* 일반 공격 플레이어 */
	GAMEINSTANCE->Check_Group(COL_BLUEATTACK, COL_REDHITBOX_BODY);
	GAMEINSTANCE->Check_Group(COL_BLUEATTACK, COL_REDHITBOX_HEAD);

	/* 가드 공격 플레이어 */
	GAMEINSTANCE->Check_Group(COL_BLUEATTACK, COL_REDGUARD);

	/*가드 브레이크 플레이어 */
	GAMEINSTANCE->Check_Group(COL_BLUEGUARDBREAK, COL_REDGUARD);
	GAMEINSTANCE->Check_Group(COL_BLUEGUARDBREAK, COL_REDHITBOX_BODY);

	/* 그로기 공격 플레이어 */
	GAMEINSTANCE->Check_Group(COL_BLUEGROGGYATTACK, COL_REDHITBOX_BODY);
	GAMEINSTANCE->Check_Group(COL_BLUEGROGGYATTACK, COL_REDGUARD);
	GAMEINSTANCE->Check_Group(COL_BLUEGROGGYATTACK, COL_REDATTACK);


	/* 띄우기 공격 플레이어 */
	GAMEINSTANCE->Check_Group(COL_BLUEFLYATTACK, COL_REDHITBOX_BODY);
	GAMEINSTANCE->Check_Group(COL_BLUEFLYATTACK, COL_REDGUARD);

	/* 가드 불가 띄우기 공격 플레이어 */
	GAMEINSTANCE->Check_Group(COL_BLUEFLYATTACKGUARDBREAK, COL_REDHITBOX_BODY);
	GAMEINSTANCE->Check_Group(COL_BLUEFLYATTACKGUARDBREAK, COL_REDGUARD);


	/* 스모크가  투사체를 캐치할 수 있도록 설정 */
	GAMEINSTANCE->Check_Group(COL_REDPROJECTILECATCH, COL_BLUEATTACK);
	GAMEINSTANCE->Check_Group(COL_REDPROJECTILECATCH, COL_BLUEGUARDBREAK);
	GAMEINSTANCE->Check_Group(COL_REDPROJECTILECATCH, COL_BLUEGROGGYATTACK);
	GAMEINSTANCE->Check_Group(COL_REDPROJECTILECATCH, COL_BLUEFLYATTACK);
	GAMEINSTANCE->Check_Group(COL_REDPROJECTILECATCH, COL_BLUEFLYATTACKGUARDBREAK);
	
	GAMEINSTANCE->Check_Group(COL_BLUEPROJECTILECATCH, COL_REDATTACK);
	GAMEINSTANCE->Check_Group(COL_BLUEPROJECTILECATCH, COL_REDGUARDBREAK);
	GAMEINSTANCE->Check_Group(COL_BLUEPROJECTILECATCH, COL_REDGROGGYATTACK);
	GAMEINSTANCE->Check_Group(COL_BLUEPROJECTILECATCH, COL_REDFLYATTACK);
	GAMEINSTANCE->Check_Group(COL_BLUEPROJECTILECATCH, COL_REDFLYATTACKGUARDBREAK);

	/* 일반 공격 적 */
	GAMEINSTANCE->Check_Group(COL_REDATTACK, COL_BLUEHITBOX_BODY);
	GAMEINSTANCE->Check_Group(COL_REDATTACK, COL_BLUEHITBOX_HEAD);

	/* 가드 공격 적 */
	GAMEINSTANCE->Check_Group(COL_REDATTACK, COL_BLUEGUARD);

	/*가드 브레이크 적 */
	GAMEINSTANCE->Check_Group(COL_REDGUARDBREAK, COL_BLUEGUARD);
	GAMEINSTANCE->Check_Group(COL_REDGUARDBREAK, COL_BLUEHITBOX_BODY);

	/* 그로기 공격 적 */
	GAMEINSTANCE->Check_Group(COL_REDGROGGYATTACK, COL_BLUEHITBOX_BODY);
	GAMEINSTANCE->Check_Group(COL_REDGROGGYATTACK, COL_BLUEGUARD);
	GAMEINSTANCE->Check_Group(COL_REDGROGGYATTACK, COL_BLUEATTACK);

	/* 띄우기 공격 적 */
	GAMEINSTANCE->Check_Group(COL_REDFLYATTACK, COL_BLUEHITBOX_BODY);
	GAMEINSTANCE->Check_Group(COL_REDFLYATTACK, COL_BLUEGUARD);


	/* 가드 불가 띄우기 공격 적 */
	GAMEINSTANCE->Check_Group(COL_REDFLYATTACKGUARDBREAK, COL_BLUEHITBOX_BODY);
	GAMEINSTANCE->Check_Group(COL_REDFLYATTACKGUARDBREAK, COL_BLUEGUARD);

	/* 가드 불가 띄우기 공격 적 */
	GAMEINSTANCE->Check_Group(COL_SIGHTRANGE, COL_BLUETEAM);
	GAMEINSTANCE->Check_Group(COL_SIGHTRANGE, COL_REDTEAM);
	GAMEINSTANCE->Check_Group(COL_SIGHTRANGE, COL_TRIGGER);
}
