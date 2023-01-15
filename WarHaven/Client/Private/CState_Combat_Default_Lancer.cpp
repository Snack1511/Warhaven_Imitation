#include "stdafx.h"
#include "CState_Combat_Default_Lancer.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"
#include "CUnit_Lancer.h"

#include "CPlayer.h"

#include "CLancerNeedle.h"

CState_Combat_Default_Lancer::CState_Combat_Default_Lancer()
{
}

CState_Combat_Default_Lancer::~CState_Combat_Default_Lancer()
{
}

CState_Combat_Default_Lancer* CState_Combat_Default_Lancer::Create()
{
    CState_Combat_Default_Lancer* pInstance = new CState_Combat_Default_Lancer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_Default_Lancer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Default_Lancer::Initialize()
{

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 5;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMBAT_DEAFULT_LANCER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)
  
    m_iStateChangeKeyFrame = 99;

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;
    m_fMyMaxLerp = 0.4f;
    m_fMyAccel = 30.f;
    m_iDirectionRand = STATE_DIRECTION_N;
    //m_eJumpFallStateType = AI_STATE_COMMON_FALL_LANCER;

    m_fAIMyLength = 10.f;

    return __super::Initialize();
}

void CState_Combat_Default_Lancer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    if (ePrevType == m_eStateType)
        m_fInterPolationTime = 0.f;

    m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;
    

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Default_Lancer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{


    if(static_cast<CUnit_Lancer*>(pOwner)->Get_NeedleNums() >= CUnit_Lancer::eNeedle::eNeedle_Max)
        return AI_STATE_COMBAT_BREEZE_BEGIN_LANCER;
    

    _uint iFrame = pAnimator->Get_CurAnimFrame() + 1;

    
    if (pOwner->Get_TargetUnit())
    {
        if (fabs(Get_TargetLook_Length(pOwner)) < m_fAIMyLength - 3.f)
            return AI_STATE_COMBAT_ATTACKSWING_LANCER;

        else if (fabs(Get_TargetLook_Length(pOwner)) < m_fAIMyLength)
            return AI_STATE_COMBAT_STING_LANCER;
    }


    DoMove_AI(pOwner, pAnimator);

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Default_Lancer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Default_Lancer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}

void CState_Combat_Default_Lancer::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}
