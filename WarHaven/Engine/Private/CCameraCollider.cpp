#include "CCameraCollider.h"

#include "GameInstance.h"
#include "PhysX_Manager.h"

#include "GameObject.h"
#include "Transform.h"

#include "HIerarchyNode.h"
#include "Physics.h"
#include "CUtility_PhysX.h"

CCameraCollider::CCameraCollider(_uint iGroupID)
	: CPhysXCharacter(iGroupID)
{
}

CCameraCollider::~CCameraCollider()
{
}

CCameraCollider* CCameraCollider::Create(_uint iGroupID, const BONECOLLIDERDESC& tBoneColliderDesc)
{
	CCameraCollider* pInstance = new CCameraCollider(iGroupID);

	pInstance->m_tColliderDesc = tBoneColliderDesc;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Created : CCameraCollider");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CCameraCollider::onShapeHit(const PxControllerShapeHit& hit)
{
	if (hit.actor->getType() == PxActorType::eRIGID_STATIC)
	{
		//userdata°¡ ÀÖÀ¸¸é ±×³É º®Àº ¾Æ´Ô
		if (hit.actor->userData)
			return;

		m_pCurrentHitActor = hit.actor;
		m_pCurrentShape= hit.shape;
	}
}

void CCameraCollider::onControllerHit(const PxControllersHit& hit)
{
	
}


HRESULT CCameraCollider::Initialize_Prototype()
{
	CPhysX_Manager::Get_Instance()->Create_CapsuleController(m_tColliderDesc.fRadius, m_tColliderDesc.fHeight, &m_pPxController, this);

	if (!m_pPxController)
		return E_FAIL;


	return S_OK;
}

void CCameraCollider::Tick()
{


}

void CCameraCollider::Late_Tick()
{
	_float4 vPos = m_tColliderDesc.vOffset.MultiplyCoord(m_pOwner->Get_Transform()->Get_WorldMatrix());

	m_pPxController->setPosition(CUtility_PhysX::To_PxExtendedVec3(vPos));

	_float4 vMove = vPos - m_vPrevPos;
	m_pPxController->setPosition(CUtility_PhysX::To_PxExtendedVec3(m_vPrevPos));
	m_vPrevPos = vPos;
	m_pPxController->move(CUtility_PhysX::To_PxVec3(vMove), 0.f, fDT(0), m_tControllerFilters);

	//ÃÖ±Ù¿¡ ´êÀº ¾ÖÇåÆ¼ ray½î±â
	if (!m_pCurrentHitActor)
		return;
	if (!m_pCurrentShape)
		return;
	_float4 vOwnerPlayerPos = m_pOwner->Get_Parent()->Get_Transform()->Get_World(WORLD_POS);
	vOwnerPlayerPos.y += 1.f;

	//ray¸¦ ³»À§Ä¡·Î ½î´Â°Ô ¾î¤¿´Ï¶ó Å¸°Ù ÁöÁ¡À¸·Î ½÷¾ß´ï
	// Å¸°Ù ¹æÇâ : Ä«¸Þ¶óÀÇ look -
	_float4 vDir = m_pOwner->Get_Transform()->Get_World(WORLD_LOOK) * -1.f;

	//staticÀÌ¶û ´êÀ¸¸é ÀÌ³à¼®ÇÑÅ× ray ½÷¼­ À§Ä¡ º¸Á¤
	PxRaycastHit hitInfo;

	PxU32 hitCount = PxGeometryQuery::raycast(
		CUtility_PhysX::To_PxVec3(vOwnerPlayerPos),
		CUtility_PhysX::To_PxVec3(vDir),
		m_pCurrentShape->getGeometry().any(),
		m_pCurrentHitActor->getGlobalPose(),
		3.f, PxHitFlag::ePOSITION, 1, &hitInfo
	);

	if (hitCount > 0)
	{
		m_pPxController->setPosition(PxExtendedVec3(hitInfo.position.x, hitInfo.position.y, hitInfo.position.z));
		_float4 vHitPos = CUtility_PhysX::To_Vector(hitInfo.position);
		_float4 vHitNormal = CUtility_PhysX::To_Vector(hitInfo.normal);
		vHitPos += vHitNormal;
		m_pOwner->Get_Transform()->Set_World(WORLD_POS, vHitPos);
		m_pOwner->Get_Transform()->Make_WorldMatrix();
		m_vPrevPos = CUtility_PhysX::To_Vector(hitInfo.position);

	}


}

void CCameraCollider::OnEnable()
{
	__super::OnEnable();

	if (m_bStart)
	{
		_float4 vPos = m_tColliderDesc.vOffset.MultiplyCoord(m_pOwner->Get_Transform()->Get_WorldMatrix());

		m_pPxController->setPosition(CUtility_PhysX::To_PxExtendedVec3(vPos));
	}
	m_bStart = true;

}

void CCameraCollider::OnDisable()
{
	__super::OnDisable();
	m_pPxController->setPosition(PxExtendedVec3(-999.f, -999.f, -999.f));
}
