#include "stdafx.h"
#include "CProjectile.h"

#include "UsefulHeaders.h"

#include "HIerarchyNode.h"
#include "CCollider_Sphere.h"

#include "CColorController.h"

#include "CUnit_Archer.h"
#include "CUnit_Qanda.h"

#include "CTeamConnector.h"
#include "CUtility_PhysX.h"

#include "CTrailEffect.h"

#include "CTrailBuffer.h"
#include "Easing_Utillity.h"
#include "CUI_UnitHUD.h"

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
	, m_szMainBoneName(_origin.m_szMainBoneName)
	, m_szSubBoneName(_origin.m_szSubBoneName)
{
}

CProjectile::~CProjectile()
{
	if (!m_bCloned)
		Safe_release(m_pConvexMesh);
}

void CProjectile::Projectile_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos)
{
	if (COL_REDPROJECTILECATCH == eOtherColType || COL_BLUEPROJECTILECATCH == eOtherColType)
	{
		CUnit* pOtherUnit = dynamic_cast<CUnit*>(pOtherObj);
		
		if (m_pOwnerUnit == pOtherUnit && !pOtherUnit)
			return;

		m_pOwnerUnit = pOtherUnit;
		m_bUseOwnerUnitLook = true;

		DISABLE_COMPONENT(m_pCollider);
		pOtherUnit->Catch_ProjectileObject(this);

		const char* pRightBoneName = m_szMainBoneName.c_str();
		const char* pLeftBoneName = m_szSubBoneName.c_str();

		m_pLeftHandBone = GET_COMPONENT_FROM(m_pOwnerUnit, CModel)->Find_HierarchyNode(pLeftBoneName);
		m_pRightHandBone = GET_COMPONENT_FROM(m_pOwnerUnit, CModel)->Find_HierarchyNode(pRightBoneName);
		On_ChangePhase(eCATCHED);

		COL_GROUP_CLIENT eColType = (COL_GROUP_CLIENT)m_pCollider->Get_ColIndex();

		if (eColType == COL_REDATTACK)
			m_pCollider->Set_ColIndex(COL_BLUEATTACK);
		
		else if (eColType == COL_BLUEATTACK)
			m_pCollider->Set_ColIndex(COL_REDATTACK);



		else if (eColType == COL_REDGUARDBREAK)
			m_pCollider->Set_ColIndex(COL_BLUEGUARDBREAK);

		else if (eColType == COL_BLUEGUARDBREAK)
			m_pCollider->Set_ColIndex(COL_REDGUARDBREAK);



		else if (eColType == COL_REDGROGGYATTACK)
			m_pCollider->Set_ColIndex(COL_BLUEGROGGYATTACK);

		else if (eColType == COL_BLUEGROGGYATTACK)
			m_pCollider->Set_ColIndex(COL_REDGROGGYATTACK);



		else if (eColType == COL_REDFLYATTACK)
			m_pCollider->Set_ColIndex(COL_BLUEFLYATTACK);

		else if (eColType == COL_BLUEFLYATTACK)
			m_pCollider->Set_ColIndex(COL_REDFLYATTACK);



		else if (eColType == COL_REDFLYATTACKGUARDBREAK)
			m_pCollider->Set_ColIndex(COL_BLUEFLYATTACKGUARDBREAK);
			
		else if (eColType == COL_BLUEFLYATTACKGUARDBREAK)
			m_pCollider->Set_ColIndex(COL_REDFLYATTACKGUARDBREAK);

	}
	else
	{
		if (m_bHit)
			return;

		m_bHit = true;

		m_pOwnerUnit->CallBack_CollisionEnter(pOtherObj, eOtherColType, eMyColType, vHitPos);
		pOtherObj->CallBack_CollisionEnter(m_pOwnerUnit, eMyColType, eOtherColType, vHitPos);
		_bool bHeadShot = false;
		if (eOtherColType == COL_REDHITBOX_HEAD || eOtherColType == COL_BLUEHITBOX_HEAD)
			bHeadShot = true;

		Hit_Unit(pOtherObj, vHitPos, bHeadShot);

		
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

	const char* pRightBoneName = m_szMainBoneName.c_str();
	const char* pLeftBoneName = m_szSubBoneName.c_str();

	m_pLeftHandBone = GET_COMPONENT_FROM(m_pOwnerUnit, CModel)->Find_HierarchyNode(pLeftBoneName);
	m_pRightHandBone = GET_COMPONENT_FROM(m_pOwnerUnit, CModel)->Find_HierarchyNode(pRightBoneName);

	if (!m_pLeftHandBone)
		assert(0);

	if (!m_pRightHandBone)
		assert(0);

	On_ChangePhase(eSTART);
	ENABLE_GAMEOBJECT(this);


	if (!m_pOwnerUnit->Get_OwnerPlayer()->Get_Team())
		m_pCollider->Set_ColIndex(COL_BLUEATTACK);
		//m_pCollider->Set_ColIndex(COL_REDATTACK);
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
	ENABLE_COMPONENT(GET_COMPONENT(CCollider_Sphere));

	On_ChangePhase(eSHOOT);

	m_vStartPosition = m_pTransform->Get_World(WORLD_POS);

	if (m_pTrailEffect)
	{
		m_pTrailEffect->TurnOn_TrailEffect(true);
		m_pTrailEffect2->TurnOn_TrailEffect(true);
	}



	/* PhysX */
	PxTransform tTransform;
	ZeroMemory(&tTransform, sizeof(PxTransform));

	_float4 vCurPos = m_pTransform->Get_World(WORLD_POS);
	vCurPos += m_pTransform->Get_World(WORLD_RIGHT) * 2.5f;

	tTransform.p = CUtility_PhysX::To_PxVec3(vCurPos);
	tTransform.q = CUtility_PhysX::To_PxQuat(m_pTransform->Get_Quaternion());

	PxRigidDynamic* pActor = nullptr;
	pActor = GAMEINSTANCE->Create_DynamicActor(tTransform, PxConvexMeshGeometry(m_pConvexMesh), CPhysX_Manager::SCENE_CURRENT, 1.5f);

	_float4 vDir = ZERO_VECTOR;
	
	vDir = m_pTransform->Get_World(WORLD_RIGHT);
	vDir *= m_fMaxSpeed;
	pActor->addForce(CUtility_PhysX::To_PxVec3(vDir));
	m_pActor = pActor;
	


}

void CProjectile::On_ChangePhase(ePROJECTILE_PHASE eNextPhase)
{
	_float4x4 matOwner;
	_float4 vOffset;
	_float fRandom;
	_float fRandomZ;

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
	case Client::CProjectile::eChase:
		ENABLE_COMPONENT(m_pCollider);
		m_pCurStickBone = nullptr;
		break;

	case Client::CProjectile::eRANDOM:
		m_pCurStickBone = nullptr;
		//m_vRandLook = _float4(frandom(-0.05f, 0.05f), frandom(0.05f, 0.2f), frandom(0.05f, 0.3f));
		//m_vRandLook *= fDT(0) * m_fMaxSpeed * 2.f;
		////m_pTransform->Set_World(WORLD_POS, m_pOwnerUnit->Get_Transform()->Get_World(WORLD_POS));
		//m_fRandomPhaseMaxTime = frandom(0.3f, 0.7f);

		matOwner = m_pOwnerUnit->Get_Transform()->Get_WorldMatrix();
		fRandom = frandom(-1.f, 1.f);
		fRandomZ = frandom(-1.f, 1.f);
		vOffset = _float4(fRandom, 1.5f, fRandomZ);

		vOffset = vOffset.MultiplyCoord(matOwner);
		m_pTransform->Set_World(WORLD_POS, vOffset);


		m_vTargetPos = _float4(0.f, 5.f, 3.f);
		m_vTargetPos = m_vTargetPos.MultiplyCoord(matOwner);

		m_vRight = m_pOwnerUnit->Get_Transform()->Get_World(WORLD_RIGHT).Normalize();

		fRandom = random(0, 1);
		m_fRandSpeed = frandom(2.f, 3.f);
		m_fRandFrequency = frandom(1.f, 5.f);
		m_fRandPower = frandom(3.f, 6.f);

		if (0.5f > fRandom)
			m_fRandFrequency *= -1.f;

		break;

	case Client::CProjectile::eHIT:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Arrow_Hit", Get_ArrowHeadPos());
		DISABLE_COMPONENT(m_pCollider);
		if (m_pTrailEffect)
		{
			m_pTrailEffect->TurnOn_TrailEffect(false);
			m_pTrailEffect2->TurnOn_TrailEffect(false);
		}
		break;
	case eSTICK:
		DISABLE_COMPONENT(m_pCollider);
		if (m_pTrailEffect)
		{
			m_pTrailEffect->TurnOn_TrailEffect(false);
			m_pTrailEffect2->TurnOn_TrailEffect(false);
		}
		Safe_release(m_pActor);
		break;

	case eCATCHED:
		m_pCurStickBone = m_pLeftHandBone;
		break;
	case Client::CProjectile::eEND:
		break;
	default:
		break;
	}

	m_eCurPhase = eNextPhase;
}

