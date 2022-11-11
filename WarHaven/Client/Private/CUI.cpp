#include "stdafx.h"
#include "CUI.h"
#include "Transform.h"
#include "Renderer.h"
#include "CMesh_Rect.h" 
#include "CShader.h"
#include "Texture.h"
#include "CUtility_Transform.h"

CUI::CUI()
{
}

CUI::CUI(const CUI& Prototype)
	: CGameObject(Prototype)
	, m_vColor(Prototype.m_vColor)
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

	GET_COMPONENT(CShader)->CallBack_SetRawValues += bind(&CUI::SetUp_ShaderResource, this, placeholders::_1, "g_vColor");

	return S_OK;
}

void CUI::SetUp_ShaderResource(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_vColor", &m_vColor, sizeof(_float4));
}

void CUI::Set_Sort(_float value)
{
	m_vPosition.z = value;
	Get_Transform()->Set_World(WORLD_POS, _float4(m_vPosition.x, m_vPosition.y, m_vPosition.z));
}

void CUI::Set_Pos(_float fX, _float fY)
{
	m_vPosition.x = fX;
	m_vPosition.y = fY;
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
