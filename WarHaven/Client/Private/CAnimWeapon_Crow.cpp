#include "stdafx.h"
#include "CAnimWeapon_Crow.h"

#include "UsefulHeaders.h"
#include "CUtility_PhysX.h"

#include "HIerarchyNode.h"
#include "CColorController.h"
#include "CCollider_Sphere.h"
#include "CTeamConnector.h"

#include "CCrowBoom.h"
#include "CColorController.h"
#include "CBoneCollider.h"
#include "CUnit_Qanda.h"

CAnimWeapon_Crow::CAnimWeapon_Crow()
{
}

CAnimWeapon_Crow::~CAnimWeapon_Crow()
{
}

CAnimWeapon_Crow* CAnimWeapon_Crow::Create(wstring wstrModelFilePath, wstring wstrAnimFilePath, CUnit* pOwnerUnit, string strBoneName, 
	_float fRadianX, _float fRadianY, _float fRadianZ)
{
	CAnimWeapon_Crow* pInstance = new CAnimWeapon_Crow;

	pInstance->m_pOwnerUnit = pOwnerUnit;

	if (FAILED(pInstance->SetUp_Model(wstrModelFilePath, wstrAnimFilePath, strBoneName, fRadianX, fRadianY, fRadianZ)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_Model : CAnimWeapon_Crow");
		return nullptr;
	}
	
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CAnimWeapon_Crow");
		return nullptr;
	}

	return pInstance;
}

void CAnimWeapon_Crow::Crow_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos)
{
	if (m_eCurPhase == eSHOOT)
	{

		switch (eMyColType)
		{
		case COL_BLUEGROGGYATTACK:

			if (eOtherColType == COL_REDHITBOX_BODY ||
				eOtherColType == COL_REDGUARD)
				Boom_Crow();



			break;

		case COL_REDGROGGYATTACK:

			if (eOtherColType == COL_BLUEHITBOX_BODY ||
				eOtherColType == COL_BLUEGUARD)
				Boom_Crow();

			

			break;

		default:
			break;
		}

		
		
		
	}


}
void CAnimWeapon_Crow::Crow_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
	m_pOwnerUnit->CallBack_CollisionStay(pOtherObj, eOtherColType, eMyColType);
	pOtherObj->CallBack_CollisionStay(m_pOwnerUnit, eMyColType, eOtherColType);
}
void CAnimWeapon_Crow::Crow_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
	CUnit* pOtherUnit = nullptr;
	pOtherUnit = dynamic_cast<CUnit*>(pOtherObj);

	if (!pOtherUnit)
		return;

	pOtherUnit->CallBack_CollisionExit(pOtherObj, eOtherColType, eMyColType);
	pOtherObj->CallBack_CollisionExit(pOtherUnit, eMyColType, eOtherColType);
}

void CAnimWeapon_Crow::Boom_Crow()
{
	m_pCrowBoom->Boom(m_pOwnerUnit->Get_OwnerPlayer(), m_pTransform->Get_World(WORLD_POS));
	DISABLE_COMPONENT(GET_COMPONENT(CRenderer));
	On_ChangePhase(eHIT);
	ChangeColor_End();

	CUnit_Qanda* pQuanda = static_cast<CUnit_Qanda*>(m_pOwnerUnit);
	//pQuanda->Turn_FeatherEffect(true);
	pQuanda->Turn_SteamEffect(false);

	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Crow_Boom", m_pTransform->Get_World(WORLD_POS),
		GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_WorldMatrix(MARTIX_NOTRANS | MATRIX_NOSCALE));

	CFunctor::Play_Sound(L"Effect_Crow_Boom0", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS), 1.f);
	CFunctor::Play_Sound(L"Effect_Crow_Boom1", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS), 1.f);
	CFunctor::Play_Sound(L"Effect_Crow_Boom2", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS), 1.f);
}


