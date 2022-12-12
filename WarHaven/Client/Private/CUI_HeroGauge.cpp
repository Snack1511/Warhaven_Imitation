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
	Create_HeroGauge();
	Create_AbleHeroText();

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

void CUI_HeroGauge::Set_Shader_Gauge(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fHeroGaugeRatio", &m_fHeroGaugeRatio, sizeof(_float));
	pShader->Set_RawValue("g_fValue", &m_fValue, sizeof(_float));
}

void CUI_HeroGauge::Set_HeroGauge(_float fCurValue, _float fMaxValue)
{
	m_fCurHeroGauge = fCurValue;
	m_fMaxHeroGauge = fMaxValue;
}

void CUI_HeroGauge::SetActive_HeroGauge(_bool value)
{
	for (int i = 0; i < HG_End; ++i)
	{
		if (value == true)
		{
			ENABLE_GAMEOBJECT(m_pHeroGauge[i]);
		}
		else
		{
			DISABLE_GAMEOBJECT(m_pHeroGauge[i]);
		}
	}
}

void CUI_HeroGauge::SetActive_AbleHeroText(_bool value)
{
	Enable_Fade(m_pAbleHeroText, 0.15f);
}

void CUI_HeroGauge::Create_HeroGauge()
{
	for (int i = 0; i < HG_End; ++i)
	{
		m_pHeroGauge[i] = CUI_Object::Create();

		m_pHeroGauge[i]->Set_Pos(550.f, -280.f);
		m_pHeroGauge[i]->Set_Scale(75.f, 127.f);

		if (i == HG_BG)
		{
			m_pHeroGauge[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/HeroGauge/T_HeroGaugenew1.png"));

			m_pHeroGauge[i]->Set_Sort(0.5f);
		}
		else if (i == HG_Gauge)
		{
			m_pHeroGauge[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/HeroGauge/T_HeroGaugenew2.png"));
			m_pHeroGauge[i]->SetTexture(TEXT("../Bin/Resources/Textures/UI/Effect/T_Pattern_06.dds"));

			m_pHeroGauge[i]->Set_Sort(0.49f);
		}
		else if (i == HG_Text)
		{
			m_pHeroGauge[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Alpha0.png"));

			m_pHeroGauge[i]->Set_Pos(555.f, -195.f);
			m_pHeroGauge[i]->Set_Sort(0.5f);

			m_pHeroGauge[i]->Set_FontRender(true);
			m_pHeroGauge[i]->Set_FontStyle(true);
			m_pHeroGauge[i]->Set_FontCenter(true);
			m_pHeroGauge[i]->Set_FontScale(0.25f);
		}

		CREATE_GAMEOBJECT(m_pHeroGauge[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pHeroGauge[i]);
	}
}

void CUI_HeroGauge::Create_AbleHeroText()
{
	m_pAbleHeroText = CUI_Object::Create();

	m_pAbleHeroText->Set_FadeDesc(0.15f, 0.15f, 1.f, true);

	m_pAbleHeroText->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/HeroGauge/AbleHeroText.png"));

	m_pAbleHeroText->Set_PosY(-100.f);
	m_pAbleHeroText->Set_Scale(100.f, 38.f);
	m_pAbleHeroText->Set_Sort(0.5f);

	CREATE_GAMEOBJECT(m_pAbleHeroText, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pAbleHeroText);
}

void CUI_HeroGauge::My_Tick()
{
	__super::My_Tick();

	m_fValue += fDT(0) * 0.1f;

	m_fHeroGaugeRatio = m_fCurHeroGauge / m_fMaxHeroGauge;

	_tchar  szTemp[MAX_STR] = {};
	swprintf_s(szTemp, TEXT("%.f%%"), m_fHeroGaugeRatio * 100.f);
	m_pHeroGauge[HG_Text]->Set_FontText(szTemp);
	m_fHeroGaugeRatio = 1 - m_fHeroGaugeRatio;
}

void CUI_HeroGauge::OnEnable()
{
	__super::OnEnable();

	SetActive_HeroGauge(true);
}

void CUI_HeroGauge::OnDisable()
{
	__super::OnDisable();

	SetActive_HeroGauge(false);
}

void CUI_HeroGauge::Set_Pass()
{
	GET_COMPONENT_FROM(m_pHeroGauge[HG_Gauge], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_HeroGauge);
}

void CUI_HeroGauge::Bind_Shader()
{
	m_pHeroGauge[HG_Gauge]->Set_UIShaderFlag(SH_UI_HARDBLOOM);

	GET_COMPONENT_FROM(m_pHeroGauge[HG_Gauge], CShader)->CallBack_SetRawValues += bind(&CUI_HeroGauge::Set_Shader_Gauge, this, placeholders::_1, "g_fHeroGaugeRatio");
}