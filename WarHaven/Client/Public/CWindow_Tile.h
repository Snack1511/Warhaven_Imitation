#pragma once
#include "CImGui_Window.h"

/*
	레이어 이름으로 저장기능
	레이어 범위 설정 기능

*/

BEGIN(Engine)
class CGameObject;
class CTile;
class CMesh_Terrain;
class CCellLayer;
class CCell;
END

BEGIN(Client)

class CDebugObject;
class CTileDebugger;
class CWindow_Tile final
	: public CImGui_Window
{
private:
	CWindow_Tile();
	virtual ~CWindow_Tile();

public:
	enum eTupleData_SaveLoad {SL_FileName, SL_FilePath};

public:
	static CWindow_Tile* Create();

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual HRESULT Render() override;

public:/*세이브 로드 관련 함수*/
	void SetUp_DataFiles(const char* pPath);

private:
	void Control_SelectTile();
	void Control_SelectCell();

private:/*기타 기능 함수*/
	void		Create_SubWindow(const char* szWindowName, const ImVec2& Pos, const ImVec2& Size, function<void(CWindow_Tile&)> func);

private:
	_bool*				m_bRenderTile = nullptr;
	_bool				m_bHoverWindow = false;

private:
	_uint				m_iNumTilesX = 0;
	_uint				m_iNumTilesZ = 0;
	vector<CTileDebugger*> m_vecTileDebugger;
	_float				m_fTileSize = 0.f;
	_float				m_fTileHeightMinRange = 0.f;
	_float				m_fTileHeightMaxRange = 3.f;

private:	/*세이브 로드 관련 변수..*/
	//D:\PersonalData\MyProject\jusin128thFinalTeamPotpolio\WarHaven\Client\Bin\Data\MapData\TileData
	string m_DataDirectory = "../Bin/Data/MapData/TileData";
	string m_CellDataDirectory = "../Bin/Data/GameSystem/CellData";
	string m_TempCellDataDirectory = "../Bin/Data/MapData/CellData";
	vector<tuple<string, string>> m_FileDatas;
	_uint m_iFileDataIndex = 0;
	

private:
	CCell* m_pSelectCell = nullptr;
	CTile* m_pSelectTile = nullptr;
	_uint m_iCurSelectTileIndex = 0;
	_uint m_iCurSelectLayer = 0;
	_uint m_iCurSelectNeighbor = 0;
	_bool m_bSelectNeighbor = false;

	map<_float, CCellLayer*> m_pLayers;
	CCellLayer* m_pCurLayer = nullptr;
	_int m_iCellAttribute = 1;
	_uint m_iAttrubuteIndex = 0;
	_float m_fBrushSize = 1.f;
	static pair<const char*, _int> m_iAttributeArray[3];
private:
	void		On_Picking(_uint iLayerIndex, _float4 vPickedPos);
	void		On_Pick_Neighbor(_uint iLayerIndex, _float4 vPickedPos);
	void		On_CellPicking(_uint iLayerIndex, _float4 vPickedPos);
	void		On_CellSetAttribute(_uint iLayerIndex, _float4 vPickedPos, _float fRange);
	void		On_CellPick_Neighbor(_uint iLayerIndex, _float4 vPickedPos);
	
private:
	void		On_ShootRay();
	HRESULT		ShootRay_FirstFloor();
	HRESULT		ShootRay_Layer(_uint iLayerIndex);
	HRESULT		ReConnect_Tiles(_uint iLayerIndex);

	_bool		ShootRay_ToTerrain(_float4 vStartPos, _float4 vRayDir, _float4& vOutPos);
	_bool		ShootRay_ToBoxes(_float4 vStartPos, _float4 vRayDir, _float4& vOutPos, _float4& vOutNormal);

private:
	CMesh_Terrain* m_pMeshTerrain = nullptr;
	void		Find_Terrain();

	void		Save_All(string strKey);
	void		Save_AllLayer(string strKey);
	void		Load_All(string strKey);
	void		Load_AllLayer(string strKey);

};
END
