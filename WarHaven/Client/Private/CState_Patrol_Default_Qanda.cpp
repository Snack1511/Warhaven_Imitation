#include "stdafx.h"
#include "CState_Patrol_Default_Qanda.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"

#include "CColorController.h"

#include "UsefulHeaders.h"

CState_Patrol_Default_Qanda::CState_Patrol_Default_Qanda()
{
}

CState_Patrol_Default_Qanda::~CState_Patrol_Default_Qanda()
{
}

CState_Patrol_Default_Qanda* CState_Patrol_Default_Qanda::Create()
{
    CState_Patrol_Default_Qanda* pInstance = new CState_Patrol_Default_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Patrol_Default_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Patrol_Default_Qanda::Initialize()
{

    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 1;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATROL_DEFAULT_QANDA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_fMyMaxLerp = 0.4f;
    m_fMyAccel = 100.f;

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 1.f;

    m_iIdleDelay = 30;

    return __super::Initialize();
}

void CState_Patrol_Default_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    m_iRand = random(0, 5);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Patrol_Default_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_iRand != 0)
    {
        if (pAnimator->Get_CurAnimFrame() > m_iIdleDelay)
        {
            return AI_STATE_PATROL_WALK_QANDA;
        }
    }
    else
    {
        if (pAnimator->Get_CurAnimFrame() > m_iIdleDelay)
        {
            return m_eStateType;
        }
        
    }


    return __super::Tick(pOwner, pAnimator);
}

void CState_Patrol_Default_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
    /* 할거없음 */
}

STATE_TYPE CState_Patrol_Default_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
