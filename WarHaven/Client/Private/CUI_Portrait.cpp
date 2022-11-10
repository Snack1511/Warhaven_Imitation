#include "CUI_Portrait.h"
#include "CUI_Object.h"
#include "Texture.h"

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

	m_eType = CLASS_FIONA;

	return S_OK;
}

HRESULT CUI_Portrait::Initialize()
{
	return S_OK;
}

HRESULT CUI_Portrait::Start()
{
	// 함수로 뺴ㅃ시다

	switch (m_eType)
	{
	case CLASS_WARRIOR:
		GET_COMPONENT_FROM(m_pUI, CTexture)->Set_CurTextureIndex(CLASS_WARRIOR);
		break;
	case CLASS_SPEAR:
		GET_COMPONENT_FROM(m_pUI, CTexture)->Set_CurTextureIndex(CLASS_SPEAR);
		break;
	case CLASS_ARCHER:
		GET_COMPONENT_FROM(m_pUI, CTexture)->Set_CurTextureIndex(CLASS_ARCHER);
		break;
	case CLASS_PALADIN:
		GET_COMPONENT_FROM(m_pUI, CTexture)->Set_CurTextureIndex(CLASS_PALADIN);
		break;
	case CLASS_PRIEST:
		GET_COMPONENT_FROM(m_pUI, CTexture)->Set_CurTextureIndex(CLASS_PRIEST);
		break;
	case CLASS_ENGINEER:
		GET_COMPONENT_FROM(m_pUI, CTexture)->Set_CurTextureIndex(CLASS_ENGINEER);
		break;
	case CLASS_FIONA:
		GET_COMPONENT_FROM(m_pUI, CTexture)->Set_CurTextureIndex(CLASS_FIONA);
		break;
	case CLASS_QANDA:
		GET_COMPONENT_FROM(m_pUI, CTexture)->Set_CurTextureIndex(CLASS_QANDA);
		break;
	case CLASS_HOEDT:
		GET_COMPONENT_FROM(m_pUI, CTexture)->Set_CurTextureIndex(CLASS_HOEDT);
		break;
	case CLASS_LANCER:
		GET_COMPONENT_FROM(m_pUI, CTexture)->Set_CurTextureIndex(CLASS_LANCER);
		break;
	}

	__super::Start();

	return S_OK;
}

void CUI_Portrait::My_Tick()
{
	__super::My_Tick();
}

void CUI_Portrait::My_LateTick()
{
	__super::My_LateTick();
}
