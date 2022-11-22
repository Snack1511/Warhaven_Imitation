#include "CUI_UnitHP.h"
#include "CUI_Object.h"

#include "GameInstance.h"
#include "CUtility_Transform.h"
#include "CUI_Renderer.h"
#include "CShader.h"

HRESULT CUI_UnitHP::Initialize_Prototype()
{
	for (int i = 0; i < IT_END; ++i)
	{
		m_pUnitHP[i] = CUI_Object::Create();
		m_pUnitHP[i]->Set_Scale(80.f, 8.f);
	}

	m_pUnitHP[BG]->Set_Color(_float4(0.f, 0.f, 0.f));
	m_pUnitHP[BG]->Set_Sort(0.5f);
	m_pUnitHP[BG]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/UnitHUD/T_HPBarBG.png"));

	m_pUnitHP[BackGauge]->Set_Color(_float4(0.5f, 0.5f, 0.5f));
	m_pUnitHP[BackGauge]->Set_Sort(0.4f);
	m_pUnitHP[BackGauge]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/UnitHUD/T_HPBarGrey.dds"));

	m_pUnitHP[Gauge]->Set_Color(_float4(1.f, 0.f, 0.f));
	m_pUnitHP[Gauge]->Set_Sort(0.3f);
	m_pUnitHP[Gauge]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/UnitHUD/T_HPBarGrey.dds"));

	GET_COMPONENT_FROM(m_pUnitHP[Gauge], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_HealthGauge);
	GET_COMPONENT_FROM(m_pUnitHP[BackGauge], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_HealthGauge);

	return S_OK;
}

HRESULT CUI_UnitHP::Initialize()
{
	return S_OK;
}

HRESULT CUI_UnitHP::Start()
{
	__super::Start();

	for (int i = 0; i < IT_END; ++i)
	{
		CREATE_GAMEOBJECT(m_pUnitHP[i], GROUP_ENEMY);
	}

	Bind_Shader();

	m_bIsDecrease = true;

	return S_OK;
}

void CUI_UnitHP::My_Tick()
{
}

void CUI_UnitHP::My_LateTick()
{
}

void CUI_UnitHP::Set_Shader_HpGauge(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_bIsDecrease", &m_bIsDecrease, sizeof(_bool));
	pShader->Set_RawValue("g_fHealthGauge", &m_fGaugeRatio, sizeof(_float));
}

void CUI_UnitHP::Set_ProjPos(CTransform* pTransform)
{
	_float m_fLengh = 2.f;
	_float4 vOffset = _float4(0.f, m_fLengh, 0.f);
	_float4 vNewPos = CUtility_Transform::Get_ProjPos(pTransform, vOffset);

	_float fScaleZ = vNewPos.z;

	for (int i = 0; i < IT_END; ++i)
	{
		m_pUnitHP[i]->Set_Pos(vNewPos);
	}
}

void CUI_UnitHP::Bind_Shader()
{
	GET_COMPONENT_FROM(m_pUnitHP[Gauge], CShader)->CallBack_SetRawValues += bind(&CUI_UnitHP::Set_Shader_HpGauge, this, placeholders::_1, "g_fHealthGauge");
	GET_COMPONENT_FROM(m_pUnitHP[BackGauge], CShader)->CallBack_SetRawValues += bind(&CUI_UnitHP::Set_Shader_HpGauge, this, placeholders::_1, "g_fHealthGauge");
}
