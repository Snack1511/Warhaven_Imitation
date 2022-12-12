#include "stdafx.h"
#include "CFadeDark.h"
#include "UsefulHeaders.h"

#include "CMesh_Rect.h"

#include "CFader.h"

CFadeDark::CFadeDark()
{
}

CFadeDark::~CFadeDark()
{
}

void CFadeDark::SetUp_ShaderResource(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_vColor", &ZERO_VECTOR, sizeof(_float4));
	pShader->Set_RawValue("g_vFlag", &ZERO_VECTOR, sizeof(_float4));

}

HRESULT CFadeDark::Initialize_Prototype()
{
	Add_Component<CMesh>(CMesh_Rect::Create(0));

	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXTEX, VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);


	CRenderer* pRenderer = CRenderer::Create(CP_RENDERER, RENDER_UI, VTXTEX_PASS_ALPHA, _float4(0.f, 0.f, 0.f, 1.f));
	Add_Component<CRenderer>(pRenderer);

	CTexture* pTexture = CTexture::Create(0, L"../bin/resources/textures/black.png", 1);
	Add_Component(pTexture);

	FADEDESC tFadeDesc;
	ZeroMemory(&tFadeDesc, sizeof(FADEDESC));
	// 페이드가 완료된 후에
	tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_DISABLE;
	// 페이드가 어떻게 될지
	tFadeDesc.eFadeStyle = FADEDESC::FADE_STYLE_DEFAULT;
	// 페이드를 어떻게 시작할지
	tFadeDesc.bFadeInFlag = FADE_TIME;
	tFadeDesc.bFadeOutFlag = FADE_TIME;

	// 페이드가 시작되는 시간
	tFadeDesc.fFadeInStartTime = 0.f;
	tFadeDesc.fFadeInTime = 0.f;

	// 페이드인이 끝나고 얼마 뒤에 아웃
	tFadeDesc.fFadeOutStartTime = 0.5f;
	tFadeDesc.fFadeOutTime = 0.3f;

	m_pFader = CFader::Create(CP_BEFORE_RENDERER, tFadeDesc);

	Add_Component(m_pFader);

	return S_OK;
}

HRESULT CFadeDark::Initialize()
{
    return S_OK;
}

HRESULT CFadeDark::Start()
{
	__super::Start();

	GET_COMPONENT(CShader)->CallBack_SetRawValues += bind(&CFadeDark::SetUp_ShaderResource, this, placeholders::_1, "g_vColor");


    return S_OK;
}

void CFadeDark::Start_Fade(_float fFadeInTime, _float fFadeOutStartTime, _float fFadeOutTime)
{
	ENABLE_GAMEOBJECT(this);
	m_pFader->Re_FadeIn();
	m_pFader->Get_FadeDesc().fFadeInTime = fFadeInTime;
	m_pFader->Get_FadeDesc().fFadeOutStartTime = fFadeOutStartTime;
	m_pFader->Get_FadeDesc().fFadeOutTime = fFadeOutTime;
}
