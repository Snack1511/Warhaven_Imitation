#include "CDestructible_Renderer.h"

#include "Model.h"
#include "GameObject.h"

#include "CShader.h"
#include "Transform.h"
#include "Render_Manager.h"

#include "MeshContainer.h"


CDestructible_Renderer::CDestructible_Renderer(_uint iGroupID)
	: CRenderer(iGroupID)
{
}

CDestructible_Renderer::~CDestructible_Renderer()
{
	Release();
}

CDestructible_Renderer* CDestructible_Renderer::Create(_uint iGroupID, const RENDER_GROUP& eRenderGroup, const _uint& iCurPass, const _float4& vOffsetPos)
{
	CDestructible_Renderer* pRenderer = new CDestructible_Renderer(iGroupID);

	pRenderer->m_iCurPass = iCurPass;
	pRenderer->m_vOffsetPos = vOffsetPos;
	pRenderer->m_eRenderGroup = eRenderGroup;

	if (FAILED(pRenderer->Initialize_Prototype()))
	{
		SAFE_DELETE(pRenderer);
		Call_MsgBox(L"Failed to Initialize_Prototype : CDestructible_Renderer");
		return nullptr;
	}

	return pRenderer;
}

HRESULT CDestructible_Renderer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDestructible_Renderer::Initialize()
{
	return S_OK;
}

void CDestructible_Renderer::Start()
{
	CComponent::Start();
	m_pShaderCom = GET_COMPONENT_FROM(m_pOwner, CShader);
	m_pModelCom = GET_COMPONENT_FROM(m_pOwner, CModel);

}

void CDestructible_Renderer::Tick()
{
}

void CDestructible_Renderer::Late_Tick()
{
	__super::Late_Tick();

	if (m_eShadowType == DEFAULTSHADOW)
		CRender_Manager::Get_Instance()->Add_Renderer(RENDER_SHADOW, this);

}

HRESULT CDestructible_Renderer::Render()
{
	m_pShaderCom->CallBack_SetRawValues(m_pShaderCom, "");

	_uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	//메쉬 컨테이너 별로 월드매트릭스를 따로 곱해주야함

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NoiseTexture", i, aiTextureType_DIFFUSE_ROUGHNESS)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_MaskTexture", i, aiTextureType_REFLECTION)))
			return E_FAIL;
		/*if (FAILED(m_pShaderCom->Begin(m_iCurPass)))
			return E_FAIL;*/

		if (FAILED(m_pModelCom->Render(i, m_pShaderCom, m_iCurPass, "g_Bones")))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CDestructible_Renderer::Bake_Shadow(_float4x4 ViewMatrix)
{
	m_pOwner->Get_Transform()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix");

	_uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	m_pShaderCom->Set_RawValue("g_ViewMatrix", &ViewMatrix, sizeof(_float4x4));

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		m_pModelCom->Bake_Shadow(i, m_pShaderCom, m_iShadowPass);
	}

	return S_OK;
}

void CDestructible_Renderer::Release()
{
}
