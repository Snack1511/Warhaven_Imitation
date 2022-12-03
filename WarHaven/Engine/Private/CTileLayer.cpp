#include "CTileLayer.h"

CTileLayer::CTileLayer()
{
}

CTileLayer::~CTileLayer()
{
	Release();
}

CTileLayer* CTileLayer::Create(_uint iNumTilesX, _uint iNumTilesZ, _float fTileSize, _uint iLayerIndex)
{
	CTileLayer* pInstance = new CTileLayer;

	if (iNumTilesX == 0 || iNumTilesZ == 0 || fTileSize <= 0.f)
	{
		Call_MsgBox(L" InValid Variables in : CTileLayer ");
		SAFE_DELETE(pInstance);
		return pInstance;
	}

	pInstance->m_iNumTilesX = iNumTilesX;
	pInstance->m_iNumTilesZ = iNumTilesZ;
	pInstance->m_fTileSize = fTileSize;
	pInstance->m_iLayerIndex = iLayerIndex;

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L" Failed to Initialize : CTileLayer ");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CTileLayer::Initialize()
{
	/* 타일 생성 */
	
	_uint	m_iTotalTileNum = m_iNumTilesX * m_iNumTilesZ;

	for (_uint i = 0; i < m_iTotalTileNum; ++i)
	{
		
		
	}

	return S_OK;
}

HRESULT CTileLayer::Render()
{
	return S_OK;
}

void CTileLayer::Release()
{
	for (auto& elem : m_vecTiles)
		SAFE_DELETE(elem);

	m_vecTiles.clear();
}
