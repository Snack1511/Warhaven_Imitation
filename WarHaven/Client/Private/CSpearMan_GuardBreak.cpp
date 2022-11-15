#include "stdafx.h"
#include "CSpearMan_GuardBreak.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

//CColorController::COLORDESC tColorDesc;

CSpearMan_GuardBreak::CSpearMan_GuardBreak()
{
}

CSpearMan_GuardBreak::~CSpearMan_GuardBreak()
{
}

CSpearMan_GuardBreak* CSpearMan_GuardBreak::Create()
{
    CSpearMan_GuardBreak* pInstance = new CSpearMan_GuardBreak();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSpear_GuardBreak");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSpearMan_GuardBreak::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 1;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SPEARMAN_GUARDBREAK;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_fInterPolationTime = 0.1f;

    m_fAnimSpeed = 2.f;

    m_iStateChangeKeyFrame = 70;

    m_vecAdjState.push_back(STATE_WALK_SPEARMAN_R);
    m_vecAdjState.push_back(STATE_RUN_SPEARMAN_R);

	m_vecAdjState.push_back(STATE_JUMP_SPEARMAN_R);


	m_vecAdjState.push_back(STATE_ATTACK_STING_SPEARMAN);
	m_vecAdjState.push_back(STATE_SPEARMAN_SPECIALGUARD);


	Add_KeyFrame(22, 0);
	Add_KeyFrame(41, 1);


    return S_OK;
}

void CSpearMan_GuardBreak::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSpearMan_GuardBreak::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	if (pAnimator->Is_CurAnimFinished())
		return STATE_IDLE_SPEARMAN_R;

	if (pOwner->Is_Air())
		return STATE_JUMPFALL_SPEARMAN_R;

	if (m_bAttackTrigger)
	{
		if (pOwner->Is_Weapon_R_Collision())
			return STATE_BOUNCE_SPEARMAN;
	}

    return __super::Tick(pOwner, pAnimator);
}

void CSpearMan_GuardBreak::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
}

STATE_TYPE CSpearMan_GuardBreak::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* SPEARMAN가 Attack 으로 오는 조건
    1.  스킬버튼 을 이용해 공격한다.
    */

    if (KEY(E, TAP))
        return m_eStateType;


    return STATE_END;
}


void CSpearMan_GuardBreak::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{

	case 0:
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
		Physics_Setting(pOwner->Get_Status().fGuardBreakSpeed, pOwner);
		pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 3.f;
		break;


	case 1:
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
		break;

	default:
		break;
	}


}