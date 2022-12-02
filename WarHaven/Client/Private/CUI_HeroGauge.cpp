#include "CUI_HeroGauge.h"
#include "GameInstance.h"
#include "CShader.h"
#include "CUI_Renderer.h"
#include "CUI_Object.h"
#include "Renderer.h"

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

	m_Prototypes[Gauge]->SetTexture(TEXT("../Bin/Resources/Textures/UI/Effect/T_Pattern_06.dds"));
	m_Prototypes[Gauge]->SetTexture(TEXT("../Bin/Resources/Textures/UI/Effect/T_Pattern_13.dds"));

	for (_uint i = 0; i < Type_End; ++i)
	{
		CREATE_GAMEOBJECT(m_Prototypes[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_Prototypes[i]);
	}

	m_Prototypes[BG]->Set_Sort(0.3f);
	m_Prototypes[Gauge]->Set_Sort(0.29f);

	return S_OK;
}

HRESULT CUI_HeroGauge::Initialize()
{
	return S_OK;
}

HRESULT CUI_HeroGauge::Start()
{
	Set_Pass();
	Bind_Shader();

	__super::Start();

	return S_OK;
}

void CUI_HeroGauge::OnEnable()
{
	__super::OnEnable();

	for (int i = 0; i < Type_End; ++i)
	{
		ENABLE_GAMEOBJECT(m_Prototypes[i]);
	}
}

void CUI_HeroGauge::OnDisable()
{
	__super::OnDisable();

	for (int i = 0; i < Type_End; ++i)
	{
		DISABLE_GAMEOBJECT(m_Prototypes[i]);
	}
}

void CUI_HeroGauge::Start_HeroGauge()
{
	for (_uint i = 0; i < Type_End; ++i)
	{
		ENABLE_GAMEOBJECT(m_Prototypes[i]);
	}
}

void CUI_HeroGauge::Set_ShaderResources(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fHeroValue", &m_fGaugeRatio, sizeof(_float));
	pShader->Set_RawValue("g_fValue", &m_fUVSpeed, sizeof(_float));
}

void CUI_HeroGauge::My_Tick()
{
	__super::My_Tick();

	// 게이지가 점점 증가하며 최대값이 됐을 경우 불 이펙트

	// 게이지가 0이 되었을 경우 이펙트 종료

	// 게이지 최상단에 이미지 좌우 크기만큼 효과 하나

	// 유닛에서 히어로 게이지 최대와 현재를 받아와서 그 비율을 넘겨야함

	m_fUVSpeed += fDT(0) * 0.1f;

	//if (!m_bHeroAble)
	//{
	//	//m_fGaugeValue -= fSpeed;
	//	if (m_fGaugeValue <= 0.f)
	//	{
	//		m_fGaugeValue = 0.f;
	//		m_bHeroAble = true;
	//	}
	//}
	//else
	//{
	//	if (KEY(NUM1, TAP) || KEY(NUM2, TAP) || KEY(NUM3, TAP) || KEY(NUM4, TAP))
	//	{
	//		if (m_bIsHero)
	//		{
	//			m_bIsHero = false;
	//			m_bHeroAble = false;
	//		}
	//		else
	//		{
	//			m_bIsHero = true;
	//		}
	//	}
	//}

	//if (m_bIsHero)
	//{
	//	//m_fGaugeValue += fSpeed;
	//	if (m_fGaugeValue >= 1.f)
	//	{
	//		m_fGaugeValue = 1.f;
	//		m_bIsHero = false;
	//		m_bHeroAble = false;
	//	}
	//}
}

void CUI_HeroGauge::My_LateTick()
{
	__super::My_LateTick();
}

void CUI_HeroGauge::Set_Pass()
{
	GET_COMPONENT_FROM(m_Prototypes[Gauge], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_HeroGauge);
}

void CUI_HeroGauge::Bind_Shader()
{
	m_Prototypes[Gauge]->Set_UIShaderFlag(SH_UI_HARDBLOOM);

	GET_COMPONENT_FROM(m_Prototypes[Gauge], CShader)
		->CallBack_SetRawValues += bind(&CUI_HeroGauge::Set_ShaderResources, this, placeholders::_1, "g_fValue");
}