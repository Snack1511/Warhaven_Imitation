#include "CUI_Portrait.h"
#include "CUI_Object.h"
#include "Texture.h"
#include "CUnit.h"
#include "GameInstance.h"

CUI_Portrait::CUI_Portrait()
{
}

CUI_Portrait::CUI_Portrait(const CUI_Portrait& Prototype)
	: CUI_Wrapper(Prototype)
{
}

CUI_Portrait::~CUI_Portrait()
{
}

HRESULT CUI_Portrait::Initialize_Prototype()
{
	m_wstrName = TEXT("Port");

	__super::Initialize_Prototype();

	Read_Texture("/HUD/Portrait", "Class");

	GET_COMPONENT_FROM(m_pUI, CTexture)->Remove_Texture(0);

	return S_OK;
}

HRESULT CUI_Portrait::Initialize()
{
	return S_OK;
}

HRESULT CUI_Portrait::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_Portrait::Set_Portrait(_uint iIndex)
{
	switch (iIndex)
	{
	case CUnit::CLASS_WARRIOR:
		GET_COMPONENT_FROM(m_pUI, CTexture)->Set_CurTextureIndex(iIndex);
		break;
	case CUnit::CLASS_SPEAR:
		GET_COMPONENT_FROM(m_pUI, CTexture)->Set_CurTextureIndex(iIndex);
		break;
	case CUnit::CLASS_ARCHER:
		GET_COMPONENT_FROM(m_pUI, CTexture)->Set_CurTextureIndex(iIndex);
		break;
	case CUnit::CLASS_PALADIN:
		GET_COMPONENT_FROM(m_pUI, CTexture)->Set_CurTextureIndex(iIndex);
		break;
	case CUnit::CLASS_PRIEST:
		GET_COMPONENT_FROM(m_pUI, CTexture)->Set_CurTextureIndex(iIndex);
		break;
	case CUnit::CLASS_ENGINEER:
		GET_COMPONENT_FROM(m_pUI, CTexture)->Set_CurTextureIndex(iIndex);
		break;
	case CUnit::CLASS_FIONA:
		GET_COMPONENT_FROM(m_pUI, CTexture)->Set_CurTextureIndex(iIndex);
		break;
	case CUnit::CLASS_QANDA:
		GET_COMPONENT_FROM(m_pUI, CTexture)->Set_CurTextureIndex(iIndex);
		break;
	case CUnit::CLASS_HOEDT:
		GET_COMPONENT_FROM(m_pUI, CTexture)->Set_CurTextureIndex(iIndex);
		break;
	case CUnit::CLASS_LANCER:
		GET_COMPONENT_FROM(m_pUI, CTexture)->Set_CurTextureIndex(iIndex);
		break;
	}
}

void CUI_Portrait::My_Tick()
{
	__super::My_Tick();

	static int iIndex = 0;

	if (KEY(E, TAP))
	{
		iIndex++;
		if (iIndex >= 10)
			iIndex = 0;
	}

	if (KEY(Q, TAP))
	{
		iIndex--;
		if (iIndex < 0)
			iIndex = 9;
	}

	Set_Portrait(iIndex);
}

void CUI_Portrait::My_LateTick()
{
	__super::My_LateTick();
}
