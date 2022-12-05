#include "CTile_Manager.h"

IMPLEMENT_SINGLETON(CTile_Manager);

CTile_Manager::CTile_Manager()
{
}

CTile_Manager::~CTile_Manager()
{
	Release();
}

HRESULT CTile_Manager::Initialize()
{
	return S_OK;
}

HRESULT CTile_Manager::Render()
{
	for (auto& elem : m_vecLayers)
	{
		if (FAILED(elem->Render()))
			return E_FAIL;
	}

	return S_OK;
}

void CTile_Manager::Release()
{
	for (auto& elem : m_vecLayers)
		SAFE_DELETE(elem);


	m_vecLayers.clear();
}

_uint CTile_Manager::Find_Index(_float4 vPosition)
{
	if (m_vecLayers.empty())
		return 0;

	_int iIndexX = _int(vPosition.x / m_fTileSize);
	_int iIndexZ = _int(vPosition.z / m_fTileSize);

	_uint iCurIndex = iIndexX + (iIndexZ * m_iNumTilesX);

	if (iCurIndex > m_iTotalTileSize)
		return 0;

	return iCurIndex;
}

HRESULT CTile_Manager::Create_Layers(_uint iNumTilesX, _uint iNumTilesZ, _float fTileSize, _uint iNumLayers)
{
	if (!m_vecLayers.empty())
		return E_FAIL;

	m_fTileSize = fTileSize;
	m_iNumTilesX = iNumTilesX;
	m_iNumTilesZ = iNumTilesZ;
	m_iTotalTileSize = iNumTilesX * iNumTilesZ;

	for (_uint i = 0; i < iNumLayers; ++i)
	{
		CTileLayer* pTileLayer = CTileLayer::Create(iNumTilesX, iNumTilesZ, fTileSize, i);
		if (!pTileLayer)
			return E_FAIL;

		m_vecLayers.push_back(pTileLayer);
	}

	return S_OK;
}
