#include "CUI_MainPlay.h"
#include "Texture.h"
#include "GameInstance.h"
#include "CUI_Object.h"

#include "Loading_Manager.h"

CUI_MainPlay::CUI_MainPlay()
{
}

CUI_MainPlay::~CUI_MainPlay()
{
}

HRESULT CUI_MainPlay::Initialize_Prototype()
{
	m_wstrName = TEXT("MainPlay_Btn");

	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CUI_MainPlay::Initialize()
{
	return S_OK;
}

HRESULT CUI_MainPlay::Start()
{
	__super::Start();

	m_pUI->CallBack_PointUp += bind(&CUI_MainPlay::On_PointUpEvent, this, placeholders::_1);

	return S_OK;
}

void CUI_MainPlay::On_PointUpEvent(const _uint& iEventNum)
{
	CLoading_Manager::Get_Instance()->Reserve_Load_Level(LEVEL_TEST);
}
