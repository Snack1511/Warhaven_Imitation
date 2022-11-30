#include "stdafx.h"
#include "CUI_Fire.h"
#include "UsefulHeaders.h"

CUI_Fire::CUI_Fire()
{
}

CUI_Fire::~CUI_Fire()
{
}

HRESULT CUI_Fire::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Fire::Initialize()
{

	return S_OK;
}

HRESULT CUI_Fire::Start()
{
	__super::Start();

	m_pFire = CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"FireTorch_0"), _float4(10.f, 10.f, 0.f, 1.f));

	GET_COMPONENT_FROM(m_pFire, CRenderer)->Set_RenderGroup(RENDER_UI);

	GET_COMPONENT_FROM(m_pFire, CRenderer)->Set_Pass(VTXRECTINSTANCE_PASS_UIFIRE);

	DISABLE_GAMEOBJECT(m_pFire);

	return S_OK;
}

void CUI_Fire::OnEnable()
{
	ENABLE_GAMEOBJECT(m_pFire);

	__super::OnEnable();
}

void CUI_Fire::OnDisable()
{
	DISABLE_GAMEOBJECT(m_pFire);

	__super::OnDisable();
}
