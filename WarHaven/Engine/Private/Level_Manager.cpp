#include "..\Public\Level_Manager.h"

#include "GameInstance.h"
#include "Level.h"

IMPLEMENT_SINGLETON(CLevel_Manager)


CLevel_Manager::CLevel_Manager()
{
}

CLevel_Manager::~CLevel_Manager()
{
	Release();
}

void CLevel_Manager::Tick()
{
	if (m_pCurrentLevel)
		m_pCurrentLevel->Tick();
}

void CLevel_Manager::Late_Tick()
{
	if (m_pCurrentLevel)
		m_pCurrentLevel->Late_Tick();
}

HRESULT CLevel_Manager::Render()
{
	if (FAILED(m_pCurrentLevel->Render()))
	{
		Call_MsgBox(L"Failed to Render : CLevel_Manager");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CLevel_Manager::Enter_Level(CLevel* pLevel)
{
	//delete m_pCurrentLevel;
	//SAFE_DELETE(m_pCurrentLevel);


	if (m_pCurrentLevel 
		&& FAILED(m_pCurrentLevel->Exit()))
		return E_FAIL;

	if (!(m_pCurrentLevel = pLevel))
		return E_FAIL;

	if (FAILED(m_pCurrentLevel->Enter()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Manager::Release()
{
	//SAFE_DELETE(m_pCurrentLevel);
}