void CAnimWeapon_Crow::ChangeColor_Charge()
{
	CColorController::COLORDESC tColorDesc;
	ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

	tColorDesc.eFadeStyle = CColorController::TIME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.1f;
	tColorDesc.fFadeOutStartTime = 9999.f;
	tColorDesc.fFadeOutTime = 3.f;
	tColorDesc.vTargetColor = RGBA(196, 13, 0, 1);
	tColorDesc.iMeshPartType = 1;
	GET_COMPONENT(CColorController)->Add_ColorControll(tColorDesc);
}

void CAnimWeapon_Crow::ChangeColor_Shoot()
{
	CColorController::COLORDESC tColorDesc;
	ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

	tColorDesc.eFadeStyle = CColorController::TIME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.1f;
	tColorDesc.fFadeOutStartTime = 9999.f;
	tColorDesc.fFadeOutTime = 0.1f;
	tColorDesc.vTargetColor = RGBA(255, 13, 0, 1);
	GET_COMPONENT(CColorController)->Add_ColorControll(tColorDesc);
}

void CAnimWeapon_Crow::ChangeColor_End()
{
	CColorController::COLORDESC tColorDesc;
	ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

	tColorDesc.eFadeStyle = CColorController::TIME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.0f;
	tColorDesc.fFadeOutStartTime = 0.f;
	tColorDesc.fFadeOutTime = 0.01f;
	tColorDesc.vTargetColor = RGB(0, 0, 0);
	tColorDesc.iMeshPartType = 1;
	GET_COMPONENT(CColorController)->Add_ColorControll(tColorDesc);
}


void CAnimWeapon_Crow::On_ChangePhase(ePhyxState eNextPhase)
{
	m_fLoopTimeAcc = 0.f;
	m_eCurPhase = eNextPhase;
}

void CAnimWeapon_Crow::Shoot_Crow(_float4 vShootPos, _float4 vShootDir)
{

	_float4 vCurPos = m_pTransform->Get_World(WORLD_POS);


	/* PhysX */
	//PxTransform tTransform;
	//ZeroMemory(&tTransform, sizeof(PxTransform));


	//tTransform.p = CUtility_PhysX::To_PxVec3(vCurPos);
	//tTransform.q = CUtility_PhysX::To_PxQuat(m_pTransform->Get_Quaternion());

	//PxRigidDynamic* pActor = nullptr;
	//pActor = GAMEINSTANCE->Create_DynamicActor(tTransform, PxConvexMeshGeometry(m_pConvexMesh), CPhysX_Manager::SCENE_CURRENT, 1.5f);

	//_float4 vDir = ZERO_VECTOR;

	//vDir = m_pTransform->Get_World(WORLD_RIGHT);
	//vDir *= m_fMaxSpeed;
	//pActor->addForce(CUtility_PhysX::To_PxVec3(vDir));
	//m_pActor = pActor;

	///* PhysX */
	//PxTransform tTransform;
	//ZeroMemory(&tTransform, sizeof(PxTransform));

	//_float4 vCurPos = vShootPos;
	//vCurPos += m_pTransform->Get_World(WORLD_LOOK) * 2.5f;

	//tTransform.p = CUtility_PhysX::To_PxVec3(vCurPos);
	//tTransform.q = CUtility_PhysX::To_PxQuat(m_pTransform->Get_Quaternion());

	//PxRigidDynamic* pActor = nullptr;
	//pActor = GAMEINSTANCE->Create_DynamicActor(tTransform, PxConvexMeshGeometry(m_pConvexMesh), CPhysX_Manager::SCENE_CURRENT, 10.f);
	//_float4 vDir = vShootDir.Normalize();
	//vDir *= 1.f;
	//pActor->addForce(CUtility_PhysX::To_PxVec3(vDir));
	////pActor->addTorque(CUtility_PhysX::To_PxVec3(vDir));
	//m_pActor = pActor;

	ENABLE_COMPONENT(m_pBoneColider);

	m_pCollider = GET_COMPONENT(CCollider_Sphere);

	if (!m_pOwnerUnit->Get_OwnerPlayer()->Get_Team())
		m_pCollider->Set_ColIndex(COL_REDFLYATTACKGUARDBREAK);
	else
	{
		eTEAM_TYPE eTeam = m_pOwnerUnit->Get_OwnerPlayer()->Get_Team()->Get_TeamType();


		if (eTeam == eTEAM_TYPE::eBLUE)
			m_pCollider->Set_ColIndex(COL_BLUEGROGGYATTACK);

		else if(eTeam == eTEAM_TYPE::eRED)
			m_pCollider->Set_ColIndex(COL_REDGROGGYATTACK);
		
	}


	m_eCurPhase = eSHOOT;
	m_vStartPosition = vCurPos;
	Set_AnimIndex(19, 0.1f, 2.f);

	m_pPhysics->Set_MaxSpeed(20.f);

	if (m_pOwnerUnit->Get_CurState() == AI_STATE_COMBAT_SHOOT_QANDA)
	{
		vShootDir.y -= 0.2f;
		m_vChaseLook = vShootDir;
	}
	else
	{
		m_vChaseLook = m_pOwnerUnit->Get_FollowCamLook();
		m_vChaseRight = m_pOwnerUnit->Get_FollowCamRight();
	}

	



}

