#include "stdafx.h"
#include "CLevel_Loading.h"

#include "GameInstance.h"

#include "Loading_Manager.h"

#include "CFader.h"
#include "CShader.h"
#include "Renderer.h"
#include "Physics.h"

#include "Functor.h"

CLevel_Loading::CLevel_Loading()
{
}

CLevel_Loading* CLevel_Loading::Create()
{
	CLevel_Loading* pInstance = new CLevel_Loading();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CLevel_Loading");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CLevel_Loading::Initialize()
{

	return S_OK;
}

HRESULT CLevel_Loading::SetUp_Prototypes()
{
	return S_OK;
}

HRESULT CLevel_Loading::Enter()
{
	//GAMEINSTANCE->Stop_Sound((CHANNEL_GROUP)CHANNEL_BGM);

	//로딩 화면 객체 생성

	__super::Enter();


	return S_OK;
}

void CLevel_Loading::Tick()
{
	if (true == CLoading_Manager::Get_Instance()->IsFinished())
	{
		if (KEY(ENTER, TAP))
		{
			CLoading_Manager::Get_Instance()->Open_NextLevel();
		}
	}
}

void CLevel_Loading::Late_Tick()
{
}

HRESULT CLevel_Loading::Render()
{

#ifdef _DEBUG
	_tchar	szTemp[MIN_STR] = L"";
	_float fLoadingFinish = CLoading_Manager::Get_Instance()->Get_Level(m_eNextLevel)->Get_LoadingFinish();
	swprintf_s(szTemp, L"로딩중 : %f", fLoadingFinish);

	if (FAILED(GAMEINSTANCE->Render_Font(L"DefaultFont", szTemp, _float2(100.f, 30.f), _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

#endif

	return S_OK;
}

HRESULT CLevel_Loading::Exit()
{
	CGameInstance::Get_Instance()->Delete_Objects(GROUP_LOADING);
	CGameInstance::Get_Instance()->Clear_All_Components();

	__super::Exit();

	return S_OK;
}



