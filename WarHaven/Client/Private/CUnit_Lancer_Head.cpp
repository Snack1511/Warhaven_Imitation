#include "stdafx.h"
#include "CUnit_Lancer_Head.h"

#include "UsefulHeaders.h"
#include "CTeamConnector.h"
#include "CCollider_Sphere.h"
#include "HIerarchyNode.h"
#include "CUnit.h"

CUnit_Lancer_Head::CUnit_Lancer_Head()
{
}

CUnit_Lancer_Head::~CUnit_Lancer_Head()
{
}

CUnit_Lancer_Head* CUnit_Lancer_Head::Create(wstring wstrModelFilePath, CHierarchyNode* pOwnerHierarcyNode, CUnit* pOwnerUnit)
{
	CUnit_Lancer_Head* pInstance = new CUnit_Lancer_Head;


	if (FAILED(pInstance->CUnit_Lancer_Head::SetUp_Model(wstrModelFilePath, pOwnerHierarcyNode, pOwnerUnit)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_Model : CUnit_Lancer_Head");
		return nullptr;
	}

	if (FAILED(pInstance->CUnit_Lancer_Head::Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CUnit_Lancer_Head");
		return nullptr;
	}


	return pInstance;
}

HRESULT CUnit_Lancer_Head::SetUp_Model(wstring wstrModelFilePath, CHierarchyNode* pOwnerHierarcyNode, CUnit* pOwnerUnit)
{
	
	CModel* pModel = CModel::Create(CP_BEFORE_RENDERER, TYPE_NONANIM, wstrModelFilePath, XMMatrixScaling(0.01f, 0.01f, 0.01f) * 
		XMMatrixRotationX(XMConvertToRadians(90.f)) * XMMatrixRotationZ(XMConvertToRadians(0.f)) * XMMatrixRotationY(XMConvertToRadians(90.f))
	);// DEFAULT_MODEL_MATRIX);

	if (!pModel)
		return E_FAIL;

	pModel->Initialize();
	Add_Component(pModel);
	pModel->Set_ShaderFlag(SH_LIGHT_BLOOM);
	pModel->Set_ShaderPassToAll(VTXMODEL_PASS_NORMALMAPPING);

	//pModel->Initialize();
	//Add_Component(pModel);
	//pModel->Set_ShaderFlag(SH_LIGHT_BLOOM);
	//pModel->Set_ShaderPassToAll(VTXMODEL_PASS_NORMALMAPPING);


	m_pOwnerBone = pOwnerHierarcyNode;
	m_pOwnerUnit = pOwnerUnit;

	return S_OK;
}

HRESULT CUnit_Lancer_Head::Initialize_Prototype()
{

	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXMODEL,
		VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXMODEL_PASS_NORMALMAPPING
		, _float4(0.f, 0.f, 0.f, 1.f));

	pRenderer->Initialize();
	Add_Component<CRenderer>(pRenderer);

	return S_OK;
}

HRESULT CUnit_Lancer_Head::Initialize()
{
	return S_OK;
}

HRESULT CUnit_Lancer_Head::Start()
{
	__super::Start();

	return S_OK;
}

void CUnit_Lancer_Head::My_Tick()
{
	__super::My_Tick();
}

void CUnit_Lancer_Head::My_LateTick()
{
	__super::My_LateTick();

	if (m_pOwnerBone)
	{
		CHierarchyNode* pNode = GET_COMPONENT_FROM(m_pOwnerUnit, CModel)->Find_HierarchyNode("0B_Neck");//("0B_Head");
		CHierarchyNode* pPrentNode = GET_COMPONENT_FROM(m_pOwnerUnit, CModel)->Find_HierarchyNode("0B_Neck");

		//_float4x4 BoneMatrix;
		//CHierarchyNode* pNode = Find_HierarchyNode(pBonename);
		//if (!pNode)
		//	return BoneMatrix.Identity();

		//BoneMatrix = pNode->Get_CombinedMatrix();

		//BoneMatrix *= m_TransformMatrix;
		//BoneMatrix *= m_pOwner->Get_Transform()->Get_WorldMatrix();

		//GET_COMPONENT_FROM(m_pOwnerUnit, CModel)->Get_ModelType();


		if (pNode == nullptr)
			return;

		_float4x4		matBone = pNode->Get_BoneMatrix();

		_float4x4 matWorldInv = pNode->Get_BoneMatrix().Inverse();
		_float4x4 matOffset = pNode->Get_BoneMatrix() * matWorldInv;

		matBone = matOffset * matBone;

		_float4 vPos = m_vPos;

		////matBone.m[3][0] = vPos.x;
		////matBone.m[3][1] = vPos.y - 1.0f;
		////matBone.m[3][2] = vPos.z;


		vPos = vPos.MultiplyCoord(matBone);
		m_pTransform->Set_World(WORLD_POS, vPos);

		matBone.m[3][0] = vPos.x;
		matBone.m[3][1] = vPos.y;
		matBone.m[3][2] = vPos.z;

		m_pTransform->Get_Transform().matMyWorld = matBone;


		m_pTransform->Make_WorldMatrix();

	}
}

void CUnit_Lancer_Head::OnEnable()
{
	__super::OnEnable();
}

void CUnit_Lancer_Head::OnDisable()
{
	__super::OnDisable();

}
