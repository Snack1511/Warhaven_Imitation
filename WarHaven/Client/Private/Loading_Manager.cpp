#include "stdafx.h"
#include "Loading_Manager.h"

#include "CLevel_Logo.h"
#include "CLevel_Loading.h"
#include "CLevel_Main.h"
#include "CLevel_Test.h"
#include "GameInstance.h"
#include "CLevel_BootCamp.h"
#include "CLevel_Paden.h"
#include "CShader.h"

IMPLEMENT_SINGLETON(CLoading_Manager)

CLoading_Manager::CLoading_Manager()
{
	ZeroMemory(m_arrLevels, sizeof(CLevel) * LEVEL_END);
}

CLoading_Manager::~CLoading_Manager()
{
	Release();
}

unsigned int APIENTRY LoadingMain(void* pArg)
{
	CoInitializeEx(nullptr, 0);

	CLoading_Manager*		pLoadingMgr = (CLoading_Manager*)pArg;
	CRITICAL_SECTION	CS = pLoadingMgr->Get_CS();
	EnterCriticalSection(&CS);

	HRESULT hr = 0;
	if (FAILED(pLoadingMgr->Start_Loading()))
		Call_MsgBox(L"Failed to Start_Loading : CLoad_Manager");

	LeaveCriticalSection(&CS);

	return 0;
}


HRESULT CLoading_Manager::Initialize()
{
	for (_uint i = LEVEL_LOGO; i < LEVEL_END; ++i)
	{
		m_arrLevels[i] = nullptr;
	}

	m_arrLevels[LEVEL_LOGO] = CLevel_Logo::Create();
	m_arrLevels[LEVEL_LOADING] = CLevel_Loading::Create();
	m_arrLevels[LEVEL_MAINMENU] = CLevel_Main::Create();
	m_arrLevels[LEVEL_TEST] = CLevel_Test::Create();	
	m_arrLevels[LEVEL_BOOTCAMP] = CLevel_BootCamp::Create();	
	m_arrLevels[LEVEL_PADEN] = CLevel_Paden::Create();	

	return S_OK;
}

HRESULT CLoading_Manager::Reserve_Load_Level(LEVEL_TYPE_CLIENT eLevelID)
{
	if (eLevelID < 0 || eLevelID >= LEVEL_END)
	{
		Call_MsgBox_Index(L"Out of Range in Load_Level : CLoading_Manager", eLevelID);
		return E_FAIL;
	}

	m_eLoadID = eLevelID;
	m_bReservation = true;

	return S_OK;
}

HRESULT CLoading_Manager::Load_Level()
{
	if (!m_bReservation)
		return S_OK;

	m_bReservation = false;

	/* Delete all Objects except Statics */

	for (_uint i = 0; i < GR_END; ++i)
	{
		CGameInstance::Get_Instance()->Delete_Objects(i);
	}

	CGameInstance::Get_Instance()->Clear_All_Components();
	GAMEINSTANCE->Clear_Enable_Events();
	GAMEINSTANCE->Clear_Lights();
	//CGameInstance::Get_Instance()->Delete_GameObject_Prototypes();

	m_bFinish = false;
	static_cast<CLevel_Loading*>(m_arrLevels[LEVEL_LOADING])->Set_NextLevel(m_eLoadID);
	if (FAILED(GAMEINSTANCE->Exit_CurLevel()))
		return E_FAIL;
	CHANGE_LEVEL(m_arrLevels[LEVEL_LOADING]);



	InitializeCriticalSection(&m_CriticalSection);
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoading_Manager::Start_Loading()
{
	/* �ε��� ȭ���̶�, �ε��� ��ü �������� �س��� ������, */


	/* �̰��� ���� �а�, ��ü �����ϴ� �ڵ� �ۼ� */
	m_arrLevels[m_eLoadID]->SetUp_Prototypes();

	/* �� �����Ǹ� �ε��� ȭ���̶� �ε��� ��ü ������� */

	Finish_LoadingThread();
	return S_OK;
}

void CLoading_Manager::Open_NextLevel()
{
	CHANGE_LEVEL(m_arrLevels[m_eLoadID]);
}

void CLoading_Manager::Release()
{
	while (m_hThread == 0)
	{
		continue;
	}

	for (_uint i = 0; i < LEVEL_END; ++i)
	{
		SAFE_DESTROY(m_arrLevels[i]);
	}

	
}

void CLoading_Manager::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
	_float fResource = 1.f;

	fResource = m_arrLevels[m_eLoadID]->Get_LoadingFinish();

	pShader->Set_RawValue(pConstantName, &fResource, sizeof(_float));


	/*pShader->CallBack_SetRawValues +=
		bind(&CLevel_Loading::Set_ShaderResource, this, placeholders::_1, variablename);*/
}

void CLoading_Manager::Finish_LoadingThread()
{
	m_bFinish = true;

	WaitForSingleObject(m_hThread, INFINITE);
	DeleteCriticalSection(&m_CriticalSection);
	CloseHandle(m_hThread);
	m_hThread = 0;
}
