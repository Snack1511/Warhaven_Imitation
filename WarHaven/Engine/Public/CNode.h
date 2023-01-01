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
	//�ߺ��� �ڽĳ�� ������
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
	//������ ���� ���� Ÿ�ϱ����� �ҿ��� --> ��˻�� ��ȭ
	_uint m_iTotalGCost = 0;
	//����Ÿ�Ͽ��� ���� Ÿ�� ������ �ҿ��� --> �� �˻縶�� ��ȭ
	_uint m_iCurGCost = 0;
	//���� Ÿ�Ͽ��� ������������ ���� �ҿ���
	_uint m_iHCost = 0;

};
END