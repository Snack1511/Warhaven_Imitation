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

	m_pUI->CallBack_ButtonClick += bind(&CUI_MainPlay::On_ButtonClickEvent, this, placeholders::_1);

	return S_OK;
}

void CUI_MainPlay::On_MouseInEvent(const _uint& iEventNum)
{
}

void CUI_MainPlay::On_MouseExitEvent(const _uint& iEventNum)
{
}

void CUI_MainPlay::On_ButtonClickEvent(const _uint& iEventNum)
{
	CLoading_Manager::Get_Instance()->Reserve_Load_Level(LEVEL_TEST);
}
