#pragma once
#include "Engine_Defines.h"
BEGIN(Engine)
class CCell;
class CNode;
#ifdef _DEBUG
class CShader;
#endif
class ENGINE_DLL CCellLayer
{
public:
	typedef list<CNode*> NODES;
	typedef vector<CCell*> CELLS;
	typedef list<CCell*> CellList;
public:
	CCellLayer();
	virtual ~CCellLayer();
public:
	static CCellLayer* Create(_uint XNums, _uint ZNums, _float fTileSize, _float4 vCenterPosition, _float fHeightMin, _float fHeightMax);
	static CCellLayer* Create(wstring strFilePath);
public:
	void Save(wstring strPath);
public:
	//셀 및 노드 설정
	HRESULT SetUp_Cells(_uint XNums, _uint ZNums, _float fTileSize, _float4 vCenterPosition);
	HRESULT SetUp_Cells(wstring strFilePath);

	//이웃 설정
	HRESULT SetUp_Neighbor();
	HRESULT SetUp_Neighbor(map<_float, CCellLayer*>& Layers);
	//노드 설정
	HRESULT SetUp_Nodes();
	//가시성 설정
	HRESULT SetUp_Visibility();
	void Set_DebugName(wstring strName) { m_strDebugName = strName; }
#ifdef _DEBUG
public:
	HRESULT SetUp_Shader();
	void DebugTick();
	void DebugRendering();
private:
	CShader* m_pDebugShader = nullptr;

#endif
public:
	_bool Check_BlockedVisibility(_float2 v2LinkStart, _float2 v2LinkEnd, _float2 v2CheckStart, _float2 v2CheckEnd);
	CellList Get_BestRoute(CNode* pStartNode, CNode* pEndNode);
	void Add_Link(CNode* pStart, CNode* pEnd);
	void Add_SearchLink(CNode* pStart, CNode* pEnd);
	HRESULT Ready_SearchingChild();
	void 	Set_Visiblity(CNode* pNode);
	CellList Get_StairCellList(_float fLayerKey);
private:
	_bool Bake_OpenCloseList(NODES& rhsOpenNodeList, NODES& rhsCloseNodeList, CNode* pEndNode);
	//목표노드인지 검사 후 아니면 현재노드의 자식노드를 OpenList에 업데이트
	_bool Check_EndNode(NODES& rhsOpenList, NODES& rhsCloseList, CNode* pSelectNode, CNode* pEndNode);
	void Bake_BestList(NODES& OutBestList, NODES& OpenNodeList, NODES& CloseNodeList, CNode* pEnd);
	CellList Make_CellList(NODES& OutBestList);
	void Find_NearCell(CNode* pStartNode, CNode* pEndNode, CellList& rhsCellList);
	void Clear_Nodes();
public:
	CCell* Find_Cell(_float4 vPosition);
	CCell* Get_Cell(_int Index);
	_uint Get_XTileNums() { return m_iXNums; }
	_uint Get_ZTileNums() { return m_iZNums; }
	_float Get_TileSize() { return m_fTileSize; }
	_float Get_MinHeight() { return m_fLayerHeightMin; }
	_float Get_MaxHeight() { return m_fLayerHeightMax; }
	void Set_MinHeight(_float Height);
private:
	CNode* Pop_CheapestNode(NODES& rhsOpenList);
	void Add_CellList(_uint iCellAttribute, CCell* pCell);
private:
	wstring m_strDebugName = L"";
	_float m_fLayerHeightMin = 0.f;
	_float m_fLayerHeightMax = 3.f;
	_float m_fTileSize = 1.f;
	_float4 m_vCenterPosition;
	_uint m_iXNums = 0;
	_uint m_iZNums = 0;

private:
	//전체 셀들
	CELLS m_Cells;
	NODES m_Nodes;

	//셀 속성별로 관리
	map<_uint, CellList> m_CellList;
	map<_float, CellList> m_StairList;
	list < _float4> m_BlockedIncludePosition;
private:
	_float* m_NeighborLayerKeys = nullptr;
	_int* m_NeighborIndex = nullptr;


};
END
