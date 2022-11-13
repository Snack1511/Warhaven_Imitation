#include "stdafx.h" 
#include "CBloodOverlay.h"
#include "GameInstance.h"
#include "Texture.h"
#include "CUI_Renderer.h"
#include "Renderer.h"

#include "CFader.h"
#include "CShader.h"
#include "CUser.h"
#include "CUnit.h"


CBloodOverlay::CBloodOverlay()
{
}

CBloodOverlay::CBloodOverlay(const CBloodOverlay& Prototype)
	: CUI(Prototype)
{
}

CBloodOverlay::~CBloodOverlay()
{
}

void CBloodOverlay::Trun_BloodOverlay(_float fHpRatio)
{
	if (0.5f <= fHpRatio)
		m_fImgIdx = 0;
	else if (0.5f > fHpRatio && 0.2f <= fHpRatio)
		m_fImgIdx = 1;
	else if (0.2f > fHpRatio)
		m_fImgIdx = 2;

	GET_COMPONENT(CFader)->Re_FadeIn();
}

HRESULT CBloodOverlay::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	// 텍스처를 추가하면 0 ~ 2번까지 각각 셰이더 텍스처에 할당됨
	SetTexture(TEXT("../Bin/Resources/Textures/Effects/WarHaven/Texture/T_BloodOverlay_%d.dds"),3);
		
	GET_COMPONENT(CUI_Renderer)->Set_Pass(VTXTEX_PASS_BLOODOVERLAY);

	Set_Pos(0.f, 0.f);
	Set_Scale(g_iWinCX, g_iWinCY);

	FADEDESC tFadeDesc;
	tFadeDesc.bFadeInFlag = FADE_TIME;
	tFadeDesc.bFadeOutFlag = FADE_TIME;
	tFadeDesc.fAlpha = 1.f;
	tFadeDesc.fFadeInStartTime = 0.f;
	tFadeDesc.fFadeInTime = 1.f;
	tFadeDesc.fFadeOutStartTime = 1.f;
	tFadeDesc.fFadeOutTime = 1.f;
	tFadeDesc.eFadeStyle = FADEDESC::FADE_STYLE_DEFAULT;
	tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_DISABLE;
	


	CFader* pFader = CFader::Create(CP_BEFORE_RENDERER, tFadeDesc);
	Add_Component<CFader>(pFader);

	return S_OK;
}

HRESULT CBloodOverlay::Initialize()
{
	return S_OK;
}

HRESULT CBloodOverlay::Start()
{
	__super::Start();


	return S_OK;
}

void CBloodOverlay::OnEnable()
{
	GET_COMPONENT(CTexture)->Set_CurTextureIndex(m_fImgIdx);
}

void CBloodOverlay::OnDisable()
{
}

void CBloodOverlay::My_Tick()
{
	__super::My_Tick();


	/*if (STATE_HIT == m_pUint->Get_CurState())
	{
		
	}*/
}

void CBloodOverlay::SetUp_ShaderResource(CShader* pShader, const char* pConstName)
{
	
}
