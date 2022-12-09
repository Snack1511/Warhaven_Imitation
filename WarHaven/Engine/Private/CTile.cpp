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
	//Ÿ�� Ÿ�Կ� ���� �ٸ��� �ֱ�
	_uint iReturnGCost = 1;
	return iReturnGCost;
}

_int CTile::Get_NeighvorIndex(_uint NeighvorDir)
{
	if (nullptr == m_arrAdjTiles[NeighvorDir])
		return -1;
	else
		return m_arrAdjTiles[NeighvorDir]->m_iIndex;
}

void CTile::Set_Neighvor(_uint NeighvorDir, CTile* pTile)
{
	_uint MyDirFromNeighvor = (NeighvorDir + 4) % 8;
	
	if (m_arrAdjTiles[NeighvorDir] == pTile)
		return;

	if (nullptr != m_arrAdjTiles[NeighvorDir])
		m_arrAdjTiles[NeighvorDir]->m_arrAdjTiles[MyDirFromNeighvor] = nullptr;//�ڽ� ���� ����

	m_arrAdjTiles[NeighvorDir] = pTile;//�� �̿� ������Ʈ

	if (nullptr != pTile)
		pTile->Set_Neighvor(MyDirFromNeighvor, this);//�� �̿��� ��ġ�� �� ����ֱ�
}

_bool CTile::Is_ValidTile()
{
	if (m_eTileFlag == eTileFlags_None ||
		CHECK_FLAG(eTileFlags_Blocked)
		)
		return false;

	return true;
}
