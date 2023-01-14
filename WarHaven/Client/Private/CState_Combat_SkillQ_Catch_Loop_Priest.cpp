#include "stdafx.h"
#include "CState_Combat_SkillQ_Catch_Loop_Priest.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"
#include "CUnit_Priest.h"


CState_Combat_SkillQ_Catch_Loop_Priest::CState_Combat_SkillQ_Catch_Loop_Priest()
{
}

CState_Combat_SkillQ_Catch_Loop_Priest::~CState_Combat_SkillQ_Catch_Loop_Priest()
{
}

CState_Combat_SkillQ_Catch_Loop_Priest* CState_Combat_SkillQ_Catch_Loop_Priest::Create()
{
    CState_Combat_SkillQ_Catch_Loop_Priest* pInstance = new CState_Combat_SkillQ_Catch_Loop_Priest();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_SkillQ_Catch_Loop_Priest");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_SkillQ_Catch_Loop_Priest::Initialize()
{
    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 9;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_PROJECTILECATCH_LOOP_PRIEST;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.4f;

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 999;

	return __super::Initialize();
}

void CState_Combat_SkillQ_Catch_Loop_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner�� Animator Set Idle�� */
	pOwner->Enable_GuardCollider(true);

	static_cast<CUnit_Priest*>(pOwner)->Turn_CatchEffet(true);
    __super::Enter(pOwner, pAnimator, ePrevType, pData);

	Play_Voice(pOwner, L"Voice_Catch", m_fAIDeafultVolume);
}

STATE_TYPE CState_Combat_SkillQ_Catch_Loop_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    // ���߿� ����

	if (KEY(Q, AWAY))
	{
		if (KEY(W, TAP) || KEY(A, TAP) || KEY(S, TAP) || KEY(D, TAP))
			return STATE_RUN_PRIEST;
		else
			return STATE_IDLE_PRIEST;

		pOwner->On_Use(CUnit::SKILL3);
	}

	if (pOwner->Get_CatchProjectileObject())
		return STATE_PROJECTILECATCH_HIT_PRIEST;

	else if (pOwner->Get_CatchedBall())
		return STATE_PROJECTILECATCH_HIT_PRIEST;

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillQ_Catch_Loop_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	static_cast<CUnit_Priest*>(pOwner)->Turn_CatchEffet(false);

	pOwner->Enable_GuardCollider(false);
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_SkillQ_Catch_Loop_Priest::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}


void CState_Combat_SkillQ_Catch_Loop_Priest::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{

}
