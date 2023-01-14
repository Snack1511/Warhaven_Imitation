#include "stdafx.h"
#include "CState_PathNavigation_Sprint_Paladin_End.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "UsefulHeaders.h"

CState_PathNavigation_Sprint_Paladin_End::CState_PathNavigation_Sprint_Paladin_End()
{
}

CState_PathNavigation_Sprint_Paladin_End::~CState_PathNavigation_Sprint_Paladin_End()
{
}

CState_PathNavigation_Sprint_Paladin_End* CState_PathNavigation_Sprint_Paladin_End::Create()
{
    CState_PathNavigation_Sprint_Paladin_End* pInstance = new CState_PathNavigation_Sprint_Paladin_End();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Sprint_Paladin_End");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Sprint_Paladin_End::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 46;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_SPRINTEND_PALADIN;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 30;

    Add_KeyFrame(18, 222);
    Add_KeyFrame(29, 333);

  
    m_fAnimSpeed = 2.5f;

    return S_OK;
}

void CState_PathNavigation_Sprint_Paladin_End::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    m_fRand = frandom(0.3f, 0.8f);

    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;

    CState_PathNavigation::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Sprint_Paladin_End::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    if (m_fAIDelayTime > m_fRand)
        return AI_STATE_PATHNAVIGATION_DEFAULT_PALADIN_R;


    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Sprint_Paladin_End::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.F;
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Sprint_Paladin_End::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}


void CState_PathNavigation_Sprint_Paladin_End::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}