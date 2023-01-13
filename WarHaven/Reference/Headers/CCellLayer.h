#pragma once
#include "Engine_Defines.h"
#define DEBUGRENDER
#define TmpRender
BEGIN(Engine)
class CCell;
class CNode;
#ifdef _DEBUG
class CShader;
#endif
class CShader;
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
	static CCellLayer* Create(_uint XNums, _uint ZNums, _float fTileSize, _float4 vCenterPosition, _float fHeightMin, _float fHeightMax, _int BaseCellAttribute = 4);
	static CCellLayer* Create(wstring strFolderPath, wstring strFolderName);
	CCellLayer* Clone_Layer(_float MinHeight);
public:
	void Save(wstring strForlderPath, wstring strPath);
	void Save_Info(wstring strInfoPath);
	void Save_Attribute(wstring strAttributePath);
	void Save_NeighborIndices(wstring strNeighborIndicesPath);
	void Save_NeighborKeys(wstring strNeighborKeysPath);
	void Save_Visiblity(wstring strNeighborKeysPath);

public:
	void Reset_Neighbor();
	//셀 및 노드 설정
	HRESULT SetUp_Cells(_uint XNums, _uint ZNums, _float fTileSize, _float4 vCenterPosition, _int iAttribute);
	HRESULT SetUp_Cells(wstring strFilePath);
	HRESULT Load_LayerInfo(wstring strFilePath);
	HRESULT Load_LayerAttribute(wstring strFilePath);
	HRESULT Load_LayerNeighborKeys(wstring strFilePath);
	HRESULT Load_LayerNeighborIndices(wstring strFilePath);
	HRESULT Load_Visibility(wstring strFilePath);
	HRESULT Load_Visibility();

	//이웃 설정
	HRESULT SetUp_Neighbor();
	HRESULT SetUp_Neighbor(map<_float, CCellLayer*>& Layers);
	//노드 설정
	HRESULT SetUp_Nodes();
	//가시성 설정
	HRESULT SetUp_Visibility();
	HRESULT SetUp_Visibility_UseThread();
	void Compare_Link(list<pair<CNode*, CNode*>>& LinkList, list<pair<CNode*, CNode*>>::iterator CmpIter);
	void Set_DebugName(wstring strName) { m_strDebugName = strName; }
	HRESULT Make_Cells(_byte* ArrAttribute);
	HRESULT SetUp_CellList();
	wstring Get_DebugName() { return m_strDebugName; }
#ifdef _DEBUG
#ifdef DEBUGRENDER
public:
	HRESULT SetUp_Vertex();
	HRESULT SetUp_Instancing();
	HRESULT SetUp_Index();
	HRESULT SetUp_Shader();

	HRESULT Create_VertexBuffer();
	HRESULT Create_IndexBuffer();

	void DebugTick();
	void DebugRendering();
	_float4 Get_Color(CCell* pCell);
private:
	CShader* m_pDebugShader = nullptr;
private:
	D3D11_BUFFER_DESC m_BufferDesc;
	D3D11_SUBRESOURCE_DATA m_SubResourceData;
private:
	ComPtr<ID3D11Buffer> m_pVB = nullptr;
	ComPtr<ID3D11Buffer> m_pIB = nullptr;
private:
	_uint						m_iStride = 0;
	_uint						m_iNumVertices = 0;
	_uint						m_iNumVertexBuffers = 0;
	ComPtr<ID3D11Buffer>		m_pVBInstance = nullptr;
	_uint						m_iInstanceStride = 0;
	_uint						m_iNumInstance = 0;
	_uint						m_iIndicesStride = 0;
	_uint						m_iNumPrimitive = 0;
	_uint						m_iNumIndices = 0;
	DXGI_FORMAT					m_eIndexFormat;
	D3D_PRIMITIVE_TOPOLOGY		m_eToplogy;
#endif
#else
#ifdef TmpRender
public:
	HRESULT SetUp_Vertex();
	HRESULT SetUp_Instancing();
	HRESULT SetUp_Index();
	HRESULT SetUp_Shader();

	HRESULT Create_VertexBuffer();
	HRESULT Create_IndexBuffer();

	void DebugTick();
	void DebugRendering();
	_float4 Get_Color(CCell* pCell);
private:
	CShader* m_pDebugShader = nullptr;
private:
	D3D11_BUFFER_DESC m_BufferDesc;
	D3D11_SUBRESOURCE_DATA m_SubResourceData;
