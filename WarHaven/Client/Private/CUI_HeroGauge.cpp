#include "CUI_HeroGauge.h"
#include "GameInstance.h"
#include "CShader.h"
#include "Renderer.h"
#include "CUI_Object.h"

CUI_HeroGauge::CUI_HeroGauge()
{
}

CUI_HeroGauge::~CUI_HeroGauge()
{
}

HRESULT CUI_HeroGauge::Initialize_Prototype()
{
	Read_UI("HeroGauge");

	m_Prototypes[BG] = m_pUIMap[TEXT("HeroGaugeBG")];
	m_Prototypes[Gauge] = m_pUIMap[TEXT("HeroGauge")];

	return S_OK;
}

HRESULT CUI_HeroGauge::Initialize()
{
	return S_OK;
}

HRESULT CUI_HeroGauge::Start()
{
	for (_uint i = 0; i < Type_End; ++i)
	{
		CREATE_GAMEOBJECT(m_Prototypes[i], GROUP_UI);
	}

	Set_Pass();
	Bind_Shader();

	__super::Start();

	return S_OK;
}

void CUI_HeroGauge::Set_ShaderResources(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fGaugeValue, sizeof(_float));
}

void CUI_HeroGauge::My_Tick()
{
	__super::My_Tick();

	// 게이지가 점점 증가하며 최대값이 됐을 경우 불 이펙트

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

void CUI_HeroGauge::Set_Pass()
{
	GET_COMPONENT_FROM(m_Prototypes[Gauge], CRenderer)->Set_Pass(VTXTEX_PASS_UI_HeroGauge);
}

void CUI_HeroGauge::Bind_Shader()
{
	GET_COMPONENT_FROM(m_Prototypes[Gauge], CShader)
		->CallBack_SetRawValues += bind(&CUI_HeroGauge::Set_ShaderResources, this, placeholders::_1, "g_fValue");
}