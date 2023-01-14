#include "stdafx.h"
#include "CCannonBall.h"

#include "UsefulHeaders.h"

#include "CUtility_PhysX.h"
#include "CColorController.h"

#include "CCannonBoom.h"
#include "CCollider_Sphere.h"

#include "HIerarchyNode.h"

#include "CTeamConnector.h"

CCannonBall::CCannonBall()
{
}

CCannonBall::~CCannonBall()
{
}

void CCannonBall::Shoot_Cannon(CPlayer* pOwnerPlayer, _float4 vShootPos, _float4 vShootDir)
{
	m_pOwnerPlayer = pOwnerPlayer;
	ENABLE_GAMEOBJECT(this);

	/* PhysX */
	PxTransform tTransform;
	ZeroMemory(&tTransform, sizeof(PxTransform));

	_float4 vCurPos = vShootPos;

	tTransform.p = CUtility_PhysX::To_PxVec3(vCurPos);
	tTransform.q = CUtility_PhysX::To_PxQuat(m_pTransform->Get_Quaternion());

	PxRigidDynamic* pActor = nullptr;
	pActor = GAMEINSTANCE->Create_DynamicActor(tTransform, PxConvexMeshGeometry(m_pConvexMesh), CPhysX_Manager::SCENE_CURRENT, 25.f);
	_float4 vDir = vShootDir.Normalize();
	vDir *= m_fForcePower;
	pActor->addForce(CUtility_PhysX::To_PxVec3(vDir));
	//pActor->addTorque(CUtility_PhysX::To_PxVec3(vDir));
	m_pActor = pActor;

	if (m_pOwnerPlayer->Get_Team()->Get_TeamType() == eTEAM_TYPE::eBLUE)
		m_pCollider->Set_ColIndex(COL_BLUEFLYATTACKGUARDBREAK);
	else
		m_pCollider->Set_ColIndex(COL_REDFLYATTACKGUARDBREAK);


}

void CCannonBall::Shoot_CatchedCannon(_float4 vShootDir)
{
	m_bCatched = false;

	/* PhysX */
	PxTransform tTransform;
	ZeroMemory(&tTransform, sizeof(PxTransform));

	_float4 vCurPos = m_pTransform->Get_World(WORLD_POS);
	//memcpy(&vCurPos, m_pTargetBone->Get_BoneMatrix().m[3], sizeof(_float4));
	
	tTransform.p = CUtility_PhysX::To_PxVec3(vCurPos);
	tTransform.q = CUtility_PhysX::To_PxQuat(m_pTransform->Get_Quaternion());

	PxRigidDynamic* pActor = nullptr;
	pActor = GAMEINSTANCE->Create_DynamicActor(tTransform, PxConvexMeshGeometry(m_pConvexMesh), CPhysX_Manager::SCENE_CURRENT, 25.f);
	_float4 vDir = vShootDir.Normalize();
	vDir *= m_fForcePower;
	pActor->addForce(CUtility_PhysX::To_PxVec3(vDir));
	//pActor->addTorque(CUtility_PhysX::To_PxVec3(vDir));
	m_pActor = pActor;
}

void CCannonBall::Projectile_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos)
{
	if (COL_REDPROJECTILECATCH == eOtherColType || COL_BLUEPROJECTILECATCH == eOtherColType)
	{
		if (m_bCatched)
			return;

		CUnit* pOtherUnit = dynamic_cast<CUnit*>(pOtherObj);

		m_pOwnerPlayer = pOtherUnit->Get_OwnerPlayer();

		DISABLE_COMPONENT(m_pCollider);

		pOtherUnit->Catch_CannonBall(this);

		m_pTargetBone = GET_COMPONENT_FROM(pOtherUnit, CModel)->Find_HierarchyNode("0B_L_WP1");
		m_bCatched = true;

		Safe_release(m_pActor);

	}
}

void CCannonBall::Projectile_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
}

void CCannonBall::Projectile_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
}

HRESULT CCannonBall::Initialize_Prototype()
{
	CModel* pModel = CModel::Create(CP_BEFORE_RENDERER, TYPE_NONANIM, L"../bin/resources/meshes/weapons/ball/SK_CannonBall.fbx",
		XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(270.f)) * XMMatrixRotationX(XMConvertToRadians(90.0f))
	);

	if (!pModel)
		return E_FAIL;

	pModel->Initialize();
	Add_Component(pModel);
	pModel->Set_ShaderFlag(SH_LIGHT_BLOOM);
	pModel->Set_ShaderPassToAll(VTXMODEL_PASS_NORMALMAPPING);




	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXMODEL,
		VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXMODEL_PASS_NORMALMAPPING
		, _float4(0.f, 0.f, 0.f, 1.f));

	Add_Component<CRenderer>(pRenderer);

	CCollider_Sphere* pCollider = CCollider_Sphere::Create(CP_AFTER_TRANSFORM, 0.5f, COL_BLUEATTACK, ZERO_VECTOR, DEFAULT_TRANS_MATRIX);
	Add_Component(pCollider);


	/* PhysX */
	CMeshContainer* pMesh = (pModel->Get_MeshContainers().back().second);

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

	CColorController* pCController = CColorController::Create(CP_BEFORE_RENDERER);

	if (!pCController)
		return E_FAIL;

	Add_Component(pCController);

	m_pCannonBoom = CCannonBoom::Create();
	if (!m_pCannonBoom)
		return E_FAIL;
	m_pCannonBoom->Initialize();

	return S_OK;
}

