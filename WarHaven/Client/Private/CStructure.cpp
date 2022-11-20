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

#include "CUtility_Transform.h"
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

void CStructure::Unit_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos)
{
}

void CStructure::Unit_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
}

void CStructure::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
}

void CStructure::Set_Passes(VTXMODEL_PASS_TYPE ePassType)
{
}

void CStructure::Make_PhysXCollider(ePhysXEnum eShapeType, _uint iLODLevel)
{
	//기존 피직스부터 다 꺼야함
	if (eShapeType >= ePhysXEnum::eBOX)
		return;

	m_eCurType = eShapeType;

	for (auto& elem : m_vecPhysXColliders)
	{
		DELETE_COMPONENT(elem, this);
	}

	m_vecPhysXColliders.clear();
	m_mapComponents[HASHCODE(CPhysXCollider)].clear();

	m_vecColliderPos.clear();
	m_vecColliderScale.clear();
	m_vecAngles.clear();
	
	
	switch (eShapeType)
	{
	case Client::CStructure::ePhysXEnum::eCONVEX:

		for (auto& elem : m_pModelCom->Get_MeshContainers())
		{
			if (elem.first != iLODLevel)
				continue;

			CPhysXCollider* pCol = CPhysXCollider::Create_Convex(0, elem.second, m_pTransform);
			if (!pCol)
				continue;
			Add_Component(pCol);
			m_vecPhysXColliders.push_back(pCol);

		}

		break;
	case Client::CStructure::ePhysXEnum::eTRIANGLE:
		

		for (auto& elem : m_pModelCom->Get_MeshContainers())
		{
			if (elem.first != iLODLevel)
				continue;
			CPhysXCollider* pCol = CPhysXCollider::Create(0, elem.second, m_pTransform);
			if (!pCol)
				continue;
			Add_Component(pCol);
			m_vecPhysXColliders.push_back(pCol);
		}

		break;
	case Client::CStructure::ePhysXEnum::eBOX:
		break;
	default:
		break;
	}

	if (m_vecPhysXColliders.empty())
		Call_MsgBox(L"LOD없어서 충돌체 굽기 실패!");
	else
	{
		m_iCOlliderSourceLOD = iLODLevel;
	}
}

CPhysXCollider* CStructure::Make_PhysXCollier_Box()
{
	/* 박스가 아니었으면 기존 꺼 다지우고 박스 하나 생성*/
	if (m_eCurType != ePhysXEnum::eBOX)
	{
		m_eCurType = ePhysXEnum::eBOX;

		for (auto& elem : m_vecPhysXColliders)
		{
			DELETE_COMPONENT(elem, this);
		}

		m_vecPhysXColliders.clear();
		m_mapComponents[HASHCODE(CPhysXCollider)].clear();

		m_vecColliderPos.clear();
		m_vecColliderScale.clear();
		m_vecAngles.clear();
	}


	CPhysXCollider::PHYSXCOLLIDERDESC		tPhysXColliderDesc;
	ZeroMemory(&tPhysXColliderDesc, sizeof(CPhysXCollider::PHYSXCOLLIDERDESC));

	tPhysXColliderDesc.eShape = CPhysXCollider::COLLIDERSHAPE::BOX;
	tPhysXColliderDesc.eType = CPhysXCollider::COLLIDERTYPE::STATIC;
	tPhysXColliderDesc.fDensity = 1.f;

	//위치는 현재 객체 가운데로?

	tPhysXColliderDesc.vPosition = m_pTransform->Get_World(WORLD_POS);

	tPhysXColliderDesc.vQuat = m_pTransform->Get_Quaternion();
	tPhysXColliderDesc.vPosition = m_pTransform->Get_World(WORLD_POS);
	tPhysXColliderDesc.vScale = _float4(1.f, 1.f, 1.f);

	CPhysXCollider* pPhysXCollider = CPhysXCollider::Create(CP_BEFORE_TRANSFORM, tPhysXColliderDesc);
	Add_Component(pPhysXCollider);
	m_vecPhysXColliders.push_back(pPhysXCollider);

	m_vecColliderPos.push_back(tPhysXColliderDesc.vPosition);
	m_vecAngles.push_back(tPhysXColliderDesc.vQuat);
	m_vecColliderScale.push_back(tPhysXColliderDesc.vScale);


	return pPhysXCollider;
}


void CStructure::RePosition_Box(_uint iIndex, _float4 vOffsetPosition)
{
	if (m_eCurType != ePhysXEnum::eBOX)
		return;

	if (iIndex >= m_vecPhysXColliders.size())
	{
		Call_MsgBox(L"PhysX충돌체 갯수보다 높은 수의 인덱스로 접근 : CStructure");
		return;
	}

	_float4 vWorldPos = m_pTransform->Get_World(WORLD_POS);
	vWorldPos += vOffsetPosition;

	m_vecPhysXColliders[iIndex]->Set_Position(vWorldPos.XMLoad());
	m_vecColliderPos[iIndex] = vWorldPos;
}

void CStructure::ReScale_Box(_uint iIndex, _float4 vScale)
{
	if (m_eCurType != ePhysXEnum::eBOX)
		return;

	if (iIndex >= m_vecPhysXColliders.size())
	{
		Call_MsgBox(L"PhysX충돌체 갯수보다 높은 수의 인덱스로 접근 : CStructure");
		return;
	}

	m_vecPhysXColliders[iIndex]->Set_Scale(vScale.XMLoad());
	m_vecColliderScale[iIndex] = vScale;
}

void CStructure::Rotate_Box(_uint iIndex, _float4 vAngles)
{
	if (m_eCurType != ePhysXEnum::eBOX)
		return;

	if (iIndex >= m_vecPhysXColliders.size())
	{
		Call_MsgBox(L"PhysX충돌체 갯수보다 높은 수의 인덱스로 접근 : CStructure");
		return;
	}

	_float4 vQuat = XMQuaternionRotationRollPitchYawFromVector(vAngles.XMLoad());

	m_vecPhysXColliders[iIndex]->Rotate(vQuat.XMLoad());
	m_vecAngles[iIndex] = vQuat;
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
	
	//Make_PhysXCollider(ePhysXEnum::eCONVEX);


	return S_OK;
}

HRESULT CStructure::Start()
{
	__super::Start();

	//m_pTransform->Set_Scale(_float4(0.01f, 0.01f, 0.01f, 0.f));
	m_pModelCom->Set_ShaderPassToAll(VTXMODEL_PASS_NORMALMAPPING);
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

}

void CStructure::My_LateTick()
{
	__super::My_LateTick();
}

_uint CStructure::Get_BoxCount()
{
	if (m_eCurType != ePhysXEnum::eBOX)
		return 0;
	if (m_vecPhysXColliders.empty())
		return 0;

	return _uint(m_vecPhysXColliders.size());
}
