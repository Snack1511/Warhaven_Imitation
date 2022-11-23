#include "CUI_UnitHUD.h"
#include "CUI_Object.h"

#include "CUnit.h"
#include "CUI_UnitHP.h"

#include "GameInstance.h"
#include "CUtility_Transform.h"
#include "Transform.h"
#include "Easing_Utillity.h"
#include "Texture.h"

HRESULT CUI_UnitHUD::Initialize_Prototype()
{
	Create_UnitHUD();

	return S_OK;
}

HRESULT CUI_UnitHUD::Initialize()
{
	return S_OK;
}

HRESULT CUI_UnitHUD::Start()
{
	Init_UnitNameText();

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

	ENABLE_GAMEOBJECT(m_pUnitNameText);
	for (int i = 0; i < UI_End; ++i)
	{
		ENABLE_GAMEOBJECT(m_pUnitUI[i]);
	}
}

void CUI_UnitHUD::OnDisable()
{
	__super::OnDisable();

	DISABLE_GAMEOBJECT(m_pUnitNameText);
	for (int i = 0; i < UI_End; ++i)
	{
		DISABLE_GAMEOBJECT(m_pUnitUI[i]);
	}
}

void CUI_UnitHUD::My_Tick()
{
	__super::My_Tick();

	/*m_fAccTime += fDT(0);
	if (1.f < m_fAccTime)
	{
		m_fAccTime = 1.f;
	}
	_float4 vCurPoint = CEasing_Utillity::CubicIn(vStartPoint, vTargetPoint, m_fAccTime, 1.f);
	m_tStatus.fHP = vCurPoint.x; */

	_float fHpGaugeRatio = m_tStatus.fHP / m_tStatus.fMaxHP;
	dynamic_cast<CUI_UnitHP*>(m_pUnitUI[UI_Hp])->Set_GaugeRatio(fHpGaugeRatio);
}

void CUI_UnitHUD::My_LateTick()
{
	__super::My_LateTick();
}

void CUI_UnitHUD::Set_ProjPos(CTransform* pTransform)
{	
	_float4 vOffset = _float4(-0.1f, 2.f, 0.f);
	_float4 vNewPos = CUtility_Transform::Get_ProjPos(pTransform, vOffset);

	m_pUnitNameText->Set_Pos(vNewPos);

	dynamic_cast<CUI_UnitHP*>(m_pUnitUI[UI_Hp])->Set_ProjPos(pTransform);
}

void CUI_UnitHUD::Create_UnitHUD()
{
	m_pUnitNameText = CUI_Object::Create();
	m_pUnitUI[UI_Hp] = CUI_UnitHP::Create();
}

void CUI_UnitHUD::Init_UnitNameText()
{
	GET_COMPONENT_FROM(m_pUnitNameText, CTexture)->Remove_Texture(0);

	m_pUnitNameText->Set_FontRender(true);
	m_pUnitNameText->Set_FontStyle(true);
	m_pUnitNameText->Set_FontScale(0.2f);
	m_pUnitNameText->Set_FontColor(_float4(1.f, 0.f, 0.f, 1.f));

	m_pUnitNameText->Set_FontText(TEXT("Àû±º"));

	CREATE_GAMEOBJECT(m_pUnitNameText, GROUP_UI);
}
