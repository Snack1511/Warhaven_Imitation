#include "CTile.h"

#define CHECK_FLAG(flag)	(m_eTileFlag & flag)

CTile::CTile()
{
}

CTile::~CTile()
{
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

_bool CTile::Is_VaildTile()
{
	if (!CHECK_FLAG(eTileFlags_None) ||
		!CHECK_FLAG(eTileFlags_Blocked)
		)
		return true;

	return false;
}
