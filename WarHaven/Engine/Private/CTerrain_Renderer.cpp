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
	_int TextureNums = _int(m_pTextureList.size());
	ID3D11ShaderResourceView** pArrSRVs = new ID3D11ShaderResourceView* [TextureNums];
	ZeroMemory(pArrSRVs, sizeof(ID3D11ShaderResourceView*) * TextureNums);
	_int Index = 0;
	if (!m_pTextureList.empty())
	{
		for (auto& elem : m_pTextureList)
		{
			//for(auto& elemvec : elem->Get_vecTexture())
			pArrSRVs[Index++] = elem->Get_vecTexture()[0].pSRV.Get();
		}
	}

	m_pShaderCom->Set_ShaderResourceViewArray("g_DiffArray", pArrSRVs, TextureNums);
	m_pShaderCom->Set_RawValue("g_iNumTexture", &TextureNums, sizeof(_int));
	m_pShaderCom->CallBack_SetRawValues(m_pShaderCom, "");

	if (FAILED(m_pShaderCom->Begin(m_iCurPass)))
		return E_FAIL;

	if (FAILED(m_pMeshCom->Render()))
		return E_FAIL;

	ZeroMemory(pArrSRVs, sizeof(ID3D11ShaderResourceView*) * TextureNums);
	Safe_Delete_Array(pArrSRVs);
	
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