HRESULT CCannonBall::Initialize()
{
	return S_OK;
}

HRESULT CCannonBall::Start()
{
	__super::Start();

	m_pCollider = GET_COMPONENT(CCollider_Sphere);

	if (m_pCannonBoom)
	{
		CREATE_GAMEOBJECT(m_pCannonBoom, GROUP_EFFECT);
		DISABLE_GAMEOBJECT(m_pCannonBoom);
	}
	

	CColorController::COLORDESC tColorDesc;
	ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

	tColorDesc.eFadeStyle = CColorController::TIME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.1f;
	tColorDesc.fFadeOutStartTime = 9999.f;
	tColorDesc.fFadeOutTime = 0.1f;
	tColorDesc.vTargetColor = _float4((255.f / 255.f), (42.f / 255.f), (42.f / 255.f), 0.1f);
	//tColorDesc.vTargetColor *= 1.1f;
	tColorDesc.iMeshPartType = MODEL_PART_SKEL;

	GET_COMPONENT(CColorController)->Add_ColorControll(tColorDesc);

	CallBack_CollisionEnter += bind(&CCannonBall::Projectile_CollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
	CallBack_CollisionStay += bind(&CCannonBall::Projectile_CollisionStay, this, placeholders::_1, placeholders::_2, placeholders::_3);
	CallBack_CollisionExit += bind(&CCannonBall::Projectile_CollisionExit, this, placeholders::_1, placeholders::_2, placeholders::_3);


	return S_OK;
}

void CCannonBall::My_Tick()
{
	if (!m_bCatched)
		return;

	if (!m_pTargetBone)
		return;

	_float4 vTargetPos;
	memcpy(&vTargetPos, m_pTargetBone->Get_BoneMatrix().m[3], sizeof(_float4));

	vTargetPos += m_pOwnerPlayer->Get_CurrentUnit()-> Get_Transform()->Get_World(WORLD_LOOK);

	_float4 vCurPos = m_pTransform->Get_World(WORLD_POS);
	
	_float4 vDir = vTargetPos - vCurPos;
	_float fLength = vDir.Length();

	_float fSpeed = 4.f;

	vCurPos += vDir * fSpeed * fDT(0);
	
	m_pTransform->Set_World(WORLD_POS, vCurPos);


}

void CCannonBall::My_LateTick()
{
	if (m_bCatched)
		return;

	/* PhysX 따라가기 */
	if (!m_pActor)
		return;

	PxTransform tTransform = m_pActor->getGlobalPose();
	_float4x4 matPhysX = CUtility_PhysX::To_Matrix(tTransform);
	m_pTransform->Get_Transform().matMyWorld = matPhysX;

	m_pTransform->Make_WorldMatrix();

	_float fPower = CUtility_PhysX::To_Vector(m_pActor->getLinearVelocity()).Length() * fDT(0);

	if (fPower < 15.f * fDT(0))
		On_Boom();
}

void CCannonBall::OnEnable()
{
	__super::OnEnable();
	m_bCatched = false;
	
	Create_Light(this, ZERO_VECTOR, 20.f, 0.f, 0.1f, 10.f, 0.1f, RGB(255, 0, 0), true);
}

void CCannonBall::OnDisable()
{
	__super::OnDisable();
	Safe_release(m_pActor);

}

void CCannonBall::Create_Light(CGameObject* pOwner, _float4 vOffset, _float fRange, _float fRandomRange, _float fFadeInTime, _float fDuration, _float fFadeOutTime, _float4 Diffuse, _bool bLoop)
{
	LIGHTDESC			LightDesc;

	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.eFadeType = tagLightDesc::FADEIN;

	LightDesc.pOwner = pOwner;
	LightDesc.vOffset = vOffset;
	LightDesc.fRange = fRange;
	LightDesc.fRandomRange = fRandomRange;

	LightDesc.fLightFadeInTime = fFadeInTime;
	LightDesc.fLightTime = fDuration;
	LightDesc.fLightFadeOutTime = fFadeOutTime;

	LightDesc.vTargetDiffuse = Diffuse;
	LightDesc.vTargetAmbient = _float4(0.2f, 0.2f, 0.2f);
	LightDesc.vTargetSpecular = _float4(1.f, 1.f, 1.f);

	LightDesc.eInEasingType = tagLightDesc::EAS_Linear;
	LightDesc.eOutEasingType = tagLightDesc::EAS_Linear;

	LightDesc.bLoop = bLoop;

	GAMEINSTANCE->Add_Light(LightDesc);
}

void CCannonBall::On_Boom()
{
	DISABLE_GAMEOBJECT(this);
	if (m_pCannonBoom)
		m_pCannonBoom->Boom(m_pOwnerPlayer, m_pTransform->Get_World(WORLD_POS));

}
