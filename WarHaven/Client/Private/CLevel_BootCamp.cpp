#define MJDEVELOP

#include "stdafx.h"
#include "CLevel_BootCamp.h"

#include "GameInstance.h"

#include "CUser.h"

#include "ImGui_Manager.h"

#include "Transform.h"
#include "CSkyBox.h"
#include "CTerrain.h"

#include "CPlayer.h"

#include "CUnit_Warrior.h"
#include "CUnit_Spearman.h"
#include "CUnit_WarHammer.h"
#include "CUnit_Valkyrie.h"

#include "CBarricade.h"

#include "CCamera_Free.h"
#include "CCamera_Follow.h"

#include "CSword_Effect.h"
#include "CEffects_Factory.h"
#include "CBloodOverlay.h"

#include "Model.h"

// MJ_INCLUDE
#include "CDrawable_Terrain.h"
#include "CStructure_Instance.h"
#include "CUtility_Transform.h"
#include "CMap_Loader.h"
// YJ
#include "CDebugObject.h"

#include "CPhysXCharacter.h"
#include "CStructure.h"

#include "CMapColliders.h"

#include "CGameSystem.h"

CLevel_BootCamp::CLevel_BootCamp()
{
}

CLevel_BootCamp::~CLevel_BootCamp()
{
}

CLevel_BootCamp* CLevel_BootCamp::Create()
{
	CLevel_BootCamp* pLevel = new CLevel_BootCamp();

	pLevel->Initialize();

	return pLevel;
}

HRESULT CLevel_BootCamp::Initialize()
{
	m_vCenterPos = _float4(0.f, 0.f, -50.f);

	return S_OK;
}

HRESULT CLevel_BootCamp::SetUp_Prototypes()
{
	if (FAILED(__super::SetUp_Prototypes()))
		return E_FAIL;

	CSkyBox* pSkyBox = CSkyBox::Create(5);
	if (FAILED(pSkyBox->Initialize()))
		return E_FAIL;
	Ready_GameObject(pSkyBox, GROUP_DEFAULT);

	function<void(CGameObject*, _uint)> Ready_Object = bind(&CLevel_BootCamp::Ready_GameObject, this, placeholders::_1, placeholders::_2);
	CMap_Loader::Load_Data(wstring(TEXT("TrainingRoom01")), Ready_Object);
	m_fLoadingFinish = 0.5f;

	if (FAILED(CGameSystem::Get_Instance()->On_ReadyBootCamp(m_vecGameObjects)))
		return E_FAIL;

	m_fLoadingFinish = 1.f;

	return S_OK;
}

HRESULT CLevel_BootCamp::Enter()
{
	if (FAILED(__super::Enter()))
		return E_FAIL;

	/* GameSystem */
	if (FAILED(CGameSystem::Get_Instance()->On_EnterBootCamp()))
		return E_FAIL;
	CGameInstance::Get_Instance()->Change_Camera(L"PlayerCam");

	//CUser::Get_Instance()->Start_FadeDark(0.f, 0.1f, 0.3f);

	GAMEINSTANCE->Stop_DarkScreen(1.5f);

	return S_OK;
}

void CLevel_BootCamp::Tick()
{
	__super::Tick();
}

void CLevel_BootCamp::Late_Tick()
{
	__super::Late_Tick();
}

HRESULT CLevel_BootCamp::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


#ifdef _DEBUG
	if (FAILED(GAMEINSTANCE->Render_Font(L"DefaultFont", L"Test Level", _float2(100.f, 30.f), _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

#endif

#ifdef RELEASE_IMGUI

#ifndef _DEBUG
	if (FAILED(GAMEINSTANCE->Render_Font(L"DefaultFont", L"Test Level", _float2(100.f, 30.f), _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

#endif // !_DEBUG
#endif

	return S_OK;
}

HRESULT CLevel_BootCamp::Exit()
{
	return __super::Exit();;
}
