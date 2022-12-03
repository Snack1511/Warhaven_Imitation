#pragma once
#include "CTile.h"

BEGIN(Engine)
class CTileLayer
{
	friend class CTile_Manager;

private:
	CTileLayer();
	~CTileLayer();

public:
	static CTileLayer* Create(_uint iNumTilesX, _uint iNumTilesZ, _float fTileSize, _uint iLayerIndex);

public:
	HRESULT	Initialize();
	HRESULT	Render();
	void	Release();


private:
	vector<CTile*>	m_vecTiles;

	_uint		m_iNumTilesX = 0;
	_uint		m_iNumTilesZ = 0;

	/* 타일의 한 변의 길이 (타일은 무조건 정사각형) */
	_float		m_fTileSize = 0.f;

private:
	/* 이 레이어의 인덱스 ( 몇 층 인지 ) */
	_uint		m_iLayerIndex = 0;
};

END