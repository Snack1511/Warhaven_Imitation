#include "stdafx.h"
#include "CUI.h"
#include "Transform.h"
#include "Renderer.h"
#include "CMesh_Rect.h" 
#include "CShader.h"
#include "Texture.h"

CUI::CUI()
{
}

CUI::CUI(const CUI& Prototype)
	: CGameObject(Prototype)
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

	CRenderer* pRenderer = CRenderer::Create(CP_RENDERER, RENDER_UI, VTXTEX_PASS_DEFAULT, _float4(0.f, 0.f, 0.f, 1.f));
	Add_Component<CRenderer>(pRenderer);

	return S_OK;
}

HRESULT CUI::Start()
{
	__super::Start();

	return S_OK;
}

void CUI::SetUp_ShaderResource(CShader* pShader, const char* pConstName)
{
}

void CUI::Set_Pos(_float fX, _float fY)
{
	m_vPosition.x = fX;
	m_vPosition.y = fY;
	Get_Transform()->Set_World(WORLD_POS, _float4(m_vPosition.x, m_vPosition.y, 0.f));
}

void CUI::Set_Scale(_float value)
{
	m_vOriginScale = value;
	m_vResultScale = m_vOriginScale * m_fScaleMulitple;
	m_pTransform->Set_Scale(m_vResultScale);
}

void CUI::Set_Scale(_float fX, _float fY)
{
	m_vOriginScale.x = fX;
	m_vOriginScale.y = fY;
	m_vResultScale = m_vOriginScale * m_fScaleMulitple;
	m_pTransform->Set_Scale(m_vResultScale);
}

void CUI::Set_ScaleMultiple(_float value)
{
	m_fScaleMulitple = value;
	m_vResultScale = m_vOriginScale * m_fScaleMulitple;
	m_pTransform->Set_Scale(m_vResultScale);
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

	int left = int(newPos.x - m_vResultScale.x * 0.5f);
	int top = int(newPos.y - m_vResultScale.y * 0.5f);
	int right = int(newPos.x + m_vResultScale.x * 0.5f);
	int bottom = int(newPos.y + m_vResultScale.y * 0.5f);
	SetRect(&m_tRect, left, top, right, bottom);

	GetCursorPos(&m_ptMouse);

	ScreenToClient(g_hWnd, &m_ptMouse);
}
