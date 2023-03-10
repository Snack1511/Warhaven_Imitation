#include "stdafx.h"
#include "CState_Combat_Attack_Archer_Begin_Poison.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

#include "CCamera_Follow.h"
#include "CAnimWeapon.h"
#include "CUnit_Archer.h"
#include "HIerarchyNode.h"

CState_Combat_Attack_Archer_Begin_Poison::CState_Combat_Attack_Archer_Begin_Poison()
{
}

CState_Combat_Attack_Archer_Begin_Poison::~CState_Combat_Attack_Archer_Begin_Poison()
{
}

CState_Combat_Attack_Archer_Begin_Poison* CState_Combat_Attack_Archer_Begin_Poison::Create()
{
    CState_Combat_Attack_Archer_Begin_Poison* pInstance = new CState_Combat_Attack_Archer_Begin_Poison();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_Attack_Archer_Begin_Poison");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Attack_Archer_Begin_Poison::Initialize()
{
    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 1;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMBAT_ATTACK_BEGIN_POISION_ARCHER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.3f;
	m_iStateChangeKeyFrame = 99;

	m_fAIMyLength = 2.5f;

	Add_KeyFrame(31, 1);
	Add_KeyFrame(90, 2);


    return __super::Initialize();
}

void CState_Combat_Attack_Archer_Begin_Poison::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	__super::Enter_Attack_Begin(pOwner);
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Attack_Archer_Begin_Poison::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (m_bMoveTrigger)
		return AI_STATE_COMBAT_ATTACK_AIMING_POISION_ARCHER;

	if (m_bAttackTrigger)
		return AI_STATE_COMBAT_ATTACK_SHOOT_POISION_ARCHER;

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Attack_Archer_Begin_Poison::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_SkillTrigger().bSkillETrigger = false;
	pOwner->Set_AnimWeaponIndex(CAnimWeapon::eATTACKLOOP, FLT_MAX, FLT_MIN);
	//m_pCoreBone->Set_PrevMatrix(static_cast<CUnit_Archer*>(pOwner)->Get_CoreMat());

	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Attack_Archer_Begin_Poison::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_Combat_Attack_Archer_Begin_Poison::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	 __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

	switch (iSequence)
	{

	case 1:
		if (fabs(Get_TargetLook_Length(pOwner)) < m_fAIMyLength)
			m_bAttackTrigger = true;
		break;

	case 2:
		m_bMoveTrigger = true;
		break;

	default:
		break;
	}

}

