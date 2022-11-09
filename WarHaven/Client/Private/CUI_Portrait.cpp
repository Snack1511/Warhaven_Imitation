#include "CUI_Portrait.h"
#include "CUI_Object.h"
#include "Texture.h"
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

	Get_Texture();

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
	GET_COMPONENT_FROM(m_pUI, CTexture)->Set_CurTextureIndex(iIndex);
}

void CUI_Portrait::My_Tick()
{
	__super::My_Tick();

	if (KEY(E, TAP))
	{
		_uint iSize = GET_COMPONENT_FROM(m_pUI, CTexture)->Get_vecTexture().size() - 1;
		GET_COMPONENT_FROM(m_pUI, CTexture)->Next_Texture();
		_uint iIndex = GET_COMPONENT_FROM(m_pUI, CTexture)->Get_CurTextureIndex();
		if (iIndex >= iSize)
			GET_COMPONENT_FROM(m_pUI, CTexture)->Set_CurTextureIndex(0);
	}
}

void CUI_Portrait::My_LateTick()
{
	__super::My_LateTick();
}

void CUI_Portrait::Get_Texture()
{
	GET_COMPONENT_FROM(m_pUI, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Portrait/Archer.dds"));
	GET_COMPONENT_FROM(m_pUI, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Portrait/Engineer.dds"));
	GET_COMPONENT_FROM(m_pUI, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Portrait/Fiona.dds"));
	GET_COMPONENT_FROM(m_pUI, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Portrait/Hoedt.dds"));
	GET_COMPONENT_FROM(m_pUI, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Portrait/Lancer.dds"));
	GET_COMPONENT_FROM(m_pUI, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Portrait/Paladin.dds"));
	GET_COMPONENT_FROM(m_pUI, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Portrait/Priest.dds"));
	GET_COMPONENT_FROM(m_pUI, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Portrait/Qanda.dds"));
	GET_COMPONENT_FROM(m_pUI, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Portrait/Spearman.dds"));
}
