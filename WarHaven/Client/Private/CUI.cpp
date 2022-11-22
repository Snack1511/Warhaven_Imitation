#include "stdafx.h"
#include "CUI.h"

#include "GameInstance.h"
#include "Transform.h"
#include "Renderer.h"
#include "CMesh_Rect.h" 
#include "CShader.h"
#include "Texture.h"
#include "CUtility_Transform.h"
#include "CFader.h"
#include "CUI_Renderer.h"
#include "Renderer.h"
#include "CButton.h"

CUI::CUI()
{
}

CUI::CUI(const CUI& Prototype)
	: CGameObject(Prototype)
	, m_vColor(Prototype.m_vColor)
	, m_vPosition(Prototype.m_vPosition)
	, m_vScale(Prototype.m_vScale)
	, m_pFader(Prototype.m_pFader)
	, m_vSliceRatio(Prototype.m_vSliceRatio)
	, m_vTextureSize(Prototype.m_vTextureSize)
{
}

CUI::~CUI()
{
}

HRESULT CUI::Initialize_Prototype()
{
	Add_Component<CMesh>(CMesh_Rect::Create(0));

	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXTEX, VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);
		
	CUI_Renderer* pRenderer = CUI_Renderer::Create(CP_RENDERER, RENDER_UI, VTXTEX_PASS_DEFAULT, _float4(0.f, 0.f, 0.f, 1.f));
	Add_Component<CUI_Renderer>(pRenderer);

	FADEDESC tFadeDesc;
	ZeroMemory(&tFadeDesc, sizeof(FADEDESC));
	// 페이드가 완료된 후에
	tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_NONE;
	// 페이드가 어떻게 될지
	tFadeDesc.eFadeStyle = FADEDESC::FADE_STYLE_DEFAULT;
	// 페이드를 어떻게 시작할지
	tFadeDesc.bFadeInFlag = FADE_TIME;
	tFadeDesc.bFadeOutFlag = FADE_NONE;

	// 페이드가 시작되는 시간
	tFadeDesc.fFadeInStartTime = 0.f;
	tFadeDesc.fFadeInTime = 0.f;

	// 페이드인이 끝나고 얼마 뒤에 아웃
	tFadeDesc.fFadeOutStartTime = 0.f;
	tFadeDesc.fFadeOutTime = 0.f;

	m_pFader = CFader::Create(CP_BEFORE_RENDERER, tFadeDesc);
	Add_Component(m_pFader);

	return S_OK;
}

HRESULT CUI::Start()
{
	// ,멤버 변수 무조건 스탙
	__super::Start();

	m_pFader = GET_COMPONENT(CFader);
	GET_COMPONENT(CUI_Renderer)->Set_UI(this);

	GET_COMPONENT(CShader)->CallBack_SetRawValues += bind(&CUI::SetUp_ShaderResource, this, placeholders::_1, "g_vColor");

	return S_OK;
}

void CUI::SetUp_ShaderResource(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_vColor", &m_vColor, sizeof(_float4));
		
	pShader->Set_RawValue("g_bIsSlice", &m_bIsSlice, sizeof(_bool));
	pShader->Set_RawValue("g_vScale", &m_vScale, sizeof(_float4));
	pShader->Set_RawValue("g_SliceRatio", &m_vSliceRatio, sizeof(_float4));
	pShader->Set_RawValue("g_TextureSize", &m_vTextureSize, sizeof(_float2));
}

void CUI::Active_Fade(FADEDESC tFadeDesc)
{
	m_pFader = CFader::Create(CP_BEFORE_RENDERER, tFadeDesc);
	Add_Component(m_pFader);
}

void CUI::Set_Sort(_float value)
{
	m_vPosition.z = value;
	Get_Transform()->Set_World(WORLD_POS, _float4(m_vPosition.x, m_vPosition.y, m_vPosition.z));
}

void CUI::Set_Pos(_float4 vPos)
{
	m_vPosition.x = vPos.x;
	m_vPosition.y = vPos.y;
	Get_Transform()->Set_World(WORLD_POS, _float4(m_vPosition.x, m_vPosition.y, m_vPosition.z));
}

void CUI::Set_Pos(_float fX, _float fY)
{
	m_vPosition.x = fX;
	m_vPosition.y = fY;
	Get_Transform()->Set_World(WORLD_POS, _float4(m_vPosition.x, m_vPosition.y, m_vPosition.z));
}

void CUI::Set_PosX(_float fX)
{
	m_vPosition.x = fX;
	Get_Transform()->Set_World(WORLD_POS, _float4(m_vPosition.x, m_vPosition.y, m_vPosition.z));
}

void CUI::Set_PosY(_float fY)
{
	m_vPosition.y= fY;
	Get_Transform()->Set_World(WORLD_POS, _float4(m_vPosition.x, m_vPosition.y, m_vPosition.z));
}

void CUI::Set_Scale(_float value)
{
	m_vScale = value;
	m_pTransform->Set_Scale(m_vScale);
}

void CUI::Set_Scale(_float fX, _float fY)
{
	m_vScale.x = fX;
	m_vScale.y = fY;
	m_pTransform->Set_Scale(m_vScale);
}

void CUI::Set_ScaleX(_float fX)
{
	m_vScale.x = fX;
	m_vScale.y = m_pTransform->Get_Scale().y;
	m_pTransform->Set_Scale(m_vScale);
}

void CUI::Set_ScaleY(_float fY)
{
	m_vScale.x = m_pTransform->Get_Scale().x;
	m_vScale.y = fY;
	m_pTransform->Set_Scale(m_vScale);
}

void CUI::Set_RotationZ(_float fAngle)
{
	_float4 vAxisZ = { 0.f,0.f,1.f };
	m_fRotValue = fAngle;

	CUtility_Transform::Rotation(m_pTransform, vAxisZ, m_fRotValue);
}

void CUI::OnEnable()
{
	__super::OnEnable();
}

void CUI::OnDisable()
{
	__super::OnDisable();
}

void CUI::My_Tick()
{
	__super::My_Tick();
}

void CUI::My_LateTick()
{
	__super::My_LateTick();
}

HRESULT CUI::SetTexture(const _tchar* pFilePath, _uint iIndex)
{
	CTexture* pTexture = CTexture::Create(0, pFilePath, iIndex);
	if (!pTexture)
	{
		Call_MsgBox(TEXT("Create failed Texture"));
		return E_FAIL;
	}
	Add_Component(pTexture);

	return S_OK;
}

void CUI::CheckInRect()
{
	_float4 newPos = XMVectorSet((m_vPosition.x + 640.f), -m_vPosition.y + 360.f, 0.f, 1.f);

	int left = int(newPos.x - m_vScale.x * 0.5f);
	int top = int(newPos.y - m_vScale.y * 0.5f);
	int right = int(newPos.x + m_vScale.x * 0.5f);
	int bottom = int(newPos.y + m_vScale.y * 0.5f);
	SetRect(&m_tRect, left, top, right, bottom);

	GetCursorPos(&m_ptMouse);

	ScreenToClient(g_hWnd, &m_ptMouse);
}
