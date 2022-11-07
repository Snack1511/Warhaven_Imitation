#include "CUI_LoadingBG.h"
#include "Texture.h"

CUI_LoadingBG::CUI_LoadingBG()
{
}

CUI_LoadingBG::CUI_LoadingBG(const CUI_LoadingBG& Prototype)
	: CUI(Prototype)
{
}

CUI_LoadingBG::~CUI_LoadingBG()
{
}

HRESULT CUI_LoadingBG::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	SetTexture(TEXT("../Bin/Resources/Textures/UI/Loading/Map_Training_01.dds"));

	GET_COMPONENT(CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Loading/Map_Training_01.dds"));
	GET_COMPONENT(CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Loading/Map_Training_01.dds"));

	Set_Pos(0.f, 0.f);
	Set_Scale(1280.f, 720.f);

	m_iBGIndex = 1;

	return S_OK;
}

HRESULT CUI_LoadingBG::Initialize()
{
	return S_OK;
}

HRESULT CUI_LoadingBG::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_LoadingBG::OnEnable()
{
	__super::OnEnable();

	// 활성화 될 때 인덱스에 따라 불러올 배경이 바뀜
	GET_COMPONENT(CTexture)->Set_CurTextureIndex(m_iBGIndex);
}
