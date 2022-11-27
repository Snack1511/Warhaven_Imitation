#include "CLevel_Paden.h"

#include "GameInstance.h"
#include "CDrawable_Terrain.h"
#include "CGameSystem.h"

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

	_float4x4 mat;
	mat.Identity();
	CDrawable_Terrain* pDrawableTerrain = CDrawable_Terrain::Create(100, 100);
	pDrawableTerrain->Initialize();
	Ready_GameObject(pDrawableTerrain, GROUP_DEFAULT);

	/* GameSystem */
	if (FAILED(CGameSystem::Get_Instance()->On_ReadyTest(m_vecGameObjects)))
		return E_FAIL;

	m_fLoadingFinish = 1.f;

	return S_OK;
}

HRESULT CLevel_Paden::Enter()
{
	if (FAILED(__super::Enter()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Paden::Tick()
{
	__super::Tick();
}

void CLevel_Paden::Late_Tick()
{
	__super::Late_Tick();
}

HRESULT CLevel_Paden::Render()
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

HRESULT CLevel_Paden::Exit()
{
	return __super::Exit();;
}
