#include "CUI_LoadingIcon.h"
#include "GameInstance.h"
#include "Texture.h"
#include "CUI_Renderer.h"
#include "Renderer.h"

#include "CShader.h"

CUI_LoadingIcon::CUI_LoadingIcon()
{
}


CUI_LoadingIcon::~CUI_LoadingIcon()
{
}

HRESULT CUI_LoadingIcon::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	// 텍스처를 추가하면 0 ~ 2번까지 각각 셰이더 텍스처에 할당됨
	SetTexture(TEXT("../Bin/Resources/Textures/UI/Loading/T_HPLoading.dds"));

	GET_COMPONENT(CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_LoadingIcon);

	Set_Pos(580.f, -320.f);
	Set_Scale(64.f);

	return S_OK;
}

HRESULT CUI_LoadingIcon::Initialize()
{
	return S_OK;
}

HRESULT CUI_LoadingIcon::Start()
{
	__super::Start();

	GET_COMPONENT(CShader)->CallBack_SetRawValues += bind(&CUI::SetUp_ShaderResource, this, placeholders::_1, "g_fValue");
	GET_COMPONENT(CShader)->CallBack_SetRawValues += bind(&CUI::SetUp_ShaderResource, this, placeholders::_1, "g_bAppear");

	m_fCurValue = 1.f;

	return S_OK;
}

void CUI_LoadingIcon::My_Tick()
{
	__super::My_Tick();

	float fSpeed = fDT(0) * 0.5f;
	m_fCurValue += fSpeed;
	if (m_fCurValue >= 1.f)
	{
		m_bIsAppear = !m_bIsAppear;
		m_fCurValue = 0.f;
	}
}

void CUI_LoadingIcon::SetUp_ShaderResource(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fCurValue, sizeof(_float));
	pShader->Set_RawValue("g_bAppear", &m_bIsAppear, sizeof(_bool));
}