void CAnimWeapon_Crow::Set_AnimIndex(_uint iAnimIndex, _float fInterpolateTime, _float fAnimSpeed)
{
	m_pAnimator->Set_CurAnimIndex(0, iAnimIndex);
	m_pAnimator->Set_InterpolationTime(0, iAnimIndex, fInterpolateTime);
	m_pAnimator->Set_AnimSpeed(0, iAnimIndex, fAnimSpeed);
}

void CAnimWeapon_Crow::Set_PhiysicsSpeed(_float fMaxSpeed)
{
	m_pPhysics->Set_MaxSpeed(fMaxSpeed);
}

HRESULT CAnimWeapon_Crow::Initialize_Prototype()
{
	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXANIMMODEL,
		VTXANIM_DECLARATION::Element, VTXANIM_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXANIM_PASS_NORMAL
		, _float4(0.f, 0.f, 0.f, 1.f));
	pRenderer->Initialize();
	Add_Component<CRenderer>(pRenderer);

	Add_Component(CPhysics::Create(0));

	m_pCrowBoom = CCrowBoom::Create();
	if (!m_pCrowBoom)
		return E_FAIL;
	m_pCrowBoom->Initialize();

	_float fRadius = 1.f;
	_float4 vOffsetPos = ZERO_VECTOR;
	//vOffsetPos.z += fRadius;
	//vOffsetPos.z += fRadius;
	//vOffsetPos.z += fRadius;
	
	// ?? ???? ????
	CCollider_Sphere* pCollider = CCollider_Sphere::Create(CP_AFTER_TRANSFORM, fRadius, COL_REDFLYATTACKGUARDBREAK, vOffsetPos, DEFAULT_TRANS_MATRIX);
	vOffsetPos.x += fRadius;

	Add_Component(pCollider);

	m_pCollider = pCollider;

	if (!m_pCollider)
		return E_FAIL;

	CColorController* pCController = CColorController::Create(CP_BEFORE_RENDERER);

	if (!pCController)
		return E_FAIL;

	Add_Component(pCController);

	CBoneCollider::BONECOLLIDERDESC tDesc;
	// ?? ????
	tDesc.fHeight = 0.3f;
	// ?? ????
	tDesc.fRadius = 0.3f;
	// ?? ???? ??
	tDesc.pRefBone = GET_COMPONENT(CModel)->Find_HierarchyNode("1B_COM");

	//?? ??????(????)
	tDesc.vOffset = _float4(0.f, 0.f, 0.f);

	m_pBoneColider = CBoneCollider::Create(CP_RIGHTBEFORE_RENDERER, tDesc);
	Add_Component(m_pBoneColider);


    return S_OK;
}

