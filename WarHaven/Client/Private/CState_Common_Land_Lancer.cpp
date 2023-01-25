#include "stdafx.h"
#include "CState_Common_Land_Lancer.h"


#include "UsefulHeaders.h"


CState_Common_Land_Lancer::CState_Common_Land_Lancer()
{
}

CState_Common_Land_Lancer::~CState_Common_Land_Lancer()
{
}

CState_Common_Land_Lancer* CState_Common_Land_Lancer::Create()
{
    CState_Common_Land_Lancer* pInstance = new CState_Common_Land_Lancer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_Land_Lancer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Common_Land_Lancer::Initialize()
{

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 8;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_LAND_LANCER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.05f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;

    return __super::Initialize();
}

void CState_Common_Land_Lancer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Land_Lancer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Land_Lancer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Common_Land_Lancer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
