#pragma once
#include "Engine_Defines.h"

#define		MAX_ADJ_TILES	8

BEGIN(Engine)


class CTileLayer;


class ENGINE_DLL CTile final
{
	friend class CTileLayer;

public:
	enum eNeighborFlags 
	{
		eNeighborFlags_LeftTop,
		eNeighborFlags_Top,
		eNeighborFlags_RightTop,
		eNeighborFlags_Right,
		eNeighborFlags_RightBottom,
		eNeighborFlags_Bottom,
		eNeighborFlags_LeftBottom,
		eNeighborFlags_Left,
		eNeighborFlags_END,
	};

	enum eTileFlags
	{
		/* Ÿ���� �ƴ� �� ġ�� ���� */
		eTileFlags_None = 0,
		eTileFlags_Blocked = (1 << 0),

		/* ���� Ÿ���߿� �ٸ� ���̾��� Ÿ���� �ִ� ���. */
		eTileFlags_Default = (1 << 1),
		eTileFlags_Stair = (1 << 2),
	};

	const char* FlagOption_Display[4]
	{
		"NONE",
		"BLOCKED",
		"DEFAULT",
		"STAIR"
	};

	const char* Neighbor_Display[8]
	{
		"Left Top",
		"Top",
		"Right Top",
		"Right",
		"Right Bottom",
		"Bottom",
		"Left Bottom",
		"Left",

	};
private:
	CTile();
	~CTile();

public:
	static CTile* Create(_float4 vCenterPos, _uint iIndex, CTileLayer* pLayer);

public:
	_uint	Get_TileFlag() { return m_eTileFlag; }
	void	Set_TileFlag(_uint eFlag);
	void	Add_TileFlag(eTileFlags eFlag);
	void	Remove_TileFlag(eTileFlags eFlag);

	void	Bake_Neighbor(eNeighborFlags NeighborFlag, CTile* pNeighborTile);
	_uint	Get_GCost();
	_float4 Get_LocalCenterPos() { return m_vCenterPos; }

	_float4 Get_WorldCenterPos() { return m_vCenterWorldPos; }
	void Set_WorldCenterPos(_float4 vWorldCenterPos) { m_vCenterWorldPos = vWorldCenterPos; }

	CTile* Get_AdjTile(eNeighborFlags eDirection) { return m_arrAdjTiles[eDirection]; }
	_uint	Get_CurLayerIndex();

public:
	void	Switch_AllAdjTiles_To(CTile* pOtherTile);
	CTile** Find_ThisTileP(CTile* pTile);
	void	Switch_Tile(CTile* pFindTile, CTile* pNewTile);

public:
	_int Get_NeighborIndex(_uint NeighborDir);
	void Set_Neighbor(_uint NeighborDir, CTile* pTile);


public:
	/* ��ȿ�� Ÿ���� �� (�� �� �ִ� Ÿ������)*/
	_bool	Is_ValidTile();
	_bool	Check_TileFlag(eTileFlags eTileFlag);

private:
	/* ���� Ÿ�� */
	CTile* m_arrAdjTiles[MAX_ADJ_TILES] = { nullptr };

	/* Ÿ���� �߾� ������ */
	_float4			m_vCenterPos = ZERO_VECTOR;

	_float4			m_vCenterWorldPos = ZERO_VECTOR;
	_uint			m_iIndex = 0;
	
	/* Ÿ�� �÷��� */
	_uint		m_eTileFlag = eTileFlags_None;

private:
	/* �� ������ layer */
	CTileLayer* m_pOwnerLayer = nullptr;
	

};

END