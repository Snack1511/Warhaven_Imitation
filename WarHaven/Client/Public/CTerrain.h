#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CMesh_Terrain;
class CCell;
class CRenderer;
class CTerrain_Renderer;
class CTexture;
END

BEGIN(Client)
class CTerrain
	: public CGameObject
{
	DECLARE_PROTOTYPE(CTerrain);
	
	friend class CWindow_Navi;

protected:
	CTerrain();
	virtual ~CTerrain();

public:
	static CTerrain* Create(_uint iNumVerticesX, _uint iNumVerticesZ);
	static CTerrain* Create(const _tchar* pFilePath);

#pragma region 내비관련
public:
	CCell* Get_Cell(_uint iIndex) { if (iIndex > m_vecCells.size()) return nullptr; return m_vecCells[iIndex]; }
	CCell* Get_StartCell() { return m_vecCells[m_iStartIndex]; }
	CCell* Get_CenterCell();
	CCell* Find_Cell(_float4 vPosition);
#pragma endregion

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

	virtual void My_Tick() override;
	virtual void My_LateTick() override;

public:
	void Change_ShaderPass(_int PassIndex = 0);
public:
	CMesh_Terrain* Get_MeshTerrain();
#pragma region 내비관련
protected:
	HRESULT Ready_NaviCells(ifstream& readFile, CMesh_Terrain* pTerrain);
	HRESULT SetUp_NeighborNaviCells();
#pragma endregion
protected:
	void Read_TerrainTexture(wstring wStrPath, list<wstring>& listPath);
protected:
	CMesh_Terrain* m_pTerrainMesh = nullptr;
	//CRenderer* m_pRenderer = nullptr;
	list<_int> m_listTerrainTexIndex;
	CTerrain_Renderer* m_pRenderer = nullptr;
#pragma region 내비관련
protected:
	vector<CCell*>	m_vecCells;
	_uint			m_iStartIndex = 0;

	_int			m_iNumCellsX = 0;
	_int			m_iNumCellsZ = 0;


#pragma endregion

private:
	HRESULT	SetUp_TerrainMesh(_uint iNumVerticesX, _uint iNumVerticesZ);
	HRESULT	SetUp_TerrainMesh(const _tchar* pFilePath);
	HRESULT SetUp_TerrainTextures();
};
END