HRESULT CAnimWeapon_Crow::Initialize()
{
	XMStoreFloat4x4(&m_OwnerBoneOffsetMatrix, XMMatrixIdentity());
	m_OwnerBoneOffsetMatrix.m[3][0] = -0.5f;
	m_OwnerBoneOffsetMatrix.m[3][1] = 0.1f;
	m_OwnerBoneOffsetMatrix.m[3][2] = 0.5f;

	m_pPhysics = GET_COMPONENT(CPhysics);
	
	if (!m_pPhysics)
		return E_FAIL;

	m_pPhysics->Set_MaxSpeed(m_pOwnerUnit->Get_Status().fSprintSpeed);

	//__super::Initialize();

    return S_OK;
}

HRESULT CAnimWeapon_Crow::Start()
{
	__super::Start();

	if (m_pCrowBoom)
	{
		CREATE_GAMEOBJECT(m_pCrowBoom, GROUP_EFFECT);
		DISABLE_GAMEOBJECT(m_pCrowBoom);
	}

	CallBack_CollisionEnter += bind(&CAnimWeapon_Crow::Crow_CollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
	CallBack_CollisionStay += bind(&CAnimWeapon_Crow::Crow_CollisionStay, this, placeholders::_1, placeholders::_2, placeholders::_3);
	CallBack_CollisionExit += bind(&CAnimWeapon_Crow::Crow_CollisionExit, this, placeholders::_1, placeholders::_2, placeholders::_3);

	Set_AnimIndex(10, 0.1f, 1.f);

    return S_OK;
}

void CAnimWeapon_Crow::OnEnable()
{
	__super::OnEnable();
	m_eCurPhase = eIDLE;
	m_pTransform->Set_World(WORLD_POS, m_pOwnerUnit->Get_Transform()->Get_World(WORLD_POS));
}

void CAnimWeapon_Crow::OnDisable()
{
	__super::OnDisable();
	DISABLE_COMPONENT(m_pBoneColider);
	Safe_release(m_pActor);
}

HRESULT CAnimWeapon_Crow::SetUp_Model(wstring wstrModelFilePath, wstring wstrAnimFilePath, string strBoneName, _float fRadianX, _float fRadianY, _float fRadianZ)
{
	CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, wstrAnimFilePath.c_str());
	pAnimator->Initialize();

	if (!pAnimator)
		return E_FAIL;

	Add_Component(pAnimator);
	m_pAnimator = pAnimator;

	_float4x4 matIdentity;
	matIdentity.Identity();

	CModel* pModel = CModel::Create(CP_BEFORE_RENDERER, TYPE_ANIM, wstrModelFilePath,
		XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(fRadianZ)) * XMMatrixRotationY(XMConvertToRadians(fRadianY)) * XMMatrixRotationX(XMConvertToRadians(fRadianX))
	);
	pModel->Add_Model(wstrModelFilePath, 1);
	pModel->Initialize();
	Add_Component(pModel);
	pModel->Set_ShaderFlag(SH_LIGHT_BLOOM);
	pModel->Set_ShaderPassToAll(VTXANIM_PASS_NORMAL);
	

	m_pOwnerBone = GET_COMPONENT_FROM(m_pOwnerUnit, CModel)->Find_HierarchyNode(strBoneName.c_str());
	if (!m_pOwnerBone)
		return E_FAIL;

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

void CAnimWeapon_Crow::Follow_Owner()
{
	if (m_pOwnerBone)
	{
		_float4 vPos = m_pOwnerUnit->Get_Transform()->Get_World(WORLD_POS);
		_float4 vCurPos = m_pTransform->Get_World(WORLD_POS);
		vCurPos.x += 0.5f;
		vCurPos.y -= 1.5f;

		_float4 vDir = vPos - vCurPos;

		//???? ???????? ????
		_float fRealDistance = vDir.Length();

		_float fRatio = (fRealDistance / m_fMaxDistance * 3.f);
		fRatio = powf(fRatio, 2.f);

		_float fSpeed = m_fMaxSpeed * fRatio;


		//???? ???? ????
		if (fSpeed > 6.f)
			fSpeed = 6.f;


		if (fabs(vDir.Length()) < 0.15f)
		{
			m_pTransform->Set_LerpLook(m_pOwnerUnit->Get_Transform()->Get_World(WORLD_LOOK), 0.4f);
			return;
		}
		else
			m_pTransform->Set_LerpLook(vDir * -1.f, 0.4f);



		m_pPhysics->Set_Dir(vDir);
		m_pPhysics->Set_Accel(fSpeed);
	}
}

