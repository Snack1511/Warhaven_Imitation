#pragma once
#include "CTile.h"

BEGIN(Engine)
class ENGINE_DLL CTileLayer
{
	friend class CTile_Manager;

private:
	CTileLayer();
	~CTileLayer();

public:
	//F = G + H
	enum NodeDataFlag{Node_NodeTile, Node_ParentTile, Node_GCost, Node_HCost, Node_FCost};
	//NodeTile, ParentTile, GCost, HCost, FCost
	typedef tuple<CTile*, CTile*, _uint, _uint, _uint> NODE;
	typedef list<NODE> NODELIST;

public:
	static CTileLayer* Create(_uint iNumTilesX, _uint iNumTilesZ, _float fTileSize, _uint iLayerIndex);

public:
	_uint	Get_LayerIndex() { return m_iLayerIndex; }
	void	Set_StandardY(_float fStandardY) { m_fStandardY = fStandardY; }

public:
	HRESULT	Initialize();
	HRESULT SetUp_NeighborTile();

	HRESULT	Render();
	void	Release();

	//타일단위 최단거리 탐색용
	_bool Get_Route_TileBase(_uint iCurrent_TileIdx, _uint iTarget_TileIdx, list<CTile*>& OutBestList);
	_bool Get_Route_NearStair(_uint iCurrent_TileIdx, _uint& OutStairIndex, list<CTile*>& OutBestList);

private:
	_bool Bake_OpenCloseList(NODELIST& rhsOpenTileList, NODELIST& rhsCloseTileList, CTile* pEndTile);
	void Bake_BestList(list<CTile*>& OutBestList, NODELIST& pOpenTileList, NODELIST& pCloseTileList, CTile* pStart, CTile* pEnd);
	NODE Pop_CheapestNode(NODELIST& rhsOpenTileList);
	_bool Check_EndTile(NODELIST& rhsOpenList, NODELIST& rhsCloseList, NODE& SelectNode,  CTile* pEndTile);
	_bool Pop_FindNode(NODELIST& TileList, CTile* pSearchTile, NODE& OutTargetNode);
	_float Get_TileDistance(CTile* pSourTile, CTile* pDestTile);

private:
	vector<CTile*>	m_vecTiles;

	_uint		m_iNumTilesX = 0;
	_uint		m_iNumTilesZ = 0;

	/* 타일의 한 변의 길이 (타일은 무조건 정사각형) */
	_float		m_fTileSize = 0.f;

	_float		m_fStandardY = 0.f;

private:
	/* 이 레이어의 인덱스 ( 몇 층 인지 ) */
	_uint		m_iLayerIndex = 0;
};

END