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

	/* Ÿ���� �� ���� ���� (Ÿ���� ������ ���簢��) */
	_float		m_fTileSize = 0.f;

private:
	/* �� ���̾��� �ε��� ( �� �� ���� ) */
	_uint		m_iLayerIndex = 0;
};

END