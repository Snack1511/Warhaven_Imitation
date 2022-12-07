#include "CTile.h"

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

void CTile::Bake_Neighvor(eNeighvorFlags NeighvorFlag, CTile* pNeighvorTile)
{
	m_arrAdjTiles[_uint(NeighvorFlag)] = pNeighvorTile;
}

_uint CTile::Get_GCost()
{
	//타일 타입에 따라 다르게 주기
	_uint iReturnGCost = 1;
	return iReturnGCost;
}

_bool CTile::Is_ValidTile()
{
	if (!CHECK_FLAG(eTileFlags_None) ||
		!CHECK_FLAG(eTileFlags_Blocked)
		)
		return true;

	return false;
}
