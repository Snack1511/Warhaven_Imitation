#include "CNode.h"

CNode::~CNode()
{
    m_pChildList.clear();
	m_pSearchingChildList.clear();
}

CNode* CNode::Create(_float4 vPosition)
{
	CNode* pInstance = new CNode();
	pInstance->m_vPosition = vPosition;
	return pInstance;
}

void CNode::Clear_Node()
{
	m_iCurGCost = 0;
	m_iTotalGCost = 0;
	m_iHCost = 0;

	m_pParentNode = nullptr;

	//m_pSearchingChildList.clear();
}

void CNode::Set_CurGCost(CNode* pPrevNode)
{
	_float4 vPrevPosition = pPrevNode->Get_Positon();
	_float Length = (m_vPosition - vPrevPosition).Length();
	m_iCurGCost = _uint(Length * 1000);
}

void CNode::Set_HCost(CNode* pEndNode)
{
	_float4 vEndPosition = pEndNode->Get_Positon();
	_float Length = (m_vPosition - vEndPosition).Length();
	m_iHCost = _uint(Length * 1000);
}

void CNode::Update_Heuristic(CNode* pEndNode)
{
	CNode* pNearChild = nullptr;
	_uint ChildHCost = 9999999;

	for (auto& Child : m_pSearchingChildList)
	{
		if (Child == pEndNode)
		{
			pNearChild = Child;
			ChildHCost = 0.f;
			break;
		}

		Child->Update_Heuristic(pEndNode);
		_uint CmpChildHCost = Child->Get_HCost();
		if (CmpChildHCost < ChildHCost)
		{
			pNearChild = Child;
			ChildHCost = CmpChildHCost;
		}
	}

	m_iHCost = ChildHCost + (pNearChild->Get_Positon() - m_vPosition).Length();
}

void CNode::Set_NodePosition(_float4 vPosition)
{
	m_vPosition = vPosition;
}

void CNode::Add_SearchChild(CNode* pNode)
{
	if (pNode)
		m_pSearchingChildList.push_back(pNode);
}

void CNode::Add_OriginChild(CNode* pNode)
{
	if (pNode)
		m_pChildList.push_back(pNode);
}

void CNode::Ready_SearchingChild()
{
	m_pSearchingChildList.assign(m_pChildList.begin(), m_pChildList.end());
}

void CNode::Unique_ChildList()
{
	m_pChildList.unique();
}

void CNode::Remove_Child(CNode* pNode)
{
	m_pChildList.remove(pNode);
}

void CNode::Clear_ChildList()
{
	m_pChildList.clear();
}
