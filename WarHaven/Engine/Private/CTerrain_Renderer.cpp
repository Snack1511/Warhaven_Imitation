#include "CTerrain_Renderer.h"
#include "GameInstance.h"
#include "GameObject.h"

#include "Render_Manager.h"

#include "Transform.h"

#include "CShader.h"
#include "Texture.h"
#include "CMesh.h"

CTerrain_Renderer::CTerrain_Renderer(_uint iGroupID)
    :CRenderer(iGroupID)
{
}

CTerrain_Renderer::~CTerrain_Renderer()
{
}

CTerrain_Renderer* CTerrain_Renderer::Create(_uint iGroupID, const RENDER_GROUP& eRenderGroup, const _uint& iCurPass, const _float4& vOffsetPos)
{
	CTerrain_Renderer* pRenderer = new CTerrain_Renderer(iGroupID);

	pRenderer->m_iCurPass = iCurPass;
	pRenderer->m_vOffsetPos = vOffsetPos;
	pRenderer->m_eRenderGroup = eRenderGroup;

	if (FAILED(pRenderer->Initialize_Prototype()))
	{
		SAFE_DELETE(pRenderer);
		Call_MsgBox(L"Failed to Initialize_Prototype : CTerrain_Renderer");
		return nullptr;
	}

	return pRenderer;
}

HRESULT CTerrain_Renderer::Render()
{
	if (!m_pTextureList.empty())
	{
		_uint iIndex = 0;
		for (auto& elem : m_pTextureList)
		{
			string strConstantName;
			if (iIndex == 0)
				strConstantName = "g_DiffuseTexture";
			else if (iIndex == 1)
				strConstantName = "g_NoiseTexture";
			else if (iIndex == 2)
				strConstantName = "g_NormalTexture";

			if (FAILED(m_pShaderCom->SetUp_ShaderResourcesArray(elem, strConstantName.c_str())))
				return E_FAIL;

			iIndex++;
		}
	}

	m_pShaderCom->CallBack_SetRawValues(m_pShaderCom, "");

	if (FAILED(m_pShaderCom->Begin(m_iCurPass)))
		return E_FAIL;

	if (FAILED(m_pMeshCom->Render()))
		return E_FAIL;

	return S_OK;
}
