#include "stdafx.h"
#include "CState_Common_FlyHit_Fiona.h"

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

CState_Common_FlyHit_Fiona::CState_Common_FlyHit_Fiona()
{
}

CState_Common_FlyHit_Fiona::~CState_Common_FlyHit_Fiona()
{
}

CState_Common_FlyHit_Fiona* CState_Common_FlyHit_Fiona::Create()
{
    CState_Common_FlyHit_Fiona* pInstance = new CState_Common_FlyHit_Fiona();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_FlyHit_Fiona");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CState_Common_FlyHit_Fiona::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_HIT;            // 애니메이션의 메쉬타입
    m_iAnimIndex = m_iFlyHitIndex;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_FLYHIT_FIONA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 3.f;

    m_iStateChangeKeyFrame = 99;

    return S_OK;
}

void CState_Common_FlyHit_Fiona::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    /* 날 때린놈의 hit info를 받았다. */

    if (ePrevType == AI_STATE_COMBAT_COUNTER_FIONA)
        m_bAttackTrigger = true;

    m_tHitInfo = *((HIT_INFO*)(pData));
    __super::Fly_State();

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_FlyHit_Fiona::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_bAttackTrigger)
        return AI_STATE_COMBAT_SPINATTACK_FIONA;

    if (pAnimator->Get_CurAnimFrame() > m_tHitInfo.iLandKeyFrame)
        return AI_STATE_COMMON_FALL_FIONA_R;//STATE_FALL_FIONA_R_AI_ENEMY;


    if (pAnimator->Is_CurAnimFinished())
        return AI_STATE_COMMON_FALL_FIONA_R;

    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_FlyHit_Fiona::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Common_FlyHit_Fiona::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    return STATE_END;
}
