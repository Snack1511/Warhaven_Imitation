#include "CUI_MainPlay.h"
#include "Texture.h"
#include "GameInstance.h"
#include "CUI_Object.h"

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

	return S_OK;
}