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
	Safe_Delete_Array(m_pArrSRVs);
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
	if (nullptr == m_pArrSRVs || m_TextureNums < m_pTextureList.size())
	{
		m_TextureNums = _int(m_pTextureList.size());
		Safe_Delete_Array(m_pArrSRVs);
		m_pArrSRVs = nullptr;

		m_pArrSRVs = new ID3D11ShaderResourceView* [m_TextureNums];
		ZeroMemory(m_pArrSRVs, sizeof(ID3D11ShaderResourceView*) * m_TextureNums);
		_int Index = 0;
		for (auto& elem : m_pTextureList)
		{
			//for(auto& elemvec : elem->Get_vecTexture())
			m_pArrSRVs[Index++] = elem->Get_vecTexture()[0].pSRV.Get();
		}
	}
	if (nullptr != m_pArrSRVs)
	{
		m_pShaderCom->Set_ShaderResourceViewArray("g_DiffArray", m_pArrSRVs, m_TextureNums);
		m_pShaderCom->Set_RawValue("g_iNumTexture", &m_TextureNums, sizeof(_int));
		m_pShaderCom->CallBack_SetRawValues(m_pShaderCom, "");

		if (FAILED(m_pShaderCom->Begin(m_iCurPass)))
			return E_FAIL;

		if (FAILED(m_pMeshCom->Render()))
			return E_FAIL;
	}
	return S_OK;
}

void CTerrain_Renderer::Clear_TextureList()
{
	m_pTextureList.clear();
}

void CTerrain_Renderer::Add_Texture(CTexture* pTexture)
{
	m_pTextureList.push_back(pTexture);
}
