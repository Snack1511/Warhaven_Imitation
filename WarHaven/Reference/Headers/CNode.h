#pragma once
#include "Engine_Defines.h"
BEGIN(Engine)
class ENGINE_DLL CNode
{
public:
	typedef list<CNode*> CHILD;
public:
	CNode() = default;
	virtual ~CNode();
public:
	static CNode* Create(_float4 vPosition);
public:
	void Clear_Node();
public:
	_float4 Get_Positon() { return m_vPosition; }
public:
	_uint Get_TotalGCost() { return m_iTotalGCost; }
	_uint Get_CurGCost() { return m_iCurGCost; }
	_uint Get_HCost() { return m_iHCost; }
	_uint Get_FCost() { return m_iTotalGCost + m_iHCost; }
public:
	void Set_TotalGCost(_uint iTotalGCost) { m_iTotalGCost = iTotalGCost; }
	void Set_CurGCost(CNode* pEndNode);
	void Set_HCost(CNode* pEndNode);
	void Update_Heuristic(CNode* pEndNode);
	void Set_NodePosition(_float4 vPosition);
public:
	CHILD& Get_OriginChildList() { return m_pChildList; }
	CHILD& Get_SearchChildList() { return m_pSearchingChildList; }
	void Add_SearchChild(CNode* pNode);
	void Add_OriginChild(CNode* pNode);
	//중복된 자식노드 정리용
	void Ready_SearchingChild();
	void Unique_ChildList();
public:
	CNode* Get_Parent() { return m_pParentNode; }
	void Set_Parent(CNode* pNode) { m_pParentNode = pNode; }
private:
	CHILD m_pChildList;
	CHILD m_pSearchingChildList;

	CNode* m_pParentNode = nullptr;

	_float4 m_vPosition;
	//시작점 부터 현재 타일까지의 소요비용 --> 재검사시 변화
	_uint m_iTotalGCost = 0;
	//이전타일에서 현재 타일 까지의 소요비용 --> 매 검사마다 변화
	_uint m_iCurGCost = 0;
	//현재 타일에서 목적지까지의 예상 소요비용
	_uint m_iHCost = 0;

};
END