#include "..\Public\Texture.h"
#include "GameInstance.h"

#include "CShader.h"

#include "Renderer.h"
#include "CResource_Manager.h"

CTexture::CTexture(_uint iGroupIdx)
	: CComponent(iGroupIdx)
{
}

CTexture::~CTexture()
{
	Release();
}
CTexture* CTexture::Create(_uint iGroupIdx, const _tchar* pTextureFilePath, const _uint& iNumTextures)
{
	CTexture* pTexture = new CTexture(iGroupIdx);

	if (FAILED(pTexture->SetUp_Textures(pTextureFilePath, iNumTextures)))
	{
		SAFE_DELETE(pTexture);
		Call_MsgBox(L"Failed to Initialize_Prototype : CTexture");
		return nullptr;
	}

	if (FAILED(pTexture->Initialize_Prototype()))
	{
		SAFE_DELETE(pTexture);
		Call_MsgBox(L"Failed to Initialize_Prototype : CTexture");
		return nullptr;
	}

	return pTexture;
}

HRESULT CTexture::Set_ShaderResourceView(CShader* pShader, const char* pConstantName)
{
	if (m_iCurTextureIndex >= m_SRVs.size())
		return E_FAIL;

	return pShader->Set_ShaderResourceView(pConstantName, m_SRVs[m_iCurTextureIndex].pSRV);
}

void CTexture::Set_ShaderResource(CShader* pShader, const char* pConstantName, _uint iIdx)
{
	if (iIdx >= m_SRVs.size())
		return;

	pShader->Set_ShaderResourceView(pConstantName, m_SRVs[iIdx].pSRV);
}

HRESULT CTexture::Add_Texture(const _tchar* pTextureFilePath, _bool bFront)
{
	TEXTUREDESC	tDesc;

	tDesc.pSRV = CResource_Manager::Get_Instance()->Get_Texture(pTextureFilePath);
	tDesc.strFilePath = pTextureFilePath;

	if (bFront && !m_SRVs.empty())
	{
		auto iter = m_SRVs.begin();

		for (_uint i = 0; i < m_iCurTextureIndex; ++i)
			++iter;

		m_SRVs.insert(iter, tDesc);
	}
	else
		m_SRVs.push_back(tDesc);

	return S_OK;
}

void CTexture::Pop_Texture()
{
	if (m_SRVs.size() == 1)
		return;

	if (m_iCurTextureIndex == m_SRVs.size() - 1)
	{
		m_iCurTextureIndex--;
	}

	m_SRVs.pop_back();
}

void CTexture::Remove_Texture(_uint iIndex)
{
	if (m_SRVs.size() == 1)
		return;

	m_SRVs.erase(m_SRVs.begin() + iIndex);
}

_bool CTexture::Next_Texture()
{
	if (++m_iCurTextureIndex >= m_SRVs.size())
	{
		--m_iCurTextureIndex;
		return false;
	}

	return true;
}

void CTexture::Random_Texture()
{
	m_iCurTextureIndex = random(0, (_int)(m_SRVs.size() - 1));
}

void CTexture::Tick()
{
}

void CTexture::Late_Tick()
{
}

HRESULT CTexture::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTexture::Initialize()
{
	return S_OK;
}

void CTexture::Release()
{
	for (auto& elem : m_SRVs)
	{
		elem.pSRV.Reset();
	}
}

HRESULT CTexture::SetUp_Textures(const _tchar* pTextureFilePath, const _uint& iNumTextures)
{
	_tchar		szTextureFilePath[MAX_PATH] = TEXT("");

	m_SRVs.reserve(iNumTextures);

	for (_uint i = 0; i < iNumTextures; ++i)
	{
		TEXTUREDESC	tDesc;
		ComPtr<ID3D11ShaderResourceView> pSRV = nullptr;
		wsprintf(szTextureFilePath, pTextureFilePath, i);

		tDesc.pSRV = CResource_Manager::Get_Instance()->Get_Texture(szTextureFilePath);
		tDesc.strFilePath = szTextureFilePath;

		m_SRVs.push_back(tDesc);
	}

	return S_OK;
}

HRESULT CTexture::CreateTGATextureFromFile(const _tchar* pTextureFilePath, ID3D11ShaderResourceView** ppOutSRV)
{
	/*ScratchImage	tScImage;
	TexMetadata	tMetaData;
	if (FAILED(DirectX::LoadFromTGAFile(pTextureFilePath, &tMetaData, tScImage)))
		return E_FAIL;

	if (FAILED(CreateShaderResourceView(PDEVICE, tScImage.GetImages(), tScImage.GetImageCount(), tMetaData, ppOutSRV)))
		return E_FAIL;*/

	return S_OK;
}