void CAnimWeapon_Crow::Late_Tick()
{

	switch (m_eCurPhase)
	{
	case Client::CAnimWeapon_Crow::eIDLE:
	{
		//if (m_pOwnerBone)
		//{
		//	_float4x4		matBone = m_pOwnerBone->Get_BoneMatrix();

		//	m_pTransform->Get_Transform().matMyWorld = matBone * m_OwnerBoneOffsetMatrix;
			
		//	m_pTransform->Make_WorldMatrix();
		//}

		if (m_pAnimator->Get_CurAnimIndex() == 13 && m_pAnimator->Is_CurAnimFinished())
			Set_AnimIndex(10, 0.05f, 2.f);

		Follow_Owner();

		


	}
		break;

	case Client::CAnimWeapon_Crow::eATTACKLOOP:
	{
		_float4 vOwnerPos = m_pOwnerUnit->Get_Transform()->Get_World(WORLD_POS);
		vOwnerPos.y += 1.8f;


		_float4 vLook = vOwnerPos - m_pTransform->Get_World(WORLD_POS);

		m_pTransform->Set_LerpLook(m_pOwnerUnit->Get_FollowCamLook() * -1.f, 0.1f);
		m_pPhysics->Set_Dir(vLook);

		if (fabs(vLook.Length()) < 0.1f)
		{
			m_pTransform->Set_World(WORLD_POS, vOwnerPos);
			break;
		}

		m_pPhysics->Set_Accel(5.f);
	}
		break;
	case Client::CAnimWeapon_Crow::eSHOOT:
	{
		m_pPhysics->Set_Dir(m_vChaseLook);
		m_pPhysics->Set_Accel(100.f);

		m_fLoopTimeAcc += fDT(0);



		if (m_fLoopTimeAcc > m_fMaxShootTime ||
			m_pBoneColider->Is_Collision())
		{
			Safe_release(m_pActor);
			Boom_Crow();

		}


	}
	break;
	case Client::CAnimWeapon_Crow::eHIT:
		/* ???? ???????? ?? */
	{
		/* PhysX ???????? */
		//PxTransform tTransform = m_pActor->getGlobalPose();
		//_float4x4 matPhysX = CUtility_PhysX::To_Matrix(tTransform);
		//m_pTransform->Get_Transform().matMyWorld = matPhysX;

//		m_pTransform->Make_WorldMatrix();

		m_fLoopTimeAcc += fDT(0);
		if (m_fLoopTimeAcc >= m_fMaxLoopTime)
		{

			DISABLE_COMPONENT(m_pBoneColider);
			ENABLE_COMPONENT(GET_COMPONENT(CRenderer));
			Set_AnimIndex(13, 0.f, 2.f);
			On_ChangePhase(eSPAWN);
			m_pPhysics->Set_MaxSpeed(10.f);

			_float4 vPos = m_pOwnerUnit->Get_Transform()->Get_World(WORLD_POS);
			vPos.x -= 0.5f;
			vPos.y += 1.5f;

			m_pTransform->Set_World(WORLD_POS, vPos);
			m_pTransform->Make_WorldMatrix();

			m_fLoopTimeAcc = 0.f;


		}
	}
		break;

	case Client::CAnimWeapon_Crow::eSPAWN:

		
		static_cast<CUnit_Qanda*>(m_pOwnerUnit)->Turn_FeatherEffect(true);
		Follow_Owner();

		if (m_pAnimator->Is_CurAnimFinished())
		{
			Set_AnimIndex(10, 0.f, 2.f);
			On_ChangePhase(eIDLE);
		}
		break;

	case Client::CAnimWeapon_Crow::eEND:
		break;
	default:
		break;
	}
}

