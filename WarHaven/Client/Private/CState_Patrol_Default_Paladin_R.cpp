#include "stdafx.h"
#include "CState_Patrol_Default_Paladin_R.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"

#include "CColorController.h"

#include "UsefulHeaders.h"

CState_Patrol_Default_Paladin_R::CState_Patrol_Default_Paladin_R()
{
}

CState_Patrol_Default_Paladin_R::~CState_Patrol_Default_Paladin_R()
{
}

CState_Patrol_Default_Paladin_R* CState_Patrol_Default_Paladin_R::Create()
{
    CState_Patrol_Default_Paladin_R* pInstance = new CState_Patrol_Default_Paladin_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Patrol_Default_Paladin_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Patrol_Default_Paladin_R::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 3;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATROL_DEAFULT_PALADIN_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iIdleDelay = 10;

    return S_OK;
}

void CState_Patrol_Default_Paladin_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    if (ePrevType == AI_STATE_PATROL_PALADIN_SWITCH_R_TO_L)
    {
        m_iIdleDelay = 2;
        m_fInterPolationTime = 0.f;
    }
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Patrol_Default_Paladin_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_iRand != 0)
    {
        if (pAnimator->Get_CurAnimFrame() > m_iIdleDelay)
        {
            if (m_iRand == 1)
                return AI_STATE_PATROL_PALADIN_SWITCH_R_TO_L;
            else
                return AI_STATE_PATROL_WALK_PALADIN_R;
        }
    }
    else
    {
        if (m_iRand == 1)
            return AI_STATE_PATROL_PALADIN_SWITCH_R_TO_L;
        else
            return AI_STATE_PATROL_WALK_PALADIN_R;
    }


    return __super::Tick(pOwner, pAnimator);
}

void CState_Patrol_Default_Paladin_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
    /* 할거없음 */
}

STATE_TYPE CState_Patrol_Default_Paladin_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