void CProjectile::SetUp_TrailEffect(_float4 vWeaponLow, _float4 vWeaponHigh, _float4 vWeaponLeft, _float4 vWeaponRight, _float4 vGlowFlag, _float4 vColor, _float fWeaponCenter, wstring wstrMaskMapPath, wstring wstrColorMapPath, _uint iTrailCount)
{
	m_pTrailEffect = CTrailEffect::Create(0, iTrailCount, vWeaponLow, vWeaponHigh,
		nullptr, m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	m_pTrailEffect2 = CTrailEffect::Create(0, iTrailCount, vWeaponLeft, vWeaponRight,
		nullptr, m_pTransform, vGlowFlag, vColor,
		wstrMaskMapPath, wstrColorMapPath);

	if (!m_pTrailEffect)
		return;

	CREATE_GAMEOBJECT(m_pTrailEffect, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrailEffect, CMesh))->Set_NoCurve();

	CREATE_GAMEOBJECT(m_pTrailEffect2, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrailEffect2, CMesh))->Set_NoCurve();

	m_pTrailEffect->TurnOn_TrailEffect(false);
	m_pTrailEffect2->TurnOn_TrailEffect(false);
}

void CProjectile::Turn_Trail(_bool bOnOff)
{
	if (m_pTrailEffect)
	{
		m_pTrailEffect->TurnOn_TrailEffect(bOnOff);
		m_pTrailEffect2->TurnOn_TrailEffect(bOnOff);
	}
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
	CMeshContainer* pMesh = (m_bFrontPhysX) ? (pModel->Get_MeshContainers().front().second) : (pModel->Get_MeshContainers().back().second);

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

HRESULT	CProjectile::SetUp_Collider(COL_GROUP_CLIENT eColType, _float fRadian)
{
	_float4 vOffsetPos = ZERO_VECTOR;

	CCollider_Sphere* pCollider = CCollider_Sphere::Create(CP_AFTER_TRANSFORM, fRadian, eColType, vOffsetPos, DEFAULT_TRANS_MATRIX);

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
	_float4 vPos = m_pTransform->Get_World(WORLD_POS);
	_float4 vLook;
	_float fY, fX;
	_float fSpeed;
	_float fTimeDelta = fDT(0);
	_float fRandom;

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
	case Client::CProjectile::eRANDOM:
	{
		m_fRandomPhaseCurTime += fTimeDelta;
		m_fTimeAcc += fTimeDelta;

		vLook = m_vTargetPos - vPos;
		vLook.Normalize();

		m_pTransform->Set_LerpLook(vLook, 0.2f);
		
		vPos += vLook * m_fRandSpeed * fTimeDelta;

		fSpeed = m_fRandFrequency * m_fTimeAcc;

		fX = m_fRandPower * sinf(fSpeed * PI * 0.5f);

		fY = m_fRandPower * cosf(-1.f * fSpeed * PI * 0.5f);
			
		vPos.x += fX * vLook.x * fTimeDelta;
		vPos.y += fX * vLook.y * fTimeDelta;
		vPos.z += fX * vLook.z * fTimeDelta;

		vPos.x += fY * m_vRight.x * fTimeDelta;
		vPos.y += fY * m_vRight.y * fTimeDelta;
		vPos.z += fY * m_vRight.z * fTimeDelta;
		
		m_pTransform->Set_World(WORLD_POS, vPos);

		if (1.5f < m_fTimeAcc)
		{
			On_ChangePhase(eChase);
			m_fTimeAcc = 0.f;

			fRandom = random(0, 1);
			m_fRandSpeed = frandom(3.f, 5.f);
			m_fRandFrequency = random(1, 10);
			m_fRandPower = frandom(20.f, 30.f);

			if (0.5f > fRandom)
				m_fRandFrequency *= -1.f;
		}
		
	}
		break;
	case Client::CProjectile::eChase:
	{
		m_fLoopTimeAcc += fTimeDelta;

		if (!m_pTargetUnit)
			assert(0);

		_float4 vLook = m_pTargetUnit->Get_Transform()->Get_World(WORLD_POS) - m_pTransform->Get_World(WORLD_POS);

		if (0.f >= m_pTargetUnit->Get_Status().fHP || !m_pTargetUnit->Is_Valid())
		{
			if (0.5f > vLook.Length())
				DISABLE_GAMEOBJECT(this);		
		}

		vLook.Normalize();

		m_pTransform->Set_LerpLook(vLook, 0.2f);

		m_fRandSpeed = CEasing_Utillity::SinIn(m_fRandSpeed, 30.f, m_fLoopTimeAcc, 1.f);
		//m_fPower = CEasing_Utillity::SinIn(m_fSpeed, 50.f, m_fLoopTimeAcc, 10.f);

		vPos += vLook * m_fRandSpeed * fTimeDelta;

		fSpeed = m_fRandFrequency * m_fLoopTimeAcc;

		fX = m_fRandPower * sinf(fSpeed * PI * 0.5f);

		fY = m_fRandPower * cosf(-1.f * fSpeed * PI * 0.5f);

		vPos.x += fX * vLook.x * fTimeDelta;
		vPos.y += fX * vLook.y * fTimeDelta;
		vPos.z += fX * vLook.z * fTimeDelta;

		vPos.x += fY * m_vRight.x * fTimeDelta;
		vPos.y += fY * m_vRight.y * fTimeDelta;
		vPos.z += fY * m_vRight.z * fTimeDelta;

		m_pTransform->Set_World(WORLD_POS, vPos);
	}

	

		break;

	case Client::CProjectile::eSHOOT:
	{
		/* PhysX 따라가기 */
		PxTransform tTransform = m_pActor->getGlobalPose();
		_float4x4 matPhysX = CUtility_PhysX::To_Matrix(tTransform);
		m_pTransform->Get_Transform().matMyWorld = matPhysX;

		m_pTransform->Make_WorldMatrix();

		_float fPower = CUtility_PhysX::To_Vector(m_pActor->getLinearVelocity()).Length() * fDT(0);

		if (fPower < 25.f * fDT(0))
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

		if (m_pCurStickBone)
		{
			_float4x4 matCurWorld = m_pCurStickBone->Get_BoneMatrix();
			matCurWorld = m_matHitOffset * matCurWorld;

			m_pTransform->Get_Transform().matMyWorld = matCurWorld;
			m_pTransform->Make_WorldMatrix();
		}
		else
			DISABLE_GAMEOBJECT(this);


	}
		break;

	case Client::CProjectile::eCATCHED:

	{
		_float4 vTargetPos;
		memcpy(&vTargetPos, m_pCurStickBone->Get_BoneMatrix().m[3], sizeof(_float4));

		vTargetPos += m_pOwnerUnit->Get_Transform()->Get_World(WORLD_LOOK) * 1.3f;

		_float4 vCurPos = m_pTransform->Get_World(WORLD_POS);

		_float4 vDir = vTargetPos - vCurPos;
		_float fLength = vDir.Length();

		_float fSpeed = 4.f;

		vCurPos += vDir * fSpeed * fDT(0);

		m_pTransform->Set_World(WORLD_POS, vCurPos);
		m_pTransform->Set_LerpLook(m_pOwnerUnit->Get_Transform()->Get_World(WORLD_RIGHT), 0.4f);
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
	m_bHit = false;
}

void CProjectile::OnDisable()
{
	__super::OnDisable();
	m_bHit = false;

	Safe_release(m_pActor);
	m_fLoopTimeAcc = 0.f;
	m_bUseOwnerUnitLook = false;
	if (m_pTrailEffect)
	{
		m_pTrailEffect->TurnOn_TrailEffect(false);
		m_pTrailEffect2->TurnOn_TrailEffect(false);
	}

}

void CProjectile::Hit_Unit(CGameObject* pHitUnit, _float4 vHitPos, _bool bHeadShot)
{
	//bHeadShot = true;

	m_pHitUnit = pHitUnit;
	On_ChangePhase(eSTICK);

	string strBoneName = "0B_Spine1";

	if (bHeadShot)
		strBoneName = "0B_Head";

	m_pCurStickBone = GET_COMPONENT_FROM(pHitUnit, CModel)->Find_HierarchyNode(strBoneName.c_str());

	if (!m_pCurStickBone)
		return;



	//1. 화살이 맞으면 일단 matInv를 곱해서 로컬로 들어가자
	_float4x4 matWorldInv = m_pCurStickBone->Get_BoneMatrix().Inverse();
	m_matHitOffset = m_pTransform->Get_WorldMatrix() * matWorldInv;


	//2. 화살 방향도 돌아가야함
	_float4 vDir = (Get_ArrowHeadPos() - vHitPos);
	vDir = vDir.MultiplyNormal(matWorldInv);

	//3. 화살의 로컬 매트릭스는 x랑 z는 지우고 dir 방향으로 밀기
	_float4 vHitLocalPos = ZERO_VECTOR;
	vHitLocalPos -= vDir.Normalize() * 100.f;

	

	memcpy(m_matHitOffset.m[3], &vHitLocalPos, sizeof(_float4));

	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Arrow_Blood", vHitPos);


} 

void CProjectile::Set_ColliderType(eTEAM_TYPE eTeamType)
{
	if (eTeamType == eTEAM_TYPE::eBLUE)
		m_pCollider->Set_ColIndex(COL_BLUEATTACK);
	else
		m_pCollider->Set_ColIndex(COL_REDATTACK);
}