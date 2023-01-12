#define MJDEVELOP

#include "stdafx.h"
#include "CLevel_Test.h"

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

// JJ
#include "CUI_Oper.h"
#include "CUI_Paden.h"
#include "CUI_EscMenu.h"
#include "CUI_Result.h"

#include "CTestObject.h"

CLevel_Test::CLevel_Test()
{
}

CLevel_Test::~CLevel_Test()
{
}

CLevel_Test* CLevel_Test::Create()
{
	CLevel_Test* pLevel = new CLevel_Test();

	pLevel->Initialize();

	return pLevel;
}

HRESULT CLevel_Test::Initialize()
{
	m_fDistance = 200.f;
	m_vSunLook = _float4(-1.f, -1.f, -1.f);
	m_bLensFlare = false;
	return S_OK;
}

HRESULT CLevel_Test::SetUp_Prototypes()
{
	if (FAILED(__super::SetUp_Prototypes()))
		return E_FAIL;

	CSkyBox* pSkyBox = CSkyBox::Create(4);
	if (FAILED(pSkyBox->Initialize()))
		return E_FAIL;
	Ready_GameObject(pSkyBox, GROUP_DEFAULT);


	_float4x4 mat;
	mat.Identity();
	CDrawable_Terrain* pDrawableTerrain = CDrawable_Terrain::Create(100, 100);
	pDrawableTerrain->Initialize();
	Ready_GameObject(pDrawableTerrain, GROUP_DEFAULT);

	//CUI_EscMenu* pUI_Esc = CUI_EscMenu::Create();
	//Ready_GameObject(pUI_Esc, GROUP_UI);

	//CUI_Oper* pUI_Oper = CUI_Oper::Create();
	//Ready_GameObject(pUI_Oper, GROUP_UI);

	CUI_Paden* pUI_Paden = CUI_Paden::Create();
	Ready_GameObject(pUI_Paden, GROUP_UI);

	// CUI_Result* pUI_Result = CUI_Result::Create();
	// Ready_GameObject(pUI_Result, GROUP_UI);

	/* GameSystem */
	if (FAILED(CGameSystem::Get_Instance()->On_ReadyTest(m_vecGameObjects)))
		return E_FAIL;


	//Ready_GameObject(CTestObject::Create(), GROUP_DEFAULT);

	m_fLoadingFinish = 1.f;

	return S_OK;
}

HRESULT CLevel_Test::Enter()
{
	__super::Enter();

	CGameSystem::Get_Instance()->On_EnterTest();
	CGameInstance::Get_Instance()->Change_Camera(L"PlayerCam");

	

	return S_OK;
}

void CLevel_Test::Tick()
{
	__super::Tick();
}

void CLevel_Test::Late_Tick()
{
	__super::Late_Tick();
}

HRESULT CLevel_Test::Render()
{
	__super::Render();

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

HRESULT CLevel_Test::Exit()
{

	return __super::Exit();
}

