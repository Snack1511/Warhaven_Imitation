#include "stdafx.h"
#include "CState_Patrol_Default_Engineer_L.h"

#include "GameInstance.h"
#include "Functor.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "Transform.h"
#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"
#include "Model.h"
#include "CColorController.h"

CState_Patrol_Default_Engineer_L::CState_Patrol_Default_Engineer_L()
{
}

CState_Patrol_Default_Engineer_L::~CState_Patrol_Default_Engineer_L()
{
}

CState_Patrol_Default_Engineer_L* CState_Patrol_Default_Engineer_L::Create()
{
    CState_Patrol_Default_Engineer_L* pInstance = new CState_Patrol_Default_Engineer_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Patrol_Default_Engineer_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Patrol_Default_Engineer_L::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_BASE_L;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 4;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATROL_DEFAULT_ENGINEER_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    return S_OK;
}

void CState_Patrol_Default_Engineer_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
 
    if (ePrevType == AI_STATE_PATROL_ENGINEER_SWITCH_R_TO_L)
    {
        m_iIdleDelay = 2;
        m_fInterPolationTime = 0.f;
    }


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Patrol_Default_Engineer_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_iRand != 0)
    {
        if (pAnimator->Get_CurAnimFrame() > m_iIdleDelay)
        {
            if(m_iRand == 1)
                return AI_STATE_PATROL_ENGINEER_SWITCH_L_TO_R;
            else
                return AI_STATE_PATROL_WALK_ENGINEER_L;
        }
    }
    else
    {
        if (m_iRand == 1)
            return AI_STATE_PATROL_ENGINEER_SWITCH_L_TO_R;
        else
            return AI_STATE_PATROL_WALK_ENGINEER_L;
    }
    

    return __super::Tick(pOwner, pAnimator);
}

void CState_Patrol_Default_Engineer_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
    /* 할거없음 */
}

STATE_TYPE CState_Patrol_Default_Engineer_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
