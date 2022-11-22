#include "CUI_UnitHUD.h"
#include "CUI_Object.h"

#include "CUnit.h"
#include "CUI_UnitHP.h"

#include "GameInstance.h"
#include "CUtility_Transform.h"
#include "Transform.h"

HRESULT CUI_UnitHUD::Initialize_Prototype()
{
	Ready_UnitHP();

	return S_OK;
}

HRESULT CUI_UnitHUD::Initialize()
{
	return S_OK;
}

HRESULT CUI_UnitHUD::Start()
{
	for (int i = 0; i < UI_End; ++i)
	{
		CREATE_GAMEOBJECT(m_pUnitUI[i], GROUP_UI);
	}

	__super::Start();

	return S_OK;
}

void CUI_UnitHUD::OnEnable()
{
	__super::OnEnable();

	for (int i = 0; i < UI_End; ++i)
	{
		ENABLE_GAMEOBJECT(m_pUnitUI[i]);
	}
}

void CUI_UnitHUD::OnDisable()
{
	__super::OnDisable();

	for (int i = 0; i < UI_End; ++i)
	{
		DISABLE_GAMEOBJECT(m_pUnitUI[i]);
	}
}

void CUI_UnitHUD::My_Tick()
{
	__super::My_Tick();

	_float fHpGaugeRatio = m_tStatus.fHP / m_tStatus.fMaxHP;
	dynamic_cast<CUI_UnitHP*>(m_pUnitUI[UI_Hp])->Set_GaugeRatio(fHpGaugeRatio);
}

void CUI_UnitHUD::My_LateTick()
{
	__super::My_LateTick();
}

void CUI_UnitHUD::Set_ProjPos(CTransform* pTransform)
{
	_float m_fLengh = 2.f;
	_float4 vOffset = _float4(0.f, m_fLengh, 0.f);
	_float4 vNewPos = CUtility_Transform::Get_ProjPos(pTransform, vOffset);

	dynamic_cast<CUI_UnitHP*>(m_pUnitUI[UI_Hp])->Set_ProjPos(pTransform);
}

void CUI_UnitHUD::Ready_UnitHP()
{
	m_pUnitUI[UI_Hp] = CUI_UnitHP::Create();
}
