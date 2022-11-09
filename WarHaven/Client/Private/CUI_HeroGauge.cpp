#include "CUI_HeroGauge.h"
#include "GameInstance.h"
#include "CShader.h"
#include "Renderer.h"
#include "CUI_Object.h"

CUI_HeroGauge::CUI_HeroGauge()
{
}

CUI_HeroGauge::CUI_HeroGauge(const CUI_HeroGauge& Prototype)
	: CUI_Wrapper(Prototype)
{
}

CUI_HeroGauge::~CUI_HeroGauge()
{
}

HRESULT CUI_HeroGauge::Initialize_Prototype()
{
	m_wstrName = TEXT("HeroGauge");

	__super::Initialize_Prototype();

	GET_COMPONENT_FROM(m_pUI, CRenderer)->Set_Pass(VTXTEX_PASS_UI_HeroGauge);

	return S_OK;
}

HRESULT CUI_HeroGauge::Initialize()
{
	return S_OK;
}

HRESULT CUI_HeroGauge::Start()
{
	__super::Start();

	GET_COMPONENT_FROM(m_pUI, CShader)->CallBack_SetRawValues += bind(&CUI_HeroGauge::Set_ShaderResources, this, placeholders::_1, "g_fValue");

	return S_OK;
}

void CUI_HeroGauge::My_Tick()
{
	__super::My_Tick();


	// 게이지가 점점 증가하며 최대값이 됐을 경우 불 이펙트

	// 변신을 했을 경우 게이지가 점점 감소

	// 변신 중 해제하면 현재 게이지 유지??

	// 게이지가 0이 되었을 경우 이펙트 종료

	// 게이지 최상단에 이미지 좌우 크기만큼 효과 하나

	_float fSpeed = fDT(0) * 0.5f;

	if (!m_bHeroAble)
	{
		m_fGaugeValue -= fSpeed;
		if (m_fGaugeValue <= 0.f)
		{
			m_fGaugeValue = 0.f;
			m_bHeroAble = true;
		}
	}
	else
	{
		if (KEY(Q, TAP))
		{
			if (m_bIsHero)
			{
				m_bIsHero = false;
				m_bHeroAble = false;
			}
			else
			{
				m_bIsHero = true;
			}
		}
	}

	if (m_bIsHero)
	{
		m_fGaugeValue += fSpeed;
		if (m_fGaugeValue >= 1.f)
		{
			m_fGaugeValue = 1.f;
			m_bIsHero = false;
			m_bHeroAble = false;
		}
	}
}

void CUI_HeroGauge::My_LateTick()
{
	__super::My_LateTick();
}

void CUI_HeroGauge::Set_ShaderResources(CShader* pShader, const char* pConstName)
{
	GET_COMPONENT_FROM(m_pUI, CShader)->Set_RawValue("g_fValue", &m_fGaugeValue, sizeof(_float));
}
