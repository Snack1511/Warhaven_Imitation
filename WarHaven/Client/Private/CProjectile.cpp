#include "stdafx.h"
#include "CProjectile.h"

#include "UsefulHeaders.h"

#include "HIerarchyNode.h"
#include "CCollider_Sphere.h"

#include "CColorController.h"

#include "CUnit_Archer.h"

#include "CTeamConnector.h"
#include "CUtility_PhysX.h"
CProjectile::CProjectile()
{
}

CProjectile::CProjectile(const CProjectile& _origin)
	: CGameObject(_origin)
	, m_fMaxSpeed(_origin.m_fMaxSpeed)
	, m_fLoopTimeAcc(_origin.m_fLoopTimeAcc)
	, m_fMaxLoopTime(_origin.m_fMaxLoopTime)
	, m_bCloned(true)
	, m_pConvexMesh(_origin.m_pConvexMesh)
	, m_hcCode(_origin.m_hcCode)
	, m_fMaxDistance(_origin.m_fMaxDistance)
	, m_vArrowHeadPos(_origin.m_vArrowHeadPos)
	, m_fDamage(_origin.m_fDamage)
{
}

CProjectile::~CProjectile()
{
	if (!m_bCloned)
		Safe_release(m_pConvexMesh);
}

void CProjectile::Projectile_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos)
{
	if (COL_PROJECTILECATCH == eOtherColType)
	{
		////COL_PROJECTILECATCH
		//if()

		//if (m_pOwnerUnit->Get_OwnerPlayer()->Get_Team()->Get_TeamType() == eTEAM_TYPE::eRED)
		//{
		//	COL_GROUP_CLIENT eOtherColliderType = (COL_GROUP_CLIENT)eOtherColType;

		//	switch ()
		//	{
		//	default:
		//		break;
		//	}

		//	m_pCollider->Set_ColIndex(COL_BLUEATTACK);

		//	switch (switch_on)
		//	{
		//	default:
		//		break;
		//	}
		//}
		//	
		//else
		//{
		//	Set_ColliderType(m_pOwnerUnit->Get_OwnerPlayer()->Get_Team()->Get_TeamType());
		//}
	}
	else
	{
		m_pOwnerUnit->CallBack_CollisionEnter(pOtherObj, eOtherColType, eMyColType, vHitPos);
		pOtherObj->CallBack_CollisionEnter(m_pOwnerUnit, eMyColType, eOtherColType, vHitPos);

		Hit_Unit(pOtherObj, vHitPos);
	}

}

void CProjectile::Projectile_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
	m_pOwnerUnit->CallBack_CollisionStay(pOtherObj, eOtherColType, eMyColType);
	pOtherObj->CallBack_CollisionStay(m_pOwnerUnit, eMyColType, eOtherColType);
}

void CProjectile::Projectile_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
	m_pOwnerUnit->CallBack_CollisionExit(pOtherObj, eOtherColType, eMyColType);
	pOtherObj->CallBack_CollisionExit(m_pOwnerUnit, eMyColType, eOtherColType);
}

void CProjectile::Reset(CGameObject* pGameObject)
{
	m_pCollider = GET_COMPONENT(CCollider_Sphere);

	m_pOwnerUnit = static_cast<CUnit*>(pGameObject);

	m_pLeftHandBone = GET_COMPONENT_FROM(m_pOwnerUnit, CModel)->Find_HierarchyNode("0B_L_WP1");
	m_pRightHandBone = GET_COMPONENT_FROM(m_pOwnerUnit, CModel)->Find_HierarchyNode("0B_R_WP1");

	if (!m_pLeftHandBone)
		assert(0);

	if (!m_pRightHandBone)
		assert(0);

	On_ChangePhase(eSTART);
	ENABLE_GAMEOBJECT(this);


	if (!m_pOwnerUnit->Get_OwnerPlayer()->Get_Team())
		m_pCollider->Set_ColIndex(COL_BLUEATTACK);
	else
	{
		Set_ColliderType(m_pOwnerUnit->Get_OwnerPlayer()->Get_Team()->Get_TeamType());
	}

	


	

}

_float4 CProjectile::Get_ArrowHeadPos()
{
	return m_vArrowHeadPos.MultiplyCoord(m_pTransform->Get_WorldMatrix());
}

HRESULT CProjectile::Initialize_Prototype()
{
	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXMODEL,
		VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXMODEL_PASS_NORMALMAPPING
		, _float4(0.f, 0.f, 0.f, 1.f));
	pRenderer->Initialize();
	Add_Component<CRenderer>(pRenderer);


	CColorController* pCController = CColorController::Create(CP_BEFORE_RENDERER);

	if (!pCController)
		return E_FAIL;

	Add_Component(pCController);

	if (FAILED(SetUp_Colliders(COL_BLUEATTACK)))
		return E_FAIL;


	

	
	return S_OK;
}

HRESULT CProjectile::Initialize()
{
    return S_OK;
}

