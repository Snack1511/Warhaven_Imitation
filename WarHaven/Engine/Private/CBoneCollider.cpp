#include "CBoneCollider.h"

#include "GameInstance.h"
#include "PhysX_Manager.h"

#include "GameObject.h"
#include "Transform.h"

#include "HIerarchyNode.h"
#include "Physics.h"
#include "CUtility_PhysX.h"

CBoneCollider::CBoneCollider(_uint iGroupID)
	: CPhysXCharacter(iGroupID)
{
}

CBoneCollider::~CBoneCollider()
{
}

CBoneCollider* CBoneCollider::Create(_uint iGroupID, const BONECOLLIDERDESC& tBoneColliderDesc)
{
	CBoneCollider* pInstance = new CBoneCollider(iGroupID);

	pInstance->m_tColliderDesc = tBoneColliderDesc;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Created : CBoneCollider");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CBoneCollider::onShapeHit(const PxControllerShapeHit& hit)
{
	//CPhysXCharacter::onShapeHit(hit);

	if (PxActorType::eRIGID_DYNAMIC == hit.actor->getType())
	{
		static_cast<PxRigidDynamic*>(hit.actor)->setMaxLinearVelocity(1.f);
		static_cast<PxRigidDynamic*>(hit.actor)->setMaxContactImpulse(1.f);

		return;

	}
	
	m_bCollisionTemp = true;
	m_vHitPos = _float4(hit.worldPos.x, hit.worldPos.y, hit.worldPos.z);
	


	_matrix matHit;


	_float4 vLook = _float4(hit.worldNormal.x, hit.worldNormal.y, hit.worldNormal.z, 0.f).Normalize();
	//vLook *= -1.f;


	matHit.r[2] = vLook.XMLoad();

	_float4 vUp = { 0.f, 1.f, 0.f };
	if ((vLook.y < 1.1f && vLook.y > 0.9f) ||
		(vLook.y > -1.1f && vLook.y < -0.9f)
		)
		vUp = _float4(0.f, 0.f, 1.f, 0.f);

	vUp.Normalize();
	_float4 vRight = vUp.Cross(vLook);
	matHit.r[0] = vRight.Normalize().XMLoad();

	vUp = vLook.Cross(vRight);
	matHit.r[1] = vUp.Normalize().XMLoad();
	matHit.r[3] = m_vHitPos.XMLoad();


	m_HitMatrix = matHit;
}

void CBoneCollider::onControllerHit(const PxControllersHit& hit)
{
	m_bCCTCollisionTemp = true;
	m_vHitPos = _float4(hit.worldPos.x, hit.worldPos.y, hit.worldPos.z);

	_matrix matHit;

	_float4 vLook = _float4(hit.worldNormal.x, hit.worldNormal.y, hit.worldNormal.z, 0.f).Normalize();
	//vLook *= -1.f;

	matHit.r[2] = vLook.XMLoad();

	_float4 vUp = { 0.f, 1.f, 0.f };
	if ((vLook.y < 1.1f && vLook.y > 0.9f) ||
		(vLook.y > -1.1f && vLook.y < -0.9f)
		)
		vUp = _float4(0.f, 0.f, 1.f, 0.f);

	vUp.Normalize();
	_float4 vRight = vUp.Cross(vLook);
	matHit.r[0] = vRight.Normalize().XMLoad();

	vUp = vLook.Cross(vRight);
	matHit.r[1] = vUp.Normalize().XMLoad();
	matHit.r[3] = m_vHitPos.XMLoad();

	m_HitMatrix = matHit;

}


HRESULT CBoneCollider::Initialize_Prototype()
{
	CPhysX_Manager::Get_Instance()->Create_CapsuleController(m_tColliderDesc.fRadius, m_tColliderDesc.fHeight, &m_pPxController, this);

	if (!m_pPxController)
		return E_FAIL;

	return S_OK;
}

void CBoneCollider::Tick()
{
	

}

void CBoneCollider::Late_Tick()
{
	//상태패턴쪽에서 얘한테 명령을 내리면
	//그때만 move를 살짝 호출해서 가져오기?

	//일단 그냥 붙여보자

	_float4x4 BoneMatrix;

	BoneMatrix = m_tColliderDesc.pRefBone->Get_BoneMatrix();

	_float4 vPos = m_tColliderDesc.vOffset.MultiplyCoord(BoneMatrix);
	

	//BoneMat의 Up으로 up 주고
	//이동값은 전프레임꺼에서 빼야함
	_float4 vUp = (*((_float4*)&BoneMatrix.m[WORLD_LOOK]));

	vUp *= -1.f;

	//_float4 vPos = (*((_float4*)&BoneMatrix.m[WORLD_POS]));

	_float4 vMove = vPos - m_vPrevPos;

	m_bCollisionTemp = false;
	m_bCCTCollisionTemp = false;
	m_pPxController->setUpDirection(CUtility_PhysX::To_PxVec3(vUp.Normalize()));

	m_pPxController->setPosition(CUtility_PhysX::To_PxExtendedVec3(m_vPrevPos));
	m_vPrevPos = vPos;

	m_pPxController->move(CUtility_PhysX::To_PxVec3(vMove), 0.f, fDT(0), m_tControllerFilters);

	if (m_bCollisionTemp)
	{
		m_bCollision = true;
	}
	else
		m_bCollision = false;



	if (m_bCCTCollisionTemp)
	{
		m_bCCTCollision = true;
	}
	else
		m_bCCTCollision = false;

}

void CBoneCollider::OnEnable()
{
	__super::OnEnable();


	if (m_bStart)
	{
		_float4x4 BoneMatrix;
		BoneMatrix = m_tColliderDesc.pRefBone->Get_BoneMatrix();
		_float4 vPos = m_tColliderDesc.vOffset.MultiplyCoord(BoneMatrix);
		m_pPxController->setPosition(CUtility_PhysX::To_PxExtendedVec3(vPos));
		m_vPrevPos = vPos;
	}
	m_bCollision = false;
	m_bStart = true;

}

void CBoneCollider::OnDisable()
{
	__super::OnDisable();
	m_pPxController->setPosition(PxExtendedVec3(-999.f, - 999.f, -999.f ));
}
