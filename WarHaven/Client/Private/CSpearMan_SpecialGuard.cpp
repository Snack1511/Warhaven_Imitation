#include "stdafx.h"
#include "CSpearMan_SpecialGuard.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

//CColorController::COLORDESC tColorDesc;

CSpearMan_SpecialGuard::CSpearMan_SpecialGuard()
{
}

CSpearMan_SpecialGuard::~CSpearMan_SpecialGuard()
{
}

CSpearMan_SpecialGuard* CSpearMan_SpecialGuard::Create()
{
    CSpearMan_SpecialGuard* pInstance = new CSpearMan_SpecialGuard();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSpear_GuardBreak");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSpearMan_SpecialGuard::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 8;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SPEARMAN_SPECIALGUARD;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_fInterPolationTime = 0.1f;

    m_fAnimSpeed = 2.f;

    m_iStateChangeKeyFrame = 70;

    m_vecAdjState.push_back(STATE_WALK_SPEARMAN_R);
    m_vecAdjState.push_back(STATE_RUN_SPEARMAN_R);
	m_vecAdjState.push_back(STATE_JUMP_SPEARMAN_R);

	Add_KeyFrame(55, 2);


    return S_OK;
}

void CSpearMan_SpecialGuard::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSpearMan_SpecialGuard::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	if (pAnimator->Is_CurAnimFinished())
		return STATE_SPEARMAN_BACKSTEP; // 원래는 STATE_IDle 스피어맨 상태로 이동해야함. 가드 테스트해야함

	if (pOwner->Is_Air())
		return STATE_JUMPFALL_SPEARMAN_R;

	if (m_bAttackTrigger)
	{
		if (pOwner->Is_Weapon_R_Collision())
			return STATE_BOUNCE_SPEARMAN;
	}

    return __super::Tick(pOwner, pAnimator);
}

void CSpearMan_SpecialGuard::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
}

STATE_TYPE CSpearMan_SpecialGuard::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* SPEARMAN이 SpecailGuard로 오는 조건
    1.  Q 를 누른다
    */

    if (KEY(Q, TAP))
        return m_eStateType;

    return STATE_END;
}


void CSpearMan_SpecialGuard::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{

	case 2:
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
		break;

	default:
		break;
	}


}