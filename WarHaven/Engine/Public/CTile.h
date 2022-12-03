#pragma once
#include "Engine_Defines.h"

#define		MAX_ADJ_TILES	8

BEGIN(Engine)


class CTileLayer;


class CTile final
{
	friend class CTileLayer;

public:
	enum eTileFlags
	{
		/* 타일이 아닌 셈 치는 거임 */
		eTileFlags_None = 0,
		eTileFlags_Blocked = (1 << 0),

		/* 인접 타일중에 다른 레이어의 타일이 있는 경우. */
		eTileFlags_Stair = (1 << 1),
	};

private:
	CTile();
	~CTile();

public:
	void	Set_TileFlag(_uint eFlag);
	void	Add_TileFlag(eTileFlags eFlag);
	void	Remove_TileFlag(eTileFlags eFlag);

public:
	/* 유효한 타일인 지 (갈 수 있는 타일인지)*/
	_bool	Is_VaildTile();

private:
	/* 인접 타일 */
	CTile* m_arrAdjTiles[MAX_ADJ_TILES] = {};

	/* 타일의 중앙 포지션 */
	_float4			m_vCenterPos;
	
	/* 타일 플래그 */
	_uint		m_eTileFlag = eTileFlags_None;


private:
	/* 날 소유한 layer */
	CTileLayer* m_pOwnerLayer = nullptr;
	

};

END