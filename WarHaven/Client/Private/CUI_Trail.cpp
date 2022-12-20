#include "stdafx.h"
#include "CUI_Trail.h"

#include "GameInstance.h"

#include "CTrailBuffer.h"
#include "CShader.h"
#include "Renderer.h"
#include "Texture.h"
#include "CShader.h"

#include "HIerarchyNode.h"
#include "Transform.h"

#include "CCustomTrail.h"

CUI_Trail::CUI_Trail()
{
}

CUI_Trail::~CUI_Trail()
{
}

CUI_Trail* CUI_Trail::Create(_uint iGroupIdx, _uint iTriCnt, _float fWide, _float fUVSPeed, _float fUVPower, _float4 vGlowFlag, _float4 vColor, wstring wstrMaskMapPath, wstring wstrColorMapPath)
{
	CUI_Trail* pInstance = new CUI_Trail();

	if (FAILED(pInstance->SetUp_TrailEffect(iGroupIdx, iTriCnt, fWide, fUVSPeed, fUVPower, vGlowFlag, vColor, wstrMaskMapPath, wstrColorMapPath)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CUI_Trail");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CUI_Trail");
		return nullptr;
	}

	return pInstance;
}

void CUI_Trail::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
	pShader->Set_RawValue(pConstantName, &m_vShaderFlag, sizeof(_float4));
	pShader->Set_RawValue("g_vGlowFlag", &m_vGlowFlag, sizeof(_float4));
	pShader->Set_RawValue("g_vColor", &m_vColor, sizeof(_float4));
	pShader->Set_RawValue("g_fUVPlusX", &m_fCurUV, sizeof(_float4));
	pShader->Set_RawValue("g_fUVPower", &m_fUVPower, sizeof(_float4));
}


void CUI_Trail::Add_Node(const _float4& vNode)
{
	m_ListNodes.push_back(vNode);
}

void CUI_Trail::ReMap_TrailBuffers()
{
	m_pCustomTrailCom->ReMap_TrailBuffers(m_ListNodes, m_fWide);
}

HRESULT CUI_Trail::Initialize_Prototype()
{
	Add_Component(CRenderer::Create(CP_RENDERER, RENDER_ALPHA, VTXTEX_PASS_UITRAIL));

	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXTEX, VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);
	return S_OK;
}

HRESULT CUI_Trail::Initialize()
{

	return S_OK;
}

HRESULT CUI_Trail::Start()
{
	__super::Start();
	GET_COMPONENT(CShader)->CallBack_SetRawValues += bind(&CUI_Trail::Set_ShaderResource, this, placeholders::_1, "g_vFlag");

	return S_OK;
}

HRESULT CUI_Trail::SetUp_TrailEffect(_uint iGroupIdx, _uint iTriCnt, _float fWide, _float fUVSPeed, _float fUVPower, _float4 vGlowFlag, _float4 vColor, wstring wstrMaskMapPath, wstring wstrColorMapPath)
{
	m_fUVSpeed = fUVSPeed;
	m_fUVPower = fUVPower;
	m_fWide = fWide;
	m_vGlowFlag = vGlowFlag;
	m_vColor = vColor;

	CCustomTrail* pCustomTrail = CCustomTrail::Create(CP_BEFORE_RENDERER, iTriCnt);
	pCustomTrail->Initialize();
	m_pCustomTrailCom = pCustomTrail;
	Add_Component<CMesh>(m_pCustomTrailCom);

	Add_Component(CTexture::Create(0, wstrMaskMapPath.c_str(), 1));
	Add_Component(CTexture::Create(0, wstrColorMapPath.c_str(), 1));

	return S_OK;
}

void CUI_Trail::My_LateTick()
{
	m_fCurUV += m_fUVSpeed * fDT(0);

	if (m_fCurUV > 1.f)
		m_fCurUV = 0.f;
}