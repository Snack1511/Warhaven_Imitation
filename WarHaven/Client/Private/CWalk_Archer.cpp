#include "stdafx.h"
#include "CWalk_Archer.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CAnimWeapon.h"

CWalk_Archer::CWalk_Archer()
{
}

CWalk_Archer::~CWalk_Archer()
{
}

HRESULT CWalk_Archer::Initialize()
{
    m_fInterPolationTime = 0.1f;

    m_vecAdjState.push_back(STATE_ATTACK_BEGIN_ARCHER);
    m_vecAdjState.push_back(STATE_ATTACK_BEGIN_SNIPING_ARCHER);
    m_vecAdjState.push_back(STATE_ATTACK_BEGIN_POISION_ARCHER);
    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_ARCHER);

    m_vecAdjState.push_back(STATE_GUARD_ARCHER);
    m_vecAdjState.push_back(STATE_ATTACK_SWING_ARCHER);

    Init_CommonState_Player();


	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;

   // m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PLAYER);

    Add_KeyFrame(37, 0);
    Add_KeyFrame(60, 0);

    m_fMyMaxLerp = 0.4f;
    m_fMyAccel = 100.f;

    return S_OK;
}

void CWalk_Archer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner의 Animator Set Idle로 */
	m_fMaxSpeed = pOwner->Get_Status().fWalkSpeed;
    pOwner->Set_AnimWeaponIndex(CAnimWeapon::eIDLE, m_fInterPolationTime, m_fAnimSpeed);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWalk_Archer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	Move_Direction_Loop(pOwner, pAnimator, 0.05f);



    return __super::Tick(pOwner, pAnimator);
}

void CWalk_Archer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CWalk_Archer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    // 천천히 
    if (KEY(C, HOLD))
    {
        // 걸어간다.
        if (KEY(W, HOLD) ||
            KEY(A, HOLD) ||
            KEY(S, HOLD) ||
            KEY(D, HOLD))
        {

            return m_eStateType;
        }

    }

    return STATE_END;
}

void CWalk_Archer::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

    switch (iSequence)
    {
    case 0:
        Play_Sound(L"Env_FootStepGround", CHANNEL_ENVIRONMENT, 0.4f);
        break;
    }
}