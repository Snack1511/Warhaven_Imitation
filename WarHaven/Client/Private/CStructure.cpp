#include "stdafx.h"
#include "CStructure.h"

#include "GameInstance.h"

#include "Texture.h"
#include "CShader.h"
#include "CModel_Renderer.h"
#include "Transform.h"
#include "Functor.h"
#include "CFader.h"
#include "Physics.h"
#include "CMesh_Rect.h"
#include "CMesh_Terrain.h"
#include "Model.h"
#include "CAnimator.h"
#include "CNavigation.h"
#include "CCell.h"
#include "MeshContainer.h"

#include "PhysXCollider.h"
CStructure::CStructure()
{
}

CStructure::~CStructure()
{
}

CStructure* CStructure::Create(wstring MeshPath)
{
	CStructure* pInstance = new CStructure;

	if (FAILED(pInstance->SetUp_Model(MeshPath)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_Model : CStructure");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CStructure");
		return nullptr;
	}

	return pInstance;
}

CStructure* CStructure::Create(wstring MeshPath, _float4x4 ObjectWorldMatrix)
{
	CStructure* pInstance = new CStructure;

	if (FAILED(pInstance->SetUp_Model(MeshPath)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_Model : CStructure");
		return nullptr;
	}

	if (FAILED(pInstance->SetUp_World(ObjectWorldMatrix)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_World : CStructure");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CStructure");
		return nullptr;
	}

	return pInstance;
}

CStructure* CStructure::Create(wstring MeshPath, _float4 vScale, _float4x4 ObjectWorldMatrix)
{
	CStructure* pInstance = new CStructure;

	if (FAILED(pInstance->SetUp_Model(MeshPath)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_Model : CStructure");
		return nullptr;
	}

	if (FAILED(pInstance->SetUp_World(vScale, ObjectWorldMatrix)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_World : CStructure");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CStructure");
		return nullptr;
	}

	return pInstance;
}

void CStructure::Unit_CollisionEnter(CGameObject* pOtherObj, const _uint& eColType)
{
}

void CStructure::Unit_CollisionStay(CGameObject* pOtherObj, const _uint& eColType)
{
}

void CStructure::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
}

void CStructure::Set_Passes(VTXMODEL_PASS_TYPE ePassType)
{
}

HRESULT CStructure::Initialize_Prototype()
{
	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXMODEL,
		VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXMODEL_PASS_DEFAULT
		, _float4(0.f, 0.f, 0.f, 1.f));
	pRenderer->Initialize();
	Add_Component<CRenderer>(pRenderer);


	return S_OK;
}

HRESULT CStructure::Initialize()
{
	m_pModelCom = GET_COMPONENT(CModel);
	//__super::Initialize();

	/* PhysX 메쉬충돌체 굽기 */
	/*for (auto& elem : m_pModelCom->Get_MeshContainers())
	{
		CPhysXCollider* pCol = CPhysXCollider::Create(0, elem.second, m_pTransform);
		Add_Component(pCol);
	}*/
	


	return S_OK;
}

HRESULT CStructure::Start()
{
	__super::Start();

	//m_pTransform->Set_Scale(_float4(0.01f, 0.01f, 0.01f, 0.f));
	//m_pModelCom->Set_ShaderPassToAll(VTXMODEL_PASS_NORMALMAPPING);
    return S_OK;
}

void CStructure::OnEnable()
{
	__super::OnEnable();
}

void CStructure::OnDisable()
{
	__super::OnDisable();
}


HRESULT CStructure::SetUp_Model(wstring strModelPath)
{
	//_float4x4 TransformMatrix = 
	//	XMMatrixScaling(0.00f, 0.005f, 0.005f)
	//	* XMMatrixRotationY(XMConvertToRadians(270.0f));
	m_DebugPath = strModelPath;
	//DEFAULT_TRANS_MATRIX
	//DEFAULT_ST
	CModel* pModel = CModel::Create(0, TYPE_NONANIM, strModelPath, DEFAULT_MODEL_MATRIX);

	Add_Component(pModel);
	return S_OK;
}

HRESULT CStructure::SetUp_World(_float4x4 worldMat)
{
	m_pTransform->Get_Transform().matMyWorld = worldMat;
	m_pTransform->Make_WorldMatrix();
	return S_OK;
}

HRESULT CStructure::SetUp_World(_float4 vScale, _float4x4 worldMat)
{
	m_pTransform->Get_Transform().vScale = vScale;
	m_pTransform->Get_Transform().matMyWorld = worldMat;
	m_pTransform->Make_WorldMatrix();
	return S_OK;
}

void CStructure::My_Tick()
{
	__super::My_Tick();
	//_float4 PickOutPos = _float4(0.f, 0.f, 0.f, 1.f);
	//_float4 PickOutNorm = _float4(0.f, 0.f, 0.f, 1.f);
	//if (KEY(LBUTTON, HOLD))
	//{
	//	if (GAMEINSTANCE->Is_Picked(
	//		this,
	//		&PickOutPos, &PickOutNorm))
	//	{
	//		//m_pTransform->Set_World();
	//		//int a = 0;
	//	}
	//}
}

void CStructure::My_LateTick()
{
	__super::My_LateTick();
}
