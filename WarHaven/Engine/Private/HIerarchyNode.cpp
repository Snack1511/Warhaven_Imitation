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
}

CHierarchyNode* CHierarchyNode::Create(CResource_Bone* pResource, CHierarchyNode* pParent, _uint iDepth, ANIM_DIVIDE eBoneType)
{
	CHierarchyNode* pInstance = new CHierarchyNode();

	if (FAILED(pInstance->Initialize(pResource, pParent, iDepth, eBoneType)))
	{
		Call_MsgBox(L"Failed to Created : CHierarchyNode");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CHierarchyNode::Set_Position(_float4 vPos)
{
	m_TransformationMatrix.m[3][0] = vPos.x;
	m_TransformationMatrix.m[3][1] = vPos.y;
	m_TransformationMatrix.m[3][2] = vPos.z;
}

void CHierarchyNode::Set_Translation_NoPos(_float4x4 TransformationMatrix)
{

	*(_float4*)(m_TransformationMatrix.m[0]) = *(_float4*)(TransformationMatrix.m[0]);
	*(_float4*)(m_TransformationMatrix.m[1]) = *(_float4*)(TransformationMatrix.m[1]);
	*(_float4*)(m_TransformationMatrix.m[2]) = *(_float4*)(TransformationMatrix.m[2]);
}

_float4x4 CHierarchyNode::Get_BoneMatrix()
{
	_float4x4 BoneMatrix;

	BoneMatrix = m_CombinedTransformationMatrix;

	BoneMatrix *= GET_COMPONENT_FROM(m_pOwner, CModel)->Get_TransformationMatrix();
	BoneMatrix *= m_pOwner->Get_Transform()->Get_WorldMatrix();

	return BoneMatrix;
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


HRESULT CHierarchyNode::Initialize(CResource_Bone* pResource, CHierarchyNode* pParent, _uint iDepth, ANIM_DIVIDE eBoneType)
{
	m_eBoneType = eBoneType;

	m_iDepth = iDepth;
	m_pParent = pParent;

	strcpy_s(m_szName, pResource->Get_Name().c_str());

	if (pResource->Get_Name() == "Root" ||
		pResource->Get_Name() == "1B_LongBow_Root"
		)
		m_bMoveNode = true;

	if (pResource->Get_Name() == "0B_L_Thigh" || pResource->Get_Name() == "0B_R_Thigh")
	{
		m_eBoneType = ANIM_DIVIDE::eBODYLOWER;
	}
	else if (pResource->Get_Name() == "0B_COM")
	{
		m_eBoneType = ANIM_DIVIDE::eBODYUPPER;
		m_bCenterNode = true;
	}
	

#ifdef _DEBUG
	string strTemp;
	switch (m_eBoneType)
	{
	case Engine::ANIM_DIVIDE::eDEFAULT:
		strTemp = "DEFAULT";
		break;
	case Engine::ANIM_DIVIDE::eBODYUPPER:
		strTemp = "BODYUPPER";
		break;
	case Engine::ANIM_DIVIDE::eBODYLOWER:
		strTemp = "BODYLOWER";
		break;
	case Engine::ANIM_DIVIDE::eCount:
		break;
	default:
		break;
	}
	cout << pResource->Get_Name() << " : " << strTemp << endl;
#endif // _DEBUG

	



	m_TransformationMatrix = pResource->Get_TransformationMatrix();

	XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_TransformationMatrix,
		XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());
	m_PrevMatrix.Identity();

	vector<CResource_Bone*>& vecChildrenBones = pResource->Get_ChildrenBones();

	for (_uint i = 0; i < pResource->Get_NumChildren(); ++i)
	{
		CHierarchyNode* pNode = CHierarchyNode::Create(vecChildrenBones[i], this, m_iDepth + 1, m_eBoneType);

		if (!pNode)
			return E_FAIL;

		m_pChildrenNodes.push_back(pNode);
	}

	return S_OK;
}

void CHierarchyNode::Update_CombinedTransformationMatrix()
{
	if (m_pParent)
	{
		m_CombinedTransformationMatrix = m_PrevMatrix * m_TransformationMatrix * m_pParent->m_CombinedTransformationMatrix;
		
	}
	else
	{
		m_CombinedTransformationMatrix = m_TransformationMatrix;
	}

	if (m_bMoveNode) 
	{
		//루트노드일 떄 해야대는거
		//3. 포지션 0 으로
		ZeroMemory(&m_CombinedTransformationMatrix.m[3], sizeof(_float3));
	}
}
