#include "stdafx.h"
#include "CProjectile.h"

#include "UsefulHeaders.h"

#include "HIerarchyNode.h"
#include "CCollider_Sphere.h"

#include "CColorController.h"

#include "CUnit_Archer.h"

CProjectile::CProjectile()
{
}

CProjectile::~CProjectile()
{
}

void CProjectile::Projectile_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos)
{
}

void CProjectile::Projectile_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
}

void CProjectile::Projectile_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
}

void CProjectile::Reset(CGameObject* pGameObject)
{
	m_pOwnerUnit = static_cast<CUnit*>(pGameObject);

	m_pLeftHandBone = GET_COMPONENT_FROM(m_pOwnerUnit, CModel)->Find_HierarchyNode("0B_L_WP1");
	m_pRightHandBone = GET_COMPONENT_FROM(m_pOwnerUnit, CModel)->Find_HierarchyNode("0B_R_WP1");

	if (!m_pLeftHandBone)
		assert(0);

	if (!m_pRightHandBone)
		assert(0);

	GET_COMPONENT(CPhysics)->Set_Speed(0.f);
	On_ChangePhase(eSTART);
	
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

	CPhysics* pPhysics = CPhysics::Create(CP_BEFORE_TRANSFORM);
	pPhysics->Set_MaxSpeed(m_fMaxSpeed);
	Add_Component(pPhysics);

	CColorController* pCController = CColorController::Create(CP_BEFORE_RENDERER);

	if (!pCController)
		return E_FAIL;

	Add_Component(pCController);
	
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


	_float4 vLook = m_pTransform->Get_World(WORLD_LOOK);
	GET_COMPONENT(CPhysics)->Set_Dir(vLook);
	GET_COMPONENT(CPhysics)->Set_SpeedasMax();
	ENABLE_COMPONENT(GET_COMPONENT(CCollider_Sphere));

	On_ChangePhase(eSHOOT);

	
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
		m_pCurStickBone = nullptr;
		break;
	case Client::CProjectile::eHIT:
		DISABLE_COMPONENT(GET_COMPONENT(CCollider_Sphere));
		break;
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

	CModel* pModel = CModel::Create(CP_BEFORE_RENDERER, TYPE_NONANIM, wstrModelFilePath,
		XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(0.0f)) * XMMatrixRotationX(XMConvertToRadians(0.0f))
	);

	if (!pModel)
		return E_FAIL;

	pModel->Initialize();
	Add_Component(pModel);
	pModel->Set_ShaderFlag(SH_LIGHT_BLOOM);
	pModel->Set_ShaderPassToAll(VTXMODEL_PASS_NORMALMAPPING);

	
	return S_OK;

}

HRESULT CProjectile::SetUp_Colliders(COL_GROUP_CLIENT eColType)
{
	_float fRadius = 0.1f;
	_float4 vOffsetPos = ZERO_VECTOR;
	CCollider_Sphere* pCollider = CCollider_Sphere::Create(CP_AFTER_TRANSFORM, fRadius, eColType, vOffsetPos, DEFAULT_TRANS_MATRIX);
	vOffsetPos.z += fRadius;
	pCollider->Add_Collider(fRadius, vOffsetPos);
	vOffsetPos.z += fRadius;
	pCollider->Add_Collider(fRadius, vOffsetPos);
	vOffsetPos.z += fRadius;
	pCollider->Add_Collider(fRadius, vOffsetPos);
	Add_Component(pCollider);


	return S_OK;
}

void CProjectile::My_Tick()
{
}

void CProjectile::My_LateTick()
{

	/* »À¿¡ ºÙÀÌ±Í */
	if (m_pCurStickBone)
	{
		_float4x4		matBone = m_pCurStickBone->Get_BoneMatrix();

		m_pTransform->Get_Transform().matMyWorld = matBone;

		m_pTransform->Make_WorldMatrix();
	}
	
}
