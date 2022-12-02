#include "stdafx.h"
#include "CAI_TG_Groggy.h"

#include "GameInstance.h"
#include "Physics.h"
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

#include "CUtility_Transform.h"

CAI_TG_Groggy::CAI_TG_Groggy()
{
}

CAI_TG_Groggy::~CAI_TG_Groggy()
{
}

CAI_TG_Groggy* CAI_TG_Groggy::Create()
{
    CAI_TG_Groggy* pInstance = new CAI_TG_Groggy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CAI_TG_Groggy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CAI_TG_Groggy::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_HIT;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 2;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_TG_GROGGYHIT_WARRIOR;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 30;

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 1.5f;

    //m_vecAdjState.push_back(STATE_IDLE_WARRIOR_R_AI_ENEMY);
   // m_vecAdjState.push_back(STATE_IDLE_WARRIOR_L_AI_ENEMY);

    

    return S_OK;
}

void CAI_TG_Groggy::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{

    /* 날 때린놈의 hit info를 받았다. */
    m_tHitInfo = *((HIT_INFO*)(pData));
    __super::Groggy_State(pOwner);

    /* Owner의 Animator Set Idle로 */
    //GET_COMPONENT_FROM(pOwner, CModel)->Set_ShaderColor(MODEL_PART_WEAPON, _float4(1, 0.3, 0, 0));

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CAI_TG_Groggy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    if(pAnimator->Is_CurAnimFinished())
        return AI_STATE_IDLE_WARRIOR_R;

    return __super::Tick(pOwner, pAnimator);
}

void CAI_TG_Groggy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CAI_TG_Groggy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Idle로 오는 조건
    1. 현재 진행중인 애니메이션이 끝났을 때
    */

    //if (pAnimator->Is_CurAnimFinished())
    //    return m_eStateType;


    return STATE_END;
}
