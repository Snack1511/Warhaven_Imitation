#include "stdafx.h"
#include "CState_PathNavigation_Sprint_Warrior_End.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "UsefulHeaders.h"

CState_PathNavigation_Sprint_Warrior_End::CState_PathNavigation_Sprint_Warrior_End()
{
}

CState_PathNavigation_Sprint_Warrior_End::~CState_PathNavigation_Sprint_Warrior_End()
{
}

CState_PathNavigation_Sprint_Warrior_End* CState_PathNavigation_Sprint_Warrior_End::Create()
{
    CState_PathNavigation_Sprint_Warrior_End* pInstance = new CState_PathNavigation_Sprint_Warrior_End();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Sprint_Warrior_End");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Sprint_Warrior_End::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 56;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_SPRINTEND_WARRIOR;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 30;

    Add_KeyFrame(18, 222);
    Add_KeyFrame(29, 333);

    

    m_fAnimSpeed = 2.5f;

    return __super::Initialize();
}

void CState_PathNavigation_Sprint_Warrior_End::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    m_fRand = frandom(0.3f, 0.8f);

    pOwner->Get_PhysicsCom()->Set_MaxSpeed(pOwner->Get_Status().fSprintSpeed);
    pOwner->Get_PhysicsCom()->Set_SpeedasMax();
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;

    CState_PathNavigation::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Sprint_Warrior_End::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    if (pAnimator->Is_CurAnimFinished())
        return AI_STATE_PATHNAVIGATION_DEFAULT_WARRIOR_R;


    return CState::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Sprint_Warrior_End::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Sprint_Warrior_End::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}


void CState_PathNavigation_Sprint_Warrior_End::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}