HRESULT CProjectile::Start()
{
	__super::Start();

	CallBack_CollisionEnter += bind(&CProjectile::Projectile_CollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
	CallBack_CollisionStay += bind(&CProjectile::Projectile_CollisionStay, this, placeholders::_1, placeholders::_2, placeholders::_3);
	CallBack_CollisionExit += bind(&CProjectile::Projectile_CollisionExit, this, placeholders::_1, placeholders::_2, placeholders::_3);
	 

	DISABLE_COMPONENT(GET_COMPONENT(CCollider_Sphere));

    return S_OK;
}


void CProjectile::On_ShootProjectile()
{
	_float4 vLook = m_pOwnerUnit->Get_Transform()->Get_World(WORLD_LOOK);

	vLook = m_pTransform->Get_World(WORLD_RIGHT);
	ENABLE_COMPONENT(GET_COMPONENT(CCollider_Sphere));

	On_ChangePhase(eSHOOT);

	m_vStartPosition = m_pTransform->Get_World(WORLD_POS);



	/* PhysX */
	PxTransform tTransform;
	ZeroMemory(&tTransform, sizeof(PxTransform));

	_float4 vCurPos = m_pTransform->Get_World(WORLD_POS);
	vCurPos += m_pTransform->Get_World(WORLD_RIGHT) * 2.5f;

	tTransform.p = CUtility_PhysX::To_PxVec3(vCurPos);
	tTransform.q = CUtility_PhysX::To_PxQuat(m_pTransform->Get_Quaternion());

	PxRigidDynamic* pActor = nullptr;
	pActor = GAMEINSTANCE->Create_DynamicActor(tTransform, PxConvexMeshGeometry(m_pConvexMesh), CPhysX_Manager::SCENE_CURRENT, 1.5f);
	_float4 vDir = m_pTransform->Get_World(WORLD_RIGHT);
	vDir *= m_fMaxSpeed;
	pActor->addForce(CUtility_PhysX::To_PxVec3(vDir));
	m_pActor = pActor;
		

}

void CProjectile::On_ChangePhase(ePROJECTILE_PHASE eNextPhase)
{
	if (eNextPhase >= eEND)
		return;

	switch (eNextPhase)
	{
	case Client::CProjectile::eSTART:
		m_pCurStickBone = m_pRightHandBone;
		break;
	case Client::CProjectile::eLOOP:
		m_pCurStickBone = m_pLeftHandBone;
		break;
	case Client::CProjectile::eSHOOT:
		ENABLE_COMPONENT(m_pCollider);
		m_pCurStickBone = nullptr;
		break;
	case Client::CProjectile::eHIT:
		DISABLE_COMPONENT(m_pCollider);
		break;
	case eSTICK:
		DISABLE_COMPONENT(m_pCollider);
		Safe_release(m_pActor);
	case Client::CProjectile::eEND:
		break;
	default:
		break;
	}

	m_eCurPhase = eNextPhase;
}

HRESULT CProjectile::SetUp_Projectile(wstring wstrModelFilePath)
{
	_float4x4 matIdentity;
	matIdentity.Identity();

	// 회전각 인자 만들어주기.
	CModel* pModel = CModel::Create(CP_BEFORE_RENDERER, TYPE_NONANIM, wstrModelFilePath,
		XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(270.f)) * XMMatrixRotationX(XMConvertToRadians(90.0f))
	);

	if (!pModel)
		return E_FAIL;

	pModel->Initialize();
	Add_Component(pModel);
	pModel->Set_ShaderFlag(SH_LIGHT_BLOOM);
	pModel->Set_ShaderPassToAll(VTXMODEL_PASS_NORMALMAPPING);


	/* PhysX */
	CMeshContainer* pMesh = (pModel->Get_MeshContainers().front().second);

	FACEINDICES32* pIndices = pMesh->CMesh::Get_Indices();
	_uint iNumPrimitive = pMesh->Get_NumPrimitive();

	_uint iNumVertices = pMesh->Get_NumVertices();
	_float3* pVerticesPos = pMesh->Get_VerticesPos();

	GAMEINSTANCE->Create_ConvexMesh(
		pVerticesPos,
		iNumVertices,
		pIndices,
		iNumPrimitive,
		&m_pConvexMesh);

	if (!m_pConvexMesh)
		return E_FAIL;

	
	return S_OK;

}

HRESULT CProjectile::SetUp_Colliders(COL_GROUP_CLIENT eColType)
{
	_float fRadius = 0.25f;
	_float4 vOffsetPos = ZERO_VECTOR;
	//vOffsetPos.z += fRadius;
	//vOffsetPos.z += fRadius;
	//vOffsetPos.z += fRadius;

	CCollider_Sphere* pCollider = CCollider_Sphere::Create(CP_AFTER_TRANSFORM, fRadius, eColType, vOffsetPos, DEFAULT_TRANS_MATRIX);
	vOffsetPos.x += fRadius;
	pCollider->Add_Collider(fRadius, vOffsetPos);
	vOffsetPos.x += fRadius;
	pCollider->Add_Collider(fRadius, vOffsetPos);
	vOffsetPos.x += fRadius;
	pCollider->Add_Collider(fRadius, vOffsetPos);
	vOffsetPos.x += fRadius;
	pCollider->Add_Collider(fRadius, vOffsetPos);
	vOffsetPos.x += fRadius;
	pCollider->Add_Collider(fRadius, vOffsetPos);

	Add_Component(pCollider);

	m_pCollider = pCollider;

	if (!m_pCollider)
		return E_FAIL;


	


	return S_OK;
}

