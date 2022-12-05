#include "CUI_HpBar.h"
#include "CShader.h"
#include "CUI_Renderer.h"
#include "CUI_Object.h"
#include "GameInstance.h"
#include "Texture.h"

CUI_HpBar::CUI_HpBar()
{
}

CUI_HpBar::~CUI_HpBar()
{
}

HRESULT CUI_HpBar::Initialize_Prototype()
{
	Create_HPUI();

	return S_OK;
}

HRESULT CUI_HpBar::Initialize()
{
	return S_OK;
}

HRESULT CUI_HpBar::Start()
{
	Set_Pass();
	Bind_Shader();

	__super::Start();

	return S_OK;
}

void CUI_HpBar::Set_Shader_Bar(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fHpRatio", &m_fHpRatio, sizeof(_float));
}

void CUI_HpBar::Set_HP(_float fCurValue, _float fMaxValue)
{
	m_fPrvHpValue = m_fCurHpValue;
	m_fCurHpValue = fCurValue;
	m_fMaxHpValue = fMaxValue;
}

void CUI_HpBar::SetActive_HP(_bool value)
{
	for (int i = 0; i < HP_End; ++i)
	{
		if (value == true)
		{
			ENABLE_GAMEOBJECT(m_pHPUI[i]);
		}
		else
		{
			DISABLE_GAMEOBJECT(m_pHPUI[i]);
		}
	}
}

void CUI_HpBar::OnEnable()
{
	__super::OnEnable();

	SetActive_HP(true);
}

void CUI_HpBar::OnDisable()
{
	__super::OnDisable();

	SetActive_HP(false);
}

void CUI_HpBar::My_Tick()
{
	__super::My_Tick();

	_float fLerpSpeed = fDT(0) * 10.f;
	m_fCurHpValue = ((1 - fLerpSpeed) * m_fPrvHpValue) + (fLerpSpeed * m_fCurHpValue);

	_tchar  szTemp[MAX_STR] = {};
	swprintf_s(szTemp, TEXT("%.f / %.f"), m_fCurHpValue, m_fMaxHpValue);
	m_pHPUI[HP_Text]->Set_FontText(szTemp);

	m_fHpRatio = m_fCurHpValue / m_fMaxHpValue;
}

void CUI_HpBar::Create_HPUI()
{
	for (int i = 0; i < HP_End; ++i)
	{
		m_pHPUI[i] = CUI_Object::Create();

		if (i == HP_BG)
		{
			m_pHPUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/HpBar/T_HPBarBG.png"));
			m_pHPUI[i]->Set_Color(_float4(0.f, 0.f, 0.f, 1.f));
			m_pHPUI[i]->Set_Pos(-370.f, -300.f);
			m_pHPUI[i]->Set_Scale(360.f, 15.f);
			m_pHPUI[i]->Set_Sort(0.5f);
		}
		else if (i == HP_Bar)
		{
			m_pHPUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/HpBar/T_HPBarGold.png"));
			m_pHPUI[i]->Set_Pos(-370.f, -299.5f);
			m_pHPUI[i]->Set_Scale(363.f, 15.f);
			m_pHPUI[i]->Set_Sort(0.49f);
		}
		else if (i == HP_Text)
		{
			m_pHPUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Alpha0.png"));
			m_pHPUI[i]->Set_Pos(-225.f, -285.f);
			m_pHPUI[i]->Set_Sort(0.5f);

			m_pHPUI[i]->Set_FontRender(true);
			m_pHPUI[i]->Set_FontStyle(true);
			m_pHPUI[i]->Set_FontCenter(true);
			m_pHPUI[i]->Set_FontScale(0.25f);
		}

		CREATE_GAMEOBJECT(m_pHPUI[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pHPUI[i]);
	}
}

void CUI_HpBar::Set_Pass()
{
	GET_COMPONENT_FROM(m_pHPUI[HP_Bar], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_HpBar);
}

void CUI_HpBar::Bind_Shader()
{
	GET_COMPONENT_FROM(m_pHPUI[HP_Bar], CShader)->CallBack_SetRawValues += bind(&CUI_HpBar::Set_Shader_Bar, this, placeholders::_1, "g_fHpRatio");
}
