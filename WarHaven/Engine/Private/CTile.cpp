#include "CTile.h"
#include "CTileLayer.h"
#include "GameInstance.h"
#define CHECK_FLAG(flag)	(m_eTileFlag & flag)

CTile::CTile()
{
}

CTile::~CTile()
{
}

CTile* CTile::Create(_float4 vCenterPos, _uint iIndex, CTileLayer* pLayer)
{
	CTile* pTile = new CTile;

	pTile->m_vCenterPos = vCenterPos;
	pTile->m_iIndex = iIndex;
	pTile->m_pOwnerLayer = pLayer;

	return pTile;
}

void CTile::Set_TileFlag(_uint eFlag)
{
	m_eTileFlag = eFlag;
}

void CTile::Add_TileFlag(eTileFlags eFlag)
{
	m_eTileFlag |= eFlag;
}


void CTile::Remove_TileFlag(eTileFlags eFlag)
{
	m_eTileFlag &= ~eFlag;
}

void CTile::Bake_Neighbor(eNeighborFlags NeighborFlag, CTile* pNeighborTile)
{
	m_arrAdjTiles[_uint(NeighborFlag)] = pNeighborTile;
}

_uint CTile::Get_GCost()
{
	//타일 타입에 따라 다르게 주기
	_uint iReturnGCost = 1;
	return iReturnGCost;
}

_uint CTile::Get_CurLayerIndex()
{
	return m_pOwnerLayer->Get_LayerIndex();
}

void CTile::Switch_AllAdjTiles_To(CTile* pOtherTile)
{
	for (_uint i = 0; i < MAX_ADJ_TILES; ++i)
	{
		if (!m_arrAdjTiles[i])
			continue;

		//y검사해서 갈아끼우기
		_float4 vOtherTilePos = pOtherTile->Get_WorldCenterPos();
		_float4 vNearTilePos = m_arrAdjTiles[i]->Get_WorldCenterPos();
		_float fSubY = fabs(vOtherTilePos.y - vNearTilePos.y);

		if (fSubY < 0.2f)
		{
			m_arrAdjTiles[i]->Switch_Tile(this, pOtherTile);
		}

	}
}

CTile** CTile::Find_ThisTileP(CTile* pTile)
{
	for (_uint i = 0; i < MAX_ADJ_TILES; ++i)
	{
		if (!m_arrAdjTiles[i])
			continue;

		if (m_arrAdjTiles[i] == pTile)
			return &m_arrAdjTiles[i];
	}

	return nullptr;
}

void CTile::Switch_Tile(CTile* pFindTile, CTile* pNewTile)
{
	for (_uint i = 0; i < MAX_ADJ_TILES; ++i)
	{
		if (!m_arrAdjTiles[i])
			continue;

		if (m_arrAdjTiles[i] == pFindTile)
			m_arrAdjTiles[i] = pNewTile;
	}

}

_int CTile::Get_NeighborIndex(_uint NeighborDir)
{
	if (nullptr == m_arrAdjTiles[NeighborDir])
		return -1;
	else
		return m_arrAdjTiles[NeighborDir]->m_iIndex;
}

void CTile::Set_Neighbor(_uint NeighborDir, CTile* pTile)
{
	_uint MyDirFromNeighbor = (NeighborDir + 4) % 8;
	
	if (m_arrAdjTiles[NeighborDir] == pTile)
		return;

	if (nullptr != m_arrAdjTiles[NeighborDir])
		m_arrAdjTiles[NeighborDir]->m_arrAdjTiles[MyDirFromNeighbor] = nullptr;//자신 연결 끊기

	m_arrAdjTiles[NeighborDir] = pTile;//내 이웃 업데이트

	if (nullptr != pTile)
		pTile->Set_Neighbor(MyDirFromNeighbor, this);//새 이웃의 위치에 나 집어넣기
}

_bool CTile::Is_ValidTile()
{
	if (m_eTileFlag == eTileFlags_None ||
		CHECK_FLAG(eTileFlags_Blocked)
		)
		return false;

	return true;
}

_bool CTile::Check_TileFlag(eTileFlags eTileFlag)
{
	return (m_eTileFlag & eTileFlag);
}
