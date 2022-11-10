#include "CUI_Crosshair.h"
#include "Functor.h"
#include "GameInstance.h"
#include "CUI_Object.h"

CUI_Crosshair::CUI_Crosshair()
{
}

CUI_Crosshair::CUI_Crosshair(const CUI_Crosshair& Prototype)
{
}

CUI_Crosshair::~CUI_Crosshair()
{
}

HRESULT CUI_Crosshair::Initialize_Prototype()
{
	Read_UI("Crosshair");

	return S_OK;
}

HRESULT CUI_Crosshair::Initialize()
{
	return S_OK;
}

HRESULT CUI_Crosshair::Start()
{
	for (auto& pair : m_pUIMap)
	{
		CUI_Object* pUI = pair.second;
		CREATE_GAMEOBJECT(pUI, GROUP_UI);
	}

	__super::Start();

	return S_OK;
}

