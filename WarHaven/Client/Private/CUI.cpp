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
	// �̹����� ����� �޽� ��Ʈ ����
	Add_Component<CMesh>(CMesh_Rect::Create(0));

	// ���̴� ����
	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXTEX,
		VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	// ������ ���� �� �н� ����
	CRenderer* pRenderer = CRenderer::Create(CP_RENDERER, RENDER_UI, VTXTEX_PASS_DEFAULT
		, _float4(0.f, 0.f, 0.f, 1.f));
	Add_Component<CRenderer>(pRenderer);

	// ���� ��ü�� ������ ������ UI �ڷᱸ���� �Ҵ��Ͽ� IMGUI���� �ҷ���

	return S_OK;
}

HRESULT CUI::Start()
{
	__super::Start();

	// �̹��� ������Ʈ�� ���� ��쿡�� Texture�� ���
	// �̹��� ������Ʈ�� ��������� �ش� �̹����� �ּҿ� �ҷ��� �̹��� ������ �޾ƿ�

	return S_OK;
}

void CUI::SetUp_ShaderResource(CShader* pShader, const char* pConstName)
{
}

void CUI::Set_Pos(_float fX, _float fY)
{
	Get_Transform()->Set_World(WORLD_POS, _float4(fX, fY, 0.f));
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
}

void CUI::My_LateTick()
{
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

void CUI::OnMouse()
{
}
