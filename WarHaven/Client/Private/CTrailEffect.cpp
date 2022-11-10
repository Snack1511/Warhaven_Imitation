#include "stdafx.h"
#include "CTrailEffect.h"

#include "GameInstance.h"

#include "CTrailBuffer.h"
#include "CShader.h"
#include "Renderer.h"
#include "Texture.h"
#include "CShader.h"

#include "HIerarchyNode.h"
#include "Transform.h"


CTrailEffect::CTrailEffect()
{
}

CTrailEffect::~CTrailEffect()
{
}

CTrailEffect* CTrailEffect::Create(_uint iGroupIdx, _uint iTriCnt, _float4 vLocalSwordLow, _float4 vLocalSwordHigh, CHierarchyNode* pSwordBone, CTransform* pUnitTransform,
	_float4 vGlowFlag, _float4 vColor, wstring wstrMaskMapPath, wstring wstrColorMapPath)
{
	CTrailEffect* pInstance = new CTrailEffect();

	pInstance->m_vColor = vColor;

	if (FAILED(pInstance->SetUp_TrailEffect(iGroupIdx, iTriCnt, vLocalSwordLow, vLocalSwordHigh, pSwordBone, pUnitTransform, vGlowFlag, wstrMaskMapPath, wstrColorMapPath)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CTrailEffect");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CTrailEffect");
		return nullptr;
	}

	return pInstance;
}

void CTrailEffect::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
	pShader->Set_RawValue(pConstantName, &m_vShaderFlag, sizeof(_float4));
	pShader->Set_RawValue("g_vGlowFlag", &m_vGlowFlag, sizeof(_float4));
	pShader->Set_RawValue("g_vColor", &m_vColor, sizeof(_float4));
}

void CTrailEffect::TurnOn_TrailEffect(_bool bTrunOn)
{
	if (!m_pTrailBuffer)
		return;

	if (bTrunOn)
		m_pTrailBuffer->Set_TrailOn();
	else
		m_pTrailBuffer->Set_TrailOff();

}

HRESULT CTrailEffect::Initialize_Prototype()
{
	Add_Component(CRenderer::Create(CP_RENDERER, RENDER_ALPHA, VTXTEX_PASS_TRAIL));

	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXTEX, VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);
    return S_OK;
}

HRESULT CTrailEffect::Initialize()
{

    return S_OK;
}

HRESULT CTrailEffect::Start()
{
	__super::Start();
	GET_COMPONENT(CShader)->CallBack_SetRawValues += bind(&CTrailEffect::Set_ShaderResource, this, placeholders::_1, "g_vFlag");

	return S_OK;
}

HRESULT CTrailEffect::SetUp_TrailEffect(_uint iGroupIdx, _uint iTriCnt, _float4 vLocalSwordLow, _float4 vLocalSwordHigh, CHierarchyNode* pSwordBone, CTransform* pUnitTransform, _float4 vGlowFlag, wstring wstrMaskMapPath, wstring wstrColorMapPath)
{
	m_vGlowFlag = vGlowFlag;
	m_pUnitTransform = pUnitTransform;
	m_pTrailBuffer = CTrailBuffer::Create(iGroupIdx, iTriCnt, vLocalSwordLow, vLocalSwordHigh, pSwordBone, pUnitTransform);

	Add_Component<CMesh>(m_pTrailBuffer);
	Add_Component(CTexture::Create(0, wstrMaskMapPath.c_str(), 1));
	Add_Component(CTexture::Create(0, wstrColorMapPath.c_str(), 1));


	return S_OK;
}

void CTrailEffect::My_LateTick()
{
	m_pTransform->Set_World(WORLD_POS, m_pUnitTransform->Get_World(WORLD_POS));
	m_pTransform->Make_WorldMatrix();

}