#include "CLevel_Hwara.h"

#include "GameInstance.h"
#include "CDrawable_Terrain.h"
#include "CGameSystem.h"
#include "CSkyBox.h"

CLevel_Hwara::CLevel_Hwara()
{
}

CLevel_Hwara::~CLevel_Hwara()
{
}

CLevel_Hwara* CLevel_Hwara::Create()
{
    CLevel_Hwara* pLevel = new CLevel_Hwara();

    pLevel->Initialize();

    return pLevel;
}

HRESULT CLevel_Hwara::Initialize()
{
    return S_OK;
}

HRESULT CLevel_Hwara::SetUp_Prototypes()
{
	if (FAILED(__super::SetUp_Prototypes()))
		return E_FAIL;

	CSkyBox* pSkyBox = CSkyBox::Create(6);
	if (FAILED(pSkyBox->Initialize()))
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

HRESULT CLevel_Hwara::Enter()
{
	if (FAILED(__super::Enter()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Hwara::Tick()
{
	__super::Tick();
}

void CLevel_Hwara::Late_Tick()
{
	__super::Late_Tick();
}

HRESULT CLevel_Hwara::Render()
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

HRESULT CLevel_Hwara::Exit()
{
	return __super::Exit();;
}
