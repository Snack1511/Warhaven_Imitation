#include "CUI_LoadingIcon.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Renderer.h"

#include "CShader.h"

CUI_LoadingIcon::CUI_LoadingIcon()
{
}

CUI_LoadingIcon::CUI_LoadingIcon(const CUI_LoadingIcon& Prototype)
	: CUI(Prototype)
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
	SetTexture(TEXT("../Bin/Resources/Textures/UI/Loading/T_HPLoadingMask.dds"));

	GET_COMPONENT(CRenderer)->Set_Pass(VTXTEX_PASS_UI_LoadingIcon);

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

	return S_OK;
}

void CUI_LoadingIcon::My_Tick()
{
	__super::My_Tick();

	float fSpeed = fDT(0) * 0.5f;
	m_bIsPositive ? m_fCurValue += fSpeed : m_fCurValue -= fSpeed;

	if (m_fCurValue >= m_fMaxValue)
	{
		m_bIsPositive = false;
	}
	else if (m_fCurValue <= 0.f)
	{
		m_bIsPositive = true;
	}
}

void CUI_LoadingIcon::SetUp_ShaderResource(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fCurValue, sizeof(_float));
}