private:
	ComPtr<ID3D11Buffer> m_pVB = nullptr;
	ComPtr<ID3D11Buffer> m_pIB = nullptr;
private:
	_uint						m_iStride = 0;
	_uint						m_iNumVertices = 0;
	_uint						m_iNumVertexBuffers = 0;
	ComPtr<ID3D11Buffer>		m_pVBInstance = nullptr;
	_uint						m_iInstanceStride = 0;
	_uint						m_iNumInstance = 0;
	_uint						m_iIndicesStride = 0;
	_uint						m_iNumPrimitive = 0;
	_uint						m_iNumIndices = 0;
	DXGI_FORMAT					m_eIndexFormat;
	D3D_PRIMITIVE_TOPOLOGY		m_eToplogy;
#endif
#endif
public:
	HRESULT SetUp_StairList(map<_float, CCellLayer*>& Layers);
	_bool Is_Access(_float LayerKey);
	list<_float> Get_LayerRoute(_float LayerKey);
	HRESULT SetUp_LayerRoute(map<_float, CCellLayer*>& Layers);
	_bool Check_MovableLayer(_float fTargetLayerKey, map<_float, CCellLayer*>&Layers, list<_float>& LayerRoute, list<_float> CircularCheckList);
	_bool Check_Circular(list<_float>& CircularCheckList);
private:
	//내 키 포함
	map<_float, list<_float>>m_LayerRoute;
public:
	_bool Check_BlockedVisibility(_float2 v2LinkStart, _float2 v2LinkEnd, _float2 v2CheckStart, _float2 v2CheckEnd);
	_bool Check_NearPoint(_float4 vLineStart, _float4 vLineEnd, _float4 vPosition);
	CellList Get_BestRoute(CNode* pStartNode, CNode* pEndNode, _bool& bOutIsFind, list<_float4>& OutNodeList);
	void Add_Link(CNode* pStart, CNode* pEnd);
	void Add_SearchLink(CNode* pStart, CNode* pEnd);
	HRESULT Ready_SearchingChild();
	void 	Set_Visiblity(CNode* pNode);
	CellList Get_StairCellList(_float fLayerKey);

	CNode* Get_Node(_float4 vPosition);
	CNode* Get_NearNode(_float4 vPosition);
public:
	void Replace_Attribute(_byte OriginAttribute, _byte ReplaceAttribute);
private:
	_bool Bake_OpenCloseList(NODES& rhsOpenNodeList, NODES& rhsCloseNodeList, CNode* pEndNode);
	//목표노드인지 검사 후 아니면 현재노드의 자식노드를 OpenList에 업데이트
	_bool Check_EndNode(NODES& rhsOpenList, NODES& rhsCloseList, CNode* pSelectNode, CNode* pEndNode);
	void Bake_BestList(NODES& OutBestList, NODES& OpenNodeList, NODES& CloseNodeList, CNode* pEnd);
	CellList Make_CellList(NODES& OutBestList);
	void Find_NearCell(CNode* pStartNode, CNode* pEndNode, CellList& rhsCellList);
	void Find_NearCell(CNode* pNode, CellList& rhsCellList);
	void Clear_Nodes();
public:
	CCell* Find_Cell(_float4 vPosition);
	list<CCell*> Find_Cell_InRange(_float4 vPosition, _float fRange);
	CCell* Get_Cell(_int Index);
	_uint Get_XTileNums() { return m_iXNums; }
	_uint Get_ZTileNums() { return m_iZNums; }
	_float Get_TileSize() { return m_fTileSize; }
	_float Get_MinHeight() { return m_fLayerHeightMin; }
	_float Get_MaxHeight() { return m_fLayerHeightMax; }
	void Set_MinHeight(_float Height);
	list<_float4>& Get_BlockedPositionList() { return m_BlockedIncludePosition; }
	list<pair<_float4, _float4>>Get_LinkList() { return m_LinkPosList; }
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
	list<pair<_float4, _float4>> m_LinkPosList;
	list<pair<CNode*, CNode*>> m_LinkList;
private:
	_float* m_NeighborLayerKeys = nullptr;
	_int* m_NeighborIndex = nullptr;
	wstring m_strFolderPath = L"";

public:
	void Func_Compare_UseThread(list<pair<CNode*, CNode*>>* pList);
private:
	vector<shared_ptr<thread>> m_ThreadArr;

};
END
