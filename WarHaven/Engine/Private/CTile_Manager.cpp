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
	m_matWorld.Identity();
	m_matWorldInv.Identity();
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

void CTile_Manager::Set_WorldMatrix(_float4x4 matWorld)
{
	m_matWorld = matWorld;
	m_matWorldInv = matWorld.Inverse();
}

void CTile_Manager::Set_WorldPos(_float4 vPos)
{
	memcpy(&m_matWorld.m[3], &vPos, sizeof(_float4));
	m_matWorldInv = m_matWorld;
	m_matWorldInv.Inverse();
}

void CTile_Manager::Set_StandardY(_uint iLayerIndex, _float fStandardY)
{
	if (iLayerIndex >= m_vecLayers.size())
		return;

	m_vecLayers[iLayerIndex]->Set_StandardY(fStandardY);
}

_float CTile_Manager::Get_StandardY(_uint iLayerIndex)
{
	if (iLayerIndex >= m_vecLayers.size())
		return 0.f;

	return m_vecLayers[iLayerIndex]->m_fStandardY;
}

_uint CTile_Manager::Find_Index(_float4 vPosition)
{
	if (m_vecLayers.empty())
		return 0;

	vPosition = vPosition.MultiplyCoord(m_matWorldInv);

	_int iIndexX = _int(vPosition.x / m_fTileSize);
	_int iIndexZ = _int(vPosition.z / m_fTileSize);

	_uint iCurIndex = iIndexX + (iIndexZ * m_iNumTilesX);

	if (iCurIndex > m_iTotalTileSize)
		return 0;

	return iCurIndex;
}

CTile* CTile_Manager::Find_Tile(_uint iLayer, _uint iIndex)
{
	if (iLayer >= m_iNumLayers)
		return nullptr;

	if (iIndex >= m_iNumTilesX * m_iNumTilesZ)
		return nullptr;

	return m_vecLayers[iLayer]->m_vecTiles[iIndex];
}

HRESULT CTile_Manager::Create_Layers(_uint iNumTilesX, _uint iNumTilesZ, _float fTileSize, _uint iNumLayers)
{
	if (!m_vecLayers.empty())
		return E_FAIL;

	m_fTileSize = fTileSize;
	m_iNumTilesX = iNumTilesX;
	m_iNumTilesZ = iNumTilesZ;
	m_iTotalTileSize = iNumTilesX * iNumTilesZ;
	m_iNumLayers = iNumLayers;

	for (_uint i = 0; i < iNumLayers; ++i)
	{
		CTileLayer* pTileLayer = CTileLayer::Create(iNumTilesX, iNumTilesZ, fTileSize, i);
		if (!pTileLayer)
			return E_FAIL;

		m_vecLayers.push_back(pTileLayer);
	}

	return S_OK;
}

 _bool CTile_Manager::Get_Route(_int iCurLayer, _int StartIndex, _int iTargetLayer, _int TargetIndex, list<CTile*>& OutRoute)
{
	/*
	if(???? ??????)
		???? ???????? ?? ?? ???? ???? ?????? ?????? ????
		Get_Route(???? ??????, ????, iTargetLayer, TargetIndex)
		
*/
	 if (iCurLayer != iTargetLayer)
	 {
		//???? ????
		 _uint iStairTileIndex = 0;
		 m_vecLayers[iCurLayer]->Get_Route_NearStair(StartIndex, iStairTileIndex, OutRoute);

		 //?????? ???? ??????
		 _int iNextLayer =(iCurLayer > iTargetLayer) ? (iCurLayer - 1) : (iCurLayer + 1);
		 return Get_Route(iNextLayer, iStairTileIndex, iTargetLayer, TargetIndex, OutRoute);
	 }
	else
	{
		return m_vecLayers[iCurLayer]->Get_Route_TileBase(StartIndex, TargetIndex, OutRoute);
	}
}
