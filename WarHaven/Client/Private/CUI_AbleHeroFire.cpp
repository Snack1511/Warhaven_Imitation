#include "stdafx.h"
#include "CUI_AbleHeroFire.h"
#include "CUI_Renderer.h"
#include "UsefulHeaders.h"
#include "Easing_Utillity.h"
#include "CFader.h"

CUI_AbleHeroFire::CUI_AbleHeroFire()
{
}

CUI_AbleHeroFire::~CUI_AbleHeroFire()
{
}

HRESULT CUI_AbleHeroFire::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	SetTexture(TEXT("../Bin/Resources/Textures/Effects/WarHaven/Texture/T_Pattern_45.dds")); //diff
	SetTexture(TEXT("../Bin/Resources/Textures/Effects/WarHaven/Texture/T_Glow_08.dds")); //noise ->mask
	
	GET_COMPONENT(CUI_Renderer)->Set_Pass(VTXTEX_PASS_UVFIRE);
	m_fUVSpeedX = 0.f;
	m_fUVSpeedY = 2.f;
	m_vColor = RGB(0, 0, 0);
	m_vPlusColor = RGB(255, 80, 30);

	m_fScale = 200.f;
	m_fTargetScale = 75.f;

	Set_Pos(0.f, -100.f);
	Set_Scale(m_fScale, m_fScale);

	FADEDESC tFadeDesc;
	tFadeDesc.bFadeInFlag = FADE_TIME;
	tFadeDesc.bFadeOutFlag = FADE_TIME;
	tFadeDesc.fAlpha = 10.f;
	tFadeDesc.fFadeInStartTime = 0.f;
	tFadeDesc.fFadeInTime = 0.5f;
	tFadeDesc.fFadeOutStartTime = 0.6f;
	tFadeDesc.fFadeOutTime = 0.3f;
	tFadeDesc.eFadeStyle = FADEDESC::FADE_STYLE_DEFAULT;
	tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_DISABLE;

	GET_COMPONENT(CFader)->Get_FadeDesc() = tFadeDesc;

	return S_OK;
}

HRESULT CUI_AbleHeroFire::Initialize()
{

	return S_OK;
}

HRESULT CUI_AbleHeroFire::Start()
{
	__super::Start();
	Set_Sort(0.5f);

	m_vUIShaderFlag = SH_UI_HARDBLOOM;
	return S_OK;
}

void CUI_AbleHeroFire::OnEnable()
{
	GET_COMPONENT(CFader)->Re_FadeIn();

	m_fScale = 200.f;
	m_fTimeAcc = 0.f;

	__super::OnEnable();
}

void CUI_AbleHeroFire::OnDisable()
{
	__super::OnDisable();


}

void CUI_AbleHeroFire::My_Tick()
{
	__super::My_Tick();

	m_fCurUVPlusY += m_fUVSpeedY * fDT(0);
	m_fCurUVPlusX += m_fUVSpeedX * fDT(0);

	if (fabs(m_fCurUVPlusX) >= 1.f)
		m_fCurUVPlusX = 0.f;
	if (fabs(m_fCurUVPlusY) >= 1.f)
		m_fCurUVPlusY = 0.f;


	if (m_fTargetScale < m_fScale)
	{
		m_fTimeAcc += fDT(0);
		m_fScale = CEasing_Utillity::ElasticEaseOut(m_fScale, m_fTargetScale, m_fTimeAcc, 0.5f);
	}
	else
	{
		m_fScale = m_fTargetScale;	
	}

	Set_Scale(m_fScale, m_fScale);

}

void CUI_AbleHeroFire::SetUp_ShaderResource(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fUVPlusX", &m_fCurUVPlusX, sizeof(_float));
	pShader->Set_RawValue("g_fUVPlusY", &m_fCurUVPlusY, sizeof(_float));
	pShader->Set_RawValue("g_vColor", &m_vColor, sizeof(_float4));
	pShader->Set_RawValue("g_vPlusColor", &m_vPlusColor, sizeof(_float4));
}
