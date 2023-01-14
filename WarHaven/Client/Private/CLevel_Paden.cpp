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
#include "CUI_Oper.h"
#include "CTrigger.h"
#include "Transform.h"
#include "CUser.h"
#include "CUI_MiniMap.h"

#include "Functor.h"


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
	m_vCenterPos = ZERO_VECTOR;
	m_fDistance = 140.f;
	m_vSunLook = _float4(-0.5f, -1.5f, 2.f);

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

	function<void(CGameObject*, _uint)> Ready_Object = bind(&CLevel_Paden::Ready_GameObject, this, placeholders::_1, placeholders::_2);

#ifdef _DEBUG
		CMap_Loader::Load_Data(wstring(TEXT("Map_Paden_TerrainOnly")), Ready_Object);
#else
		//CMap_Loader::Load_Data(wstring(TEXT("Map_Paden_TerrainOnly")), Ready_Object);
		CMap_Loader::Load_Data(wstring(TEXT("Map_Paden")), Ready_Object);

#endif


	m_fLoadingFinish = 0.5f;

	/* GameSystem */
	if (FAILED(CGameSystem::Get_Instance()->SetUp_CellLayer(wstring(TEXT("Map_Paden")))))
		return E_FAIL;

	if (FAILED(CGameSystem::Get_Instance()->On_ReadyPaden(m_vecGameObjects)))
		return E_FAIL;

	m_fLoadingFinish = 1.f;

	return S_OK;
}

HRESULT CLevel_Paden::Enter()
{
	if (FAILED(__super::Enter()))
		return E_FAIL;

	CUser::Get_Instance()->SetActive_OperUI(true);

	if (FAILED(CGameSystem::Get_Instance()->On_EnterStage()))
		return E_FAIL;

#ifdef _DEBUG
#else
	if (FAILED(CGameSystem::Get_Instance()->Paden_EnvironmentEffect()))
		return E_FAIL;

#endif 

	GAMEINSTANCE->Set_ChannelVolume((CHANNEL_GROUP)CHANNEL_BGM, 0.1f);
	GAMEINSTANCE->Set_ChannelVolume((CHANNEL_GROUP)CHANNEL_UI, 1.f);
	GAMEINSTANCE->Set_ChannelVolume((CHANNEL_GROUP)CHANNEL_VOICE, 10.f);
	GAMEINSTANCE->Set_ChannelVolume((CHANNEL_GROUP)CHANNEL_ENVIRONMENT, 1.f);

	return S_OK;
}

void CLevel_Paden::Tick()
{
	BirdSound();



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
	return S_OK;
}

HRESULT CLevel_Paden::Exit()
{
	return __super::Exit();;
}

void CLevel_Paden::BirdSound()
{
	m_fBirdTimeAcc += fDT(0);

	if (m_fBird <= m_fBirdTimeAcc)
	{
		CFunctor::Play_Sound(L"Env_Ggugi", CHANNEL_ENVIRONMENT, _float4(48.6f, 5.5f, -0.f), 1.f);
		
		m_fBirdTimeAcc = 0.f;
	}
}

void CLevel_Paden::WindSound()
{
	m_fWindTimeAcc += fDT(0);

	if (18.f <= m_fWindTimeAcc)
	{
		CFunctor::Play_Sound(L"Env_CannonWind", CHANNEL_ENVIRONMENT, _float4(-61.8f, 20.4f, 0.2f), 10.f);
		m_fWindTimeAcc = 0.f;
	}
}
