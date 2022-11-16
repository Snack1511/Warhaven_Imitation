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

	/*Terrain Shadow Pass*/
	pRenderer->m_iShadowPass = 3;

	return pRenderer;
}

HRESULT CTerrain_Renderer::Render()
{
	m_TextureNums = m_SRVList.size();
	if (nullptr == m_pArrSRVs)
	{
		Safe_Delete_Array(m_pArrSRVs);
		m_pArrSRVs = new ID3D11ShaderResourceView* [m_TextureNums];
		ZeroMemory(m_pArrSRVs, sizeof(ID3D11ShaderResourceView*) * m_TextureNums);
	}
	_int Index = 0;
	for (auto& elem : m_SRVList)
	{
		if (m_TextureNums < m_SRVList.size())
			assert(0);
		//for(auto& elemvec : elem->Get_vecTexture())
		m_pArrSRVs[Index++] = elem;
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
	Safe_Delete_Array(m_pArrSRVs);
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

void CTerrain_Renderer::Update_TextureIndex(_int Sour, _int Dest)
{
	_int Index = 0;
	ID3D11ShaderResourceView* pSourSRV = nullptr;
	ID3D11ShaderResourceView* pDestSRV = nullptr;

	for (auto& elem : m_pTextureList)
	{
		if (nullptr != pSourSRV && nullptr != pDestSRV)
		{
			break;
		}

		if (Sour == Index)
		{
			pSourSRV = elem->Get_vecTexture()[0].pSRV.Get();
		}
		if (Dest == Index)
		{
			pDestSRV = elem->Get_vecTexture()[0].pSRV.Get();
		}

		else
		{
			Index++;
		}
		//for(auto& elemvec : elem->Get_vecTexture())
	}
	while (m_SRVList.size() > 1)
	{
		m_SRVList.pop_back();
	}

	m_SRVList.push_back(pSourSRV);
	m_SRVList.push_back(pDestSRV);
	m_bSRVUpdateFlag = true;
}

void CTerrain_Renderer::Update_BackGround(_int BGIndex)
{
	_int Index = 0;
	ID3D11ShaderResourceView* pSRV = nullptr;
	if (m_SRVList.empty())
		m_SRVList.push_back(pSRV);
	for (auto& elem : m_pTextureList)
	{
		if (BGIndex == Index)
		{
			pSRV = elem->Get_vecTexture()[0].pSRV.Get();
			break;
		}

		else
		{
			Index++;
		}
		//for(auto& elemvec : elem->Get_vecTexture())
	}

	m_SRVList.front() = pSRV;
}

HRESULT CTerrain_Renderer::Bake_Shadow(_float4x4 ViewMatrix)
{
	m_pOwner->Get_Transform()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix");

	m_pShaderCom->Set_RawValue("g_ViewMatrix", &ViewMatrix, sizeof(_float4x4));

	if (FAILED(m_pShaderCom->Begin(m_iShadowPass)))
		return E_FAIL;

	if (FAILED(m_pMeshCom->Render()))
		return E_FAIL;

	return S_OK;
}
