#include "CUI_Logo.h"
#include "Texture.h"
#include "Renderer.h"
#include "CUI_Renderer.h"
#include "Loading_Manager.h"

CUI_Logo::CUI_Logo()
{
}

CUI_Logo::CUI_Logo(const CUI_Logo& Prototype)
	: CUI(Prototype)
{
}

CUI_Logo::~CUI_Logo()
{
}

#include "CFader.h"

HRESULT CUI_Logo::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	FADEDESC tFadeDesc;
	ZeroMemory(&tFadeDesc, sizeof(FADEDESC));
	// 페이드가 완료된 후에
	tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_NEXTTEXTURE;
	// 페이드가 어떻게 될지
	tFadeDesc.eFadeStyle = FADEDESC::FADE_STYLE_DEFAULT;
	// 페이드를 어떻게 시작할지
	tFadeDesc.bFadeInFlag = FADE_TIME;
	tFadeDesc.bFadeOutFlag = FADE_TIME;
	
	// 페이드가 시작되는 시간
	tFadeDesc.fFadeInStartTime = 0.f;
	tFadeDesc.fFadeInTime = 0.5f;
	
	// 페이드인이 끝나고 얼마 뒤에 아웃
	tFadeDesc.fFadeOutStartTime = 1.5f;
	tFadeDesc.fFadeOutTime = 0.5f;

	m_pFader->Get_FadeDesc() = tFadeDesc;	

	SetTexture(TEXT("../Bin/Resources/Textures/UI/Logo/Nexon.png"));

	GET_COMPONENT(CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Logo/Logo.png"));
	GET_COMPONENT(CUI_Renderer)->Set_Pass(VTXTEX_PASS_ALPHA);

	Set_Pos(0.f, 0.f);
	Set_Scale(1280.f, 720.f);

	return S_OK;
}

HRESULT CUI_Logo::Initialize()
{
	return S_OK;
}

HRESULT CUI_Logo::Start()
{
	__super::Start();

	GET_COMPONENT(CFader)->Set_FadeMessage();
	CallBack_FadeOutEvent += bind(&CUI_Logo::LoadScene, this, placeholders::_1);

	return S_OK;
}

void CUI_Logo::LoadScene(const _uint& event)
{
	CLoading_Manager::Get_Instance()->Reserve_Load_Level(LEVEL_MAINMENU);
}
