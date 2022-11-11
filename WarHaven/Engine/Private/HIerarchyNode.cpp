#include "..\Public\HIerarchyNode.h"
#include "HIerarchyNode.h"

#include "CResource_Bone.h"
#include "Model.h"

#include "GameObject.h"
#include "Transform.h"

CHierarchyNode::CHierarchyNode()
{
}

CHierarchyNode::~CHierarchyNode()
{
	Release();
}

CHierarchyNode* CHierarchyNode::Create(CResource_Bone* pResource, CHierarchyNode* pParent, _uint iDepth)
{
	CHierarchyNode* pInstance = new CHierarchyNode();

	if (FAILED(pInstance->Initialize(pResource, pParent, iDepth)))
	{
		Call_MsgBox(L"Failed to Created : CHierarchyNode");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CHierarchyNode::Add_NewChild(CHierarchyNode* pNode)
{
	if (!pNode->m_pParent)
		return E_FAIL;

	for (_uint i = 0; i < m_pChildrenNodes.size(); ++i)
	{
		if (!strcmp(m_pChildrenNodes[i]->Get_Name(), pNode->Get_Name()))
		{
			return E_FAIL;
		}
	}

	m_pChildrenNodes.push_back(pNode);


	return S_OK;
}

void CHierarchyNode::ReFind_Parent(CModel* pModel)
{
	if (m_pParent)
	{
		m_pParent = pModel->Find_HierarchyNode(m_pParent->Get_Name());
	}
}

void CHierarchyNode::Get_AllNodes(vector<CHierarchyNode*>& vecNodes)
{
	vecNodes.push_back(this);

	for (auto& elem : m_pChildrenNodes)
	{
		elem->Get_AllNodes(vecNodes);
	}
}


HRESULT CHierarchyNode::Initialize(CResource_Bone* pResource, CHierarchyNode* pParent, _uint iDepth)
{
	

	m_iDepth = iDepth;
	m_pParent = pParent;

	strcpy_s(m_szName, pResource->Get_Name().c_str());

	if (pResource->Get_Name() == "Root")
		m_bMoveNode = true;

	m_TransformationMatrix = pResource->Get_TransformationMatrix();

	XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_TransformationMatrix,
		XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_CurCombinedTransformationMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_PrevCombinedTransformationMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_SendCombinedTransformationMatrix, XMMatrixIdentity());
	

	vector<CResource_Bone*>& vecChildrenBones = pResource->Get_ChildrenBones();

	for (_uint i = 0; i < pResource->Get_NumChildren(); ++i)
	{
		CHierarchyNode* pNode = CHierarchyNode::Create(vecChildrenBones[i], this, m_iDepth + 1);

		if (!pNode)
			return E_FAIL;

		m_pChildrenNodes.push_back(pNode);
	}

	return S_OK;
}

void CHierarchyNode::Update_CombinedTransformationMatrix()
{
	if (m_bMoveNode) {
		//��Ʈ����� �� �ؾߴ�°�
		//1. ���� �Ĺ��ε��Ʈ���� �޾Ƽ� ������
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix)
			// �θ��� CombinedMatrix �� ������ �� Transform �̶� ���ؼ� ���� Combined �� ����.
			* XMLoadFloat4x4(&m_pParent->m_SendCombinedTransformationMatrix));

		//2. ���� �Ĺ��ε��Ʈ������ ���粨 ���ؼ� �̵��� ����

		//_matrix CurMat = XMLoadFloat4x4(&m_CombinedTransformationMatrix);
		//_matrix PreMat = XMLoadFloat4x4(&m_PrevCombinedTransformationMatrix);
		//_float4 vMove = CurMat.r[3] - PreMat.r[3];
		//_float fMoveLength = vMove.Length() * 0.01f;

		//if (fMoveLength < 0.5f)
		//{

		//	_float4 vPos = m_pOwner->Get_Transform()->Get_World(WORLD_POS);
		//	m_pOwner->Get_Transform()->Set_World(WORLD_POS, vPos + (m_pOwner->Get_Transform()->Get_World(WORLD_LOOK) * fMoveLength ));
		//}



		XMStoreFloat4x4(&m_SendCombinedTransformationMatrix, XMLoadFloat4x4(&m_CombinedTransformationMatrix));

		//3. ������ �Ĺ��ε��Ʈ���� ����
		for (int i = 0; i < 3; ++i)
		{
			m_SendCombinedTransformationMatrix.m[3][i] = 0.f;
		}

		m_SendCombinedTransformationMatrix.m[3][3] = 1.f;



		m_PrevCombinedTransformationMatrix = m_CombinedTransformationMatrix;
	}
	else
	{
		if (nullptr == m_pParent)
		{
			m_CombinedTransformationMatrix = m_TransformationMatrix;
			m_SendCombinedTransformationMatrix = m_CombinedTransformationMatrix;

		}

		else
		{
			
			//��� ��Ʈ��õ尡 �ƴ϶�ϱ�?
			if (m_pParent->m_bMoveNode)
			{
				int a = 0;
			

			}
				
			XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix)
				// �θ��� CombinedMatrix �� ������ �� Transform �̶� ���ؼ� ���� Combined �� ����.
				* XMLoadFloat4x4(&m_pParent->m_SendCombinedTransformationMatrix));

			m_SendCombinedTransformationMatrix = m_CombinedTransformationMatrix;


		}
	}

	

	//if (nullptr == m_pParent)
	//m_CombinedTransformationMatrix = m_TransformationMatrix;
	//	
	//else
	//XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix)
	//	// �θ��� CombinedMatrix �� ������ �� Transform �̶� ���ؼ� 
	//	* XMLoadFloat4x4(&m_pParent->m_CombinedTransformationMatrix));
}

void CHierarchyNode::Release()
{
	
}

