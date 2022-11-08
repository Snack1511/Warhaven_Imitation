#include "stdafx.h"
#include "CLevel_Logo.h"

#include "GameInstance.h"

#include "CUI_Black.h"
#include "CUI_Logo.h"

CLevel_Logo::CLevel_Logo()
{
}

CLevel_Logo::~CLevel_Logo()
{
}

CLevel_Logo* CLevel_Logo::Create()
{
	CLevel_Logo* pLevel = new CLevel_Logo();

	pLevel->Initialize();

	return pLevel;
}

HRESULT CLevel_Logo::Initialize()
{
	return S_OK;
}

HRESULT CLevel_Logo::SetUp_Prototypes()
{
	return S_OK;
}

HRESULT CLevel_Logo::Enter()
{
	CUI_Black* pUI_Black = CUI_Black::Create();
	CUI_Logo* pUI_Logo = CUI_Logo::Create();

	Ready_GameObject(pUI_Black, GROUP_UI);
	Ready_GameObject(pUI_Logo, GROUP_UI);

	__super::Enter();

	return S_OK;
}

void CLevel_Logo::Tick()
{
}

void CLevel_Logo::Late_Tick()
{
}

HRESULT CLevel_Logo::Render()
{
	return S_OK;
}

HRESULT CLevel_Logo::Exit()
{
	__super::Exit();

	return S_OK;
}