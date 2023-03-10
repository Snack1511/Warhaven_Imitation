#include "CLevel_Hwara.h"

#include "GameInstance.h"
#include "CDrawable_Terrain.h"
#include "CGameSystem.h"
#include "CSkyBox.h"

#include "CMap_Loader.h"
#include "CUser.h"

#include "CUnit.h"
#include "CPlayer.h"
#include "Functor.h"

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
	m_vCenterPos = ZERO_VECTOR;
	m_vCenterPos.y -= 10.f;
	m_fDistance = 250.f;
	m_vSunLook = _float4(1.f, -2.f, 1.f);

    return S_OK;
}

HRESULT CLevel_Hwara::SetUp_Prototypes()
{
	if (FAILED(__super::SetUp_Prototypes()))
		return E_FAIL;

	CSkyBox* pSkyBox = CSkyBox::Create(7);
	if (FAILED(pSkyBox->Initialize()))
		return E_FAIL;

	Ready_GameObject(pSkyBox, GROUP_DEFAULT);
	function<void(CGameObject*, _uint)> Ready_Object = bind(&CLevel_Hwara::Ready_GameObject, this, placeholders::_1, placeholders::_2);

	CMap_Loader::Load_Data(wstring(TEXT("Map_Hwara")), Ready_Object);

	m_fLoadingFinish = 0.5f;

	/* GameSystem */
	if (FAILED(CGameSystem::Get_Instance()->On_ReadyHwara(m_vecGameObjects)))
		return E_FAIL;

	m_fLoadingFinish = 1.f;

	return S_OK;
}

HRESULT CLevel_Hwara::Enter()
{
	if (FAILED(__super::Enter()))
		return E_FAIL;

	CUser::Get_Instance()->SetActive_OperUI(true);

	if (FAILED(CGameSystem::Get_Instance()->On_EnterStage()))
		return E_FAIL;

	PLAYER->Get_OwnerPlayer()->Set_MainPlayerStartPath(0);

#ifdef _DEBUG
#else
	if (FAILED(CGameSystem::Get_Instance()->Hwara_EnvironmentEffect()))
		return E_FAIL;

#endif 


	return S_OK;
}

void CLevel_Hwara::Tick()
{
	__super::Tick();
}

void CLevel_Hwara::Late_Tick()
{
	__super::Late_Tick();

	CGameSystem::Get_Instance()->On_Update_Hwara();

}

HRESULT CLevel_Hwara::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hwara::Exit()
{
	return __super::Exit();;
}
