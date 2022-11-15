#include "stdafx.h"
#include "CSpearMan_BackStep.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

//CColorController::COLORDESC tColorDesc;

CSpearMan_BackStep::CSpearMan_BackStep()
{
}

CSpearMan_BackStep::~CSpearMan_BackStep()
{
}

CSpearMan_BackStep* CSpearMan_BackStep::Create()
{
    CSpearMan_BackStep* pInstance = new CSpearMan_BackStep();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSpear_GuardBreak");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSpearMan_BackStep::Initialize()
{

    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 0;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SPEARMAN_BACKSTEP;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_fInterPolationTime = 0.1f;

    m_fAnimSpeed = 2.f;

    m_iStateChangeKeyFrame = 47;

    m_vecAdjState.push_back(STATE_WALK_SPEARMAN_R);
    m_vecAdjState.push_back(STATE_RUN_SPEARMAN_R);
	m_vecAdjState.push_back(STATE_JUMP_SPEARMAN_R);

	m_vecAdjState.push_back(STATE_ATTACK_STING_SPEARMAN);
	m_vecAdjState.push_back(STATE_SPEARMAN_GUARDBREAK);

    return S_OK;
}

void CSpearMan_BackStep::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	Physics_Setting(pOwner->Get_Status().fBackStepSpeed, pOwner, true, true);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSpearMan_BackStep::Tick(CUnit* pOwner, CAnimator* pAnimator)
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

void CSpearMan_BackStep::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CSpearMan_BackStep::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* SPEARMAN가 BackStep 으로 오는 조건
    1.  공격에 맞으면
    */

    return STATE_END;
}
