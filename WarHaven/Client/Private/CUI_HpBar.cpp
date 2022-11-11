#include "CUI_HpBar.h"
#include "CShader.h"
#include "Renderer.h"
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
	Read_UI("Hp");

	m_Prototypes[BG] = m_pUIMap[TEXT("HpBarBG")];

	m_Prototypes[Bar] = m_pUIMap[TEXT("HpBar")];
	m_Prototypes[Bar]->SetTexture(TEXT("../Bin/Resources/Textures/UI/Effect/T_Pattern_16.dds"));
	m_Prototypes[Bar]->SetTexture(TEXT("../Bin/Resources/Textures/UI/Effect/T_Pattern_36.png"));

	return S_OK;
}

HRESULT CUI_HpBar::Initialize()
{
	return S_OK;
}

HRESULT CUI_HpBar::Start()
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

void CUI_HpBar::My_Tick()
{
	__super::My_Tick();

	m_fValue += fDT(0);
}

void CUI_HpBar::My_LateTick()
{
	__super::My_LateTick();
}

void CUI_HpBar::Set_ShaderResourcesBG(CShader* pShader, const char* pConstName)
{
	_float4 vColor = m_Prototypes[BG]->Get_Color();
	pShader->Set_RawValue("g_vColor", &vColor, sizeof(_float4));
}

void CUI_HpBar::Set_ShaderResourcesBar(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fValue, sizeof(_float));
}

void CUI_HpBar::Set_Pass()
{
	GET_COMPONENT_FROM(m_Prototypes[BG], CRenderer)->Set_Pass(VTXTEX_PASS_UI_Color);
	GET_COMPONENT_FROM(m_Prototypes[Bar], CRenderer)->Set_Pass(VTXTEX_PASS_UI_HpBar);
}

void CUI_HpBar::Bind_Shader()
{
	GET_COMPONENT_FROM(m_Prototypes[BG], CShader)
		->CallBack_SetRawValues += bind(&CUI_HpBar::Set_ShaderResourcesBG, this, placeholders::_1, "g_vColor");

	GET_COMPONENT_FROM(m_Prototypes[Bar], CShader)
		->CallBack_SetRawValues += bind(&CUI_HpBar::Set_ShaderResourcesBar, this, placeholders::_1, "g_fValue");
}
