#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CMesh_Terrain;
END

BEGIN(Client)
class CTileDebugger
	: public CGameObject
{
	friend class CWindow_Tile;
	DECLARE_PROTOTYPE(CTileDebugger)

private:
	CTileDebugger();
	virtual ~CTileDebugger();

public:
	static CTileDebugger* Create(_uint iNumTilesX, _uint iNumTilesZ, _float fTileSize, _uint iLayerIndex);

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize() override;
	virtual HRESULT			Start() override;

public:
	void	Set_TileColor(_uint iTileIndex, _float4 vColor);

private:
	_uint		m_iMyLayerIndex = 0;
	_uint		m_iNumTilesX = 1;
	_uint		m_iNumTilesZ = 1;
	_float		m_fTileSize = 1.f;

	/* Ÿ�� ������ŭ ������, Ÿ�� �Ӽ��� ���� ��ó�� */
	CMesh_Terrain* m_pTerrainMesh = nullptr;

};

END