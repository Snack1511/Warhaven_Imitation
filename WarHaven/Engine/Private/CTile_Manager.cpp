#include "CTile_Manager.h"

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

HRESULT CTile_Manager::Create_Layers(_uint iNumPointsX, _uint iNumPointsZ, _float fTileSize, _uint iNumLayers)
{
	return S_OK;
}