void CProjectile::My_Tick()
{
}

void CProjectile::My_LateTick()
{

	switch (m_eCurPhase)
	{
	case Client::CProjectile::eSTART:
	case Client::CProjectile::eLOOP:
		if (m_pCurStickBone)
		{
			_float4x4		matBone = m_pCurStickBone->Get_BoneMatrix();

			m_pTransform->Get_Transform().matMyWorld = matBone;

			m_pTransform->Make_WorldMatrix();
		}
		break;
	case Client::CProjectile::eSHOOT:
	{
		/* PhysX 따라가기 */
		PxTransform tTransform = m_pActor->getGlobalPose();
		_float4x4 matPhysX = CUtility_PhysX::To_Matrix(tTransform);
		m_pTransform->Get_Transform().matMyWorld = matPhysX;

		m_pTransform->Make_WorldMatrix();

		_float fPower = CUtility_PhysX::To_Vector(m_pActor->getLinearVelocity()).Length();

		if (fPower < 25.f)
			On_ChangePhase(eHIT);

		_float fLength = (m_vStartPosition - m_pTransform->Get_World(WORLD_POS)).Length();

		if (fLength > m_fMaxDistance)
			DISABLE_GAMEOBJECT(this);
	}
		break;
	case Client::CProjectile::eHIT:
		/* 땅에 떨어졌을 떄 */
	{
		/* PhysX 따라가기 */
		PxTransform tTransform = m_pActor->getGlobalPose();
		_float4x4 matPhysX = CUtility_PhysX::To_Matrix(tTransform);
		m_pTransform->Get_Transform().matMyWorld = matPhysX;

		m_pTransform->Make_WorldMatrix();

		m_fLoopTimeAcc += fDT(0);
		if (m_fLoopTimeAcc >= m_fMaxLoopTime)
		{
			DISABLE_GAMEOBJECT(this);
		}
	}
		break;
	case Client::CProjectile::eSTICK:
		/* Unit에 꽂힘 */
	{
		if (!m_pHitUnit->Is_Valid())
			DISABLE_GAMEOBJECT(this);

		_float4x4 matCurWorld = m_pCurStickBone->Get_BoneMatrix();
		matCurWorld = m_matHitOffset * matCurWorld;

		m_pTransform->Get_Transform().matMyWorld = matCurWorld;
		m_pTransform->Make_WorldMatrix();
	}
		break;
	case Client::CProjectile::eEND:
		break;
	default:
		break;
	}
	
}

void CProjectile::OnEnable()
{
	__super::OnEnable();
	DISABLE_COMPONENT(GET_COMPONENT(CCollider_Sphere));
}

void CProjectile::OnDisable()
{
	__super::OnDisable();

	static_cast<CUnit_Archer*>(m_pOwnerUnit)->Collect_Arrow(m_hcCode, this);
	Safe_release(m_pActor);
	m_fLoopTimeAcc = 0.f;

}

void CProjectile::Hit_Unit(CGameObject* pHitUnit, _float4 vHitPos)
{
	_float4 vCurPos = m_pTransform->Get_World(WORLD_POS);
	_float4 vDir = (Get_ArrowHeadPos() - vHitPos);
	vCurPos -= vDir * 0.5f;

	m_pTransform->Set_World(WORLD_POS, vCurPos);
	m_pTransform->Make_WorldMatrix();


	m_pHitUnit = pHitUnit;
	On_ChangePhase(eSTICK);
	m_pCurStickBone = GET_COMPONENT_FROM(pHitUnit, CModel)->Find_HierarchyNode("0B_COM");
	
	//맞은 순간에 worldmat과 맞은 놈의 월드 inverse
	_float4x4 matWorldInv = m_pCurStickBone->Get_BoneMatrix().Inverse();
	m_matHitOffset = m_pTransform->Get_WorldMatrix() * matWorldInv;

	/**((_float4*)&m_matHitOffset.m[0]) = ((_float4*)&m_matHitOffset.m[0])->Normalize();
	*((_float4*)&m_matHitOffset.m[1]) = ((_float4*)&m_matHitOffset.m[1])->Normalize();
	*((_float4*)&m_matHitOffset.m[2]) = ((_float4*)&m_matHitOffset.m[2])->Normalize();*/

	 
}

void CProjectile::Set_ColliderType(eTEAM_TYPE eTeamType)
{
	if (eTeamType == eTEAM_TYPE::eBLUE)
		m_pCollider->Set_ColIndex(COL_BLUEATTACK);
	else
		m_pCollider->Set_ColIndex(COL_REDATTACK);
}
