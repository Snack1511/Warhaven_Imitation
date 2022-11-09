#include "CUI_HpBar.h"
#include "CShader.h"
#include "Renderer.h"
#include "CUI_Object.h"
#include "GameInstance.h"

CUI_HpBar::CUI_HpBar()
{
}

CUI_HpBar::CUI_HpBar(const CUI_HpBar& Prototype)
    : CUI_Wrapper(Prototype)
{
}

CUI_HpBar::~CUI_HpBar()
{
}

HRESULT CUI_HpBar::Initialize_Prototype()
{
	m_wstrName = TEXT("HpBar");

	__super::Initialize_Prototype();

	m_pUI->SetTexture(TEXT("../Bin/Resources/Textures/UI/HUD/HpBar/T_Pattern_16.dds"));
	m_pUI->SetTexture(TEXT("../Bin/Resources/Textures/UI/HUD/HpBar/T_Pattern_36.png"));

	GET_COMPONENT_FROM(m_pUI, CRenderer)->Set_Pass(VTXTEX_PASS_UI_HpBar);

	return S_OK;
}

HRESULT CUI_HpBar::Initialize()
{
	return S_OK;
}

HRESULT CUI_HpBar::Start()
{
	GET_COMPONENT_FROM(m_pUI, CShader)->CallBack_SetRawValues += bind(&CUI_HpBar::Set_ShaderResources, this, placeholders::_1, "g_fValue");

	__super::Start();

	return S_OK;
}

void CUI_HpBar::My_Tick()
{
	__super::My_Tick();

	m_fValue += fDT(0) * 0.1f;
}

void CUI_HpBar::My_LateTick()
{
	__super::My_LateTick();
}

void CUI_HpBar::Set_ShaderResources(CShader* pShader, const char* pConstName)
{
	GET_COMPONENT_FROM(m_pUI, CShader)->Set_RawValue("g_fValue", &m_fValue, sizeof(_float));

}
