#include "stdafx.h"
#include "CPurpleArrow.h"

#include "UsefulHeaders.h"

#include "CUnit_Archer.h"
#include "CRectEffects.h"

#include "CColorController.h"

#include "CCollider_Sphere.h"

#include "CState.h"

CPurpleArrow::CPurpleArrow()
{
}
CPurpleArrow::~CPurpleArrow()
{
	//m_Test.clear();
}

CPurpleArrow* CPurpleArrow::Create()
{
	CPurpleArrow* pInstance = new CPurpleArrow;

	if (FAILED(pInstance->CPurpleArrow::Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CPurpleArrow");
		return nullptr;
	}

	return pInstance;
}

HRESULT CPurpleArrow::Start()
{
	__super::Start();

	m_Test = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"PoisonTest", this, m_pTransform->Get_World(WORLD_POS));

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

	return S_OK;
}

HRESULT CPurpleArrow::Initialize_Prototype()
{
    if (FAILED(SetUp_Projectile(L"../bin/resources/meshes/weapons/longbow/SM_Bolt_Poison.fbx")))
        return E_FAIL;

    if (FAILED(SetUp_Colliders(COL_BLUEFLYATTACKGUARDBREAK)))
        return E_FAIL;

	m_hcCode = HASHCODE(CPurpleArrow);
	m_vArrowHeadPos = _float4(1.2f, 0.f, 0.f);

	m_fMaxPoisonTime = 0.3f;
	m_fDamage = -5.f;

	m_fMaxSpeed = 40.f;
	m_fMaxDistance = 70.f;

    return CProjectile::Initialize_Prototype();
}


void CPurpleArrow::OnEnable()
{
	__super::OnEnable();

	if(m_Test.empty())
		m_Test = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"PoisonTest", this, m_pTransform->Get_World(WORLD_POS));

}


void CPurpleArrow::OnDisable()
{
	__super::OnDisable();

	for (auto& elem : m_Test)
	{
		static_cast<CRectEffects*>(elem)->Set_AllFadeOut();
	}

	m_Test.clear();
	m_iTickCnt = 0;
}

HRESULT CPurpleArrow::SetUp_Colliders(COL_GROUP_CLIENT eColType)
{
	_float fRadius = 0.25f;
	_float4 vOffsetPos = ZERO_VECTOR;

	CCollider_Sphere* pCollider = CCollider_Sphere::Create(CP_AFTER_TRANSFORM, fRadius, eColType, vOffsetPos, DEFAULT_TRANS_MATRIX);
	vOffsetPos.x += fRadius;
	pCollider->Add_Collider(fRadius, vOffsetPos);
	vOffsetPos.x += fRadius;
	pCollider->Add_Collider(fRadius, vOffsetPos);
	vOffsetPos.x += fRadius;
	pCollider->Add_Collider(fRadius, vOffsetPos);
	vOffsetPos.x += fRadius;
	pCollider->Add_Collider(fRadius * 2.f, vOffsetPos);
	vOffsetPos.x += fRadius;
	pCollider->Add_Collider(fRadius * 2.f, vOffsetPos);

	Add_Component(pCollider);

	m_pCollider = pCollider;

	if (!m_pCollider)
		return E_FAIL;





	return S_OK;
}

void CPurpleArrow::My_Tick()
{
	__super::My_Tick();
	
	if (m_eCurPhase == eSTICK)
	{
		CUnit* pUnit = static_cast<CUnit*>(m_pHitUnit);

		m_fCurPoisonTime += fDT(0);
	
		if (m_fCurPoisonTime > m_fMaxPoisonTime)
		{

			if (m_iTickCnt == m_iMaxTickCnt)
			{
				CState::HIT_INFO tHitInfo;
				ZeroMemory(&tHitInfo, sizeof(CState::HIT_INFO));

				tHitInfo.eHitType = CState::HIT_TYPE::eUP;
				tHitInfo.fKnockBackPower = 3.f;
				tHitInfo.fJumpPower = 0.f;

				pUnit->On_PlusHp(m_fDamage * 5.f, m_pOwnerUnit, false);
				pUnit->Enter_State(pUnit->Get_HitType().eGroggyState, &tHitInfo);
				
				if (pUnit->Get_Status().fHP < 0.f)
					pUnit->On_Die();
				
				DISABLE_GAMEOBJECT(this);
			}
				
			else
			{
				pUnit->On_PlusHp(m_fDamage, m_pOwnerUnit, false, 9999);
				
				if (pUnit->Get_Status().fHP < 0.f)
				{
					pUnit->On_Die();
					DISABLE_GAMEOBJECT(this);
				}
				
				++m_iTickCnt;
				m_fCurPoisonTime = 0.f;
			}



			
		}
	}
}

void CPurpleArrow::Set_ColliderType(eTEAM_TYPE eTeamType)
{
	if (eTeamType == eTEAM_TYPE::eBLUE)
		m_pCollider->Set_ColIndex(COL_BLUEGUARDBREAK);
	else
		m_pCollider->Set_ColIndex(COL_REDGUARDBREAK);
}