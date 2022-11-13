#pragma once
#include "CTerrain.h"
BEGIN(Client)
class CDrawable_Terrain :
    public CTerrain
{
public:
	typedef tuple<wstring, _uint, _uint, _float3*> Terrain_TUPLE;
protected:
	CDrawable_Terrain();
    virtual ~CDrawable_Terrain();
public:
	static CDrawable_Terrain* Create(_uint iNumVerticesX, _uint iNumVerticesZ);
	static CDrawable_Terrain* Create(const _tchar* pFilePath);
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

	virtual void My_Tick() override;
	virtual void My_LateTick() override;
public:
	_uint Get_TerrainVerticesCount();
	_uint& Get_TerrainVerticesX() {return m_iTerrainVertX;};
	_uint& Get_TerrainVerticesZ() {return m_iTerrainVertZ;};
	_float3* Get_TerrainVerticesPos();
	_float4* Get_TerrainTileFlag();
	Terrain_TUPLE Get_TerrainData();
	void Update_Vertices();
	void Update_Normal();
protected:
	virtual HRESULT	SetUp_TerrainMesh(_uint iNumVerticesX, _uint iNumVerticesZ);
	virtual HRESULT	SetUp_TerrainMesh(const _tchar* pFilePath);
	HRESULT SetUp_TerrainInfo(ifstream& readFile, CMesh_Terrain* pTerrain);

private:
	wstring m_strTileTexturePath = L"../bin/resources/Textures/Terrain/Tile0.dds";
	_uint m_iTerrainVertX = 0;
	_uint m_iTerrainVertZ = 0;
	_float3* m_pTerrainVertPos = nullptr;
};

END