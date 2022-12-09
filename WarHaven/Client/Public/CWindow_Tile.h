#pragma once
#include "CImGui_Window.h"

BEGIN(Engine)
class CGameObject;
class CTile;

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
private:/*기타 기능 함수*/
	void		Create_SubWindow(const char* szWindowName, const ImVec2& Pos, const ImVec2& Size, function<void(CWindow_Tile&)> func);
private:
	_bool				m_bRenderTile = true;
	_bool				m_bHoverWindow = false;
private:
	vector<CTileDebugger*> m_vecTileDebugger;

private:
	_float			m_fStepY = 0.f;

private:	/*세이브 로드 관련 변수..*/
	//D:\PersonalData\MyProject\jusin128thFinalTeamPotpolio\WarHaven\Client\Bin\Data\MapData\TileData
	string m_DataDirectory = "../Bin/Data/MapData/TileData";
	vector<tuple<string, string>> m_FileDatas;
	_uint m_iFileDataIndex = 0;

private:
	CTile* m_pSelectTile = nullptr;
	_uint m_iCurSelectTileIndex = 0;
	_uint m_iCurSelectLayer = 0;
	_uint m_iCurSelectNeighbor = 0;
	_bool m_bSelectNeighbor = false;


private:
	void		On_Picking(_uint iLayerIndex, _float4 vPickedPos);
	void		On_Pick_Neighbor(_uint iLayerIndex, _float4 vPickedPos);
};
END
