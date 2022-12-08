#include "CLevel_Paden.h"

#include "GameInstance.h"
#include "CDrawable_Terrain.h"
#include "CGameSystem.h"

// MJ_INCLUDE
#include "CDrawable_Terrain.h"
#include "CStructure_Instance.h"
#include "CUtility_Transform.h"
#include "CMap_Loader.h"
#include "CSkyBox.h"

// JJ

CLevel_Paden::CLevel_Paden()
{
}

CLevel_Paden::~CLevel_Paden()
{
}

CLevel_Paden* CLevel_Paden::Create()
{
	CLevel_Paden* pLevel = new CLevel_Paden();

	pLevel->Initialize();

	return pLevel;
}

HRESULT CLevel_Paden::Initialize()
{
	return S_OK;
}

HRESULT CLevel_Paden::SetUp_Prototypes()
{
	if (FAILED(__super::SetUp_Prototypes()))
		return E_FAIL;

	CSkyBox* pSkyBox = CSkyBox::Create(6);
	if (FAILED(pSkyBox->Initialize()))
		return E_FAIL;

	Ready_GameObject(pSkyBox, GROUP_DEFAULT);
	//_float4x4 mat;
	//mat.Identity();
	//CDrawable_Terrain* pDrawableTerrain = CDrawable_Terrain::Create(100, 100);
	//pDrawableTerrain->Initialize();
	//Ready_GameObject(pDrawableTerrain, GROUP_DEFAULT);

	function<void(CGameObject*, _uint)> Ready_Object = bind(&CLevel_Paden::Ready_GameObject, this, placeholders::_1, placeholders::_2);
	//CMap_Loader::Load_Data(wstring(TEXT("Map_Paden_TerrainOnly")), Ready_Object);
	CMap_Loader::Load_Data(wstring(TEXT("Map_Paden_TerrainOnly")), Ready_Object);
	m_fLoadingFinish = 0.5f;

	/* GameSystem */
	if (FAILED(CGameSystem::Get_Instance()->On_ReadyPaden(m_vecGameObjects)))
		return E_FAIL;

	m_fLoadingFinish = 1.f;

	return S_OK;
}

HRESULT CLevel_Paden::Enter()
{
	if (FAILED(__super::Enter()))
		return E_FAIL;

	if (FAILED(CGameSystem::Get_Instance()->On_EnterStage()))
		return E_FAIL;

	/*if (FAILED(CGameSystem::Get_Instance()->Paden_EnvironmentEffect()))
		return E_FAIL;*/

	return S_OK;
}

void CLevel_Paden::Tick()
{
	__super::Tick();
}

void CLevel_Paden::Late_Tick()
{
	__super::Late_Tick();

	CGameSystem::Get_Instance()->On_Update_Paden();
}

HRESULT CLevel_Paden::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(GAMEINSTANCE->Render_Font(L"DefaultFont", L"Paden Level", _float2(100.f, 30.f), _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

#endif

#ifdef RELEASE_IMGUI

#ifndef _DEBUG
	if (FAILED(GAMEINSTANCE->Render_Font(L"DefaultFont", L"Paden Level", _float2(100.f, 30.f), _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

#endif // !_DEBUG
#endif

	return S_OK;
}

HRESULT CLevel_Paden::Exit()
{
	return __super::Exit();;
}
