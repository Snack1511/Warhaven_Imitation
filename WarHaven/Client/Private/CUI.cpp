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
	// 이미지를 출력할 메쉬 렉트 생성
	Add_Component<CMesh>(CMesh_Rect::Create(0));

	// 셰이더 생성
	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXTEX,
		VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	// 렌더러 생성 및 패스 설정
	CRenderer* pRenderer = CRenderer::Create(CP_RENDERER, RENDER_UI, VTXTEX_PASS_DEFAULT
		, _float4(0.f, 0.f, 0.f, 1.f));
	Add_Component<CRenderer>(pRenderer);

	// 원본 객체가 생성될 때마다 UI 자료구조에 할당하여 IMGUI에서 불러옴

	return S_OK;
}

HRESULT CUI::Start()
{
	__super::Start();

	// 이미지 컴포넌트가 있을 경우에만 Texture를 출력
	// 이미지 컴포넌트는 멤버변수로 해당 이미지의 주소와 불러올 이미지 개수를 받아옴

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
