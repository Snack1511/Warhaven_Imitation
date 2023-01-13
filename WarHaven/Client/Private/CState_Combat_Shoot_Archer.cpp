#include "stdafx.h"
#include "CState_Combat_Shoot_Archer.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

#include "CCamera_Follow.h"
#include "CUnit_Archer.h"
#include "CAnimWeapon.h"
#include "HierarchyNode.h"

CState_Combat_Shoot_Archer::CState_Combat_Shoot_Archer()
{
}

CState_Combat_Shoot_Archer::~CState_Combat_Shoot_Archer()
{
}

CState_Combat_Shoot_Archer* CState_Combat_Shoot_Archer::Create()
{
    CState_Combat_Shoot_Archer* pInstance = new CState_Combat_Shoot_Archer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_Shoot_Archer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Shoot_Archer::Initialize()
{
	m_tHitInfo.eHitType = HIT_TYPE::eUP;
	m_tHitInfo.fKnockBackPower = 1.f;
	m_tHitInfo.fJumpPower = 0.f;
    // 선형 보간 시간

    m_fInterPolationTime = 0.f;
    m_fAnimSpeed = 2.3f;
    m_iStateChangeKeyFrame = 99;


	Add_KeyFrame(32, 1);

    return __super::Initialize();
}

void CState_Combat_Shoot_Archer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_iRand = random(0, 2);

	if (m_iRand == 1)
		m_fMaxTime = 0.2f;
	else if (m_iRand == 0)
		m_fMaxTime = 0.4f;


	if (pOwner->Get_Status().eChargeType == CUnit::UNIT_CHARGESTEP1)
		m_fDamagePumping = 1.f;
	else if (pOwner->Get_Status().eChargeType == CUnit::UNIT_CHARGESTEP2)
		m_fDamagePumping = 1.2f;
	else if (pOwner->Get_Status().eChargeType == CUnit::UNIT_CHARGESTEP3)
		m_fDamagePumping = 1.5f;


	if (ePrevType == AI_STATE_COMBAT_ATTACK_BEGIN_ARCHER ||
		ePrevType == AI_STATE_COMBAT_ATTACK_BEGIN_POISION_ARCHER ||
		ePrevType == AI_STATE_COMBAT_ATTACK_BEGIN_SNIPING_ARCHER)
		m_fDamagePumping = 0.7f;

	_bool bBounce = false;

	if (ePrevType == AI_STATE_COMMON_BOUNCE_ARCHER)
	{
		bBounce = true;
		m_fInterPolationTime = 0.f;
	}

	pOwner->Set_AnimWeaponIndex(CAnimWeapon::eATTACKLAUNCH, m_fInterPolationTime, m_fAnimSpeed);

	pOwner->Get_Status().fRunSpeed = pOwner->Get_Status().fStoreSpeed * 0.7f;
	pOwner->Get_Status().fWalkSpeed = pOwner->Get_Status().fBackStepSpeed * 0.7f;

	_float4 vRight = pOwner->Get_Transform()->Get_World(WORLD_RIGHT);

	//pOwner->Get_Transform()->Set_Right(pOwner->Get_FollowCamLook());
	//pOwner->Get_Transform()->Make_WorldMatrix();
	static_cast<CUnit_Archer*>(pOwner)->Shoot_Arrow();
	//pOwner->Get_Transform()->Set_Right(vRight);

	//_float4x4 matOffset;
	//matOffset.Identity();
	//CHierarchyNode* pCoreBone = GET_COMPONENT_FROM(pOwner, CModel)->Find_HierarchyNode("0B_Spine");
	//pCoreBone->Set_PrevMatrix(matOffset);
	//static_cast<CUnit_Archer*>(pOwner)->Get_CoreMat() = matOffset;

	pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_TYPE::CAMERA_LERP_DEFAULT);
    
	if (bBounce)
	{
		pAnimator->Set_CurFrame(pOwner->Get_PreAnimIndex());
	}

	CState_Combat::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Shoot_Archer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (m_bAttackTrigger)
	{
		if (m_iRand == 0)
			return AI_STATE_COMBAT_DEFAULT_ARCHER_R;
		else
		{
			m_fTimeAcc += fDT(0);

			if (m_fTimeAcc > m_fMaxTime)
			{
				if (pOwner->Get_SkillTrigger().bSkillQTrigger && !pOwner->Get_SkillTrigger().bSkillETrigger)
				{
					pOwner->Get_SkillTrigger().bSkillQTrigger = false;
					return AI_STATE_COMBAT_ATTACK_BEGIN_SNIPING_ARCHER;
				}

				else if (pOwner->Get_SkillTrigger().bSkillETrigger && !pOwner->Get_SkillTrigger().bSkillQTrigger)
				{
					pOwner->Get_SkillTrigger().bSkillETrigger = false;
					return AI_STATE_COMBAT_ATTACK_BEGIN_POISION_ARCHER;
				}

				else if (!pOwner->Get_SkillTrigger().bSkillETrigger && !pOwner->Get_SkillTrigger().bSkillQTrigger)
					return AI_STATE_COMBAT_ATTACK_BEGIN_ARCHER;
			}
		}

	}

	if (pAnimator->Is_CurAnimFinished())
		return AI_STATE_COMBAT_DEFAULT_ARCHER_R;

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Shoot_Archer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Set_AnimWeaponIndex(CAnimWeapon::eIDLE, m_fInterPolationTime, m_fAnimSpeed);

	if (!m_bAttackTrigger)
		static_cast<CUnit_Archer*>(pOwner)->Create_DefaultArrow();

	pOwner->Get_Status().fRunSpeed = pOwner->Get_Status().fStoreSpeed;
	pOwner->Get_Status().fWalkSpeed = pOwner->Get_Status().fBackStepSpeed;

	pOwner->Get_PreAnimIndex() = pAnimator->Get_CurAnimFrame();
	
	pOwner->Get_Status().eChargeType = CUnit::UNIT_CHARGESTEP_END;

	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Shoot_Archer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_Combat_Shoot_Archer::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	// __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);


	switch (iSequence)
	{

	case 1:

		Choice_Arrow(pOwner);
		m_bAttackTrigger = true;

		break;

	default:
		break;
	}

}

void CState_Combat_Shoot_Archer::Choice_Arrow(CUnit* pOwner)
{
	switch (m_iRand)
	{
	case 0:
		static_cast<CUnit_Archer*>(pOwner)->Create_DefaultArrow();
		break;

	case 1:

		if (pOwner->Can_Use(CUnit::SKILL2))
		{
			static_cast<CUnit_Archer*>(pOwner)->Create_PurpleArrow();
			pOwner->Get_SkillTrigger().bSkillETrigger = true;
			pOwner->On_Use(CUnit::SKILL2);
		}
			

		else
			static_cast<CUnit_Archer*>(pOwner)->Create_DefaultArrow();

		break;

	case 2:

		if (pOwner->Can_Use(CUnit::SKILL3))
		{
			static_cast<CUnit_Archer*>(pOwner)->Create_SnipeArrow();
			pOwner->Get_SkillTrigger().bSkillQTrigger = true;
			pOwner->On_Use(CUnit::SKILL3);
		}
			
		else
			static_cast<CUnit_Archer*>(pOwner)->Create_DefaultArrow();


		break;

	default:
		break;
	}

}
