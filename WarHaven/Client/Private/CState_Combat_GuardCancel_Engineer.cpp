#include "stdafx.h"
#include "CState_Combat_GuardCancel_Engineer.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_GuardCancel_Engineer::CState_Combat_GuardCancel_Engineer()
{
}

CState_Combat_GuardCancel_Engineer::~CState_Combat_GuardCancel_Engineer()
{
}

CState_Combat_GuardCancel_Engineer* CState_Combat_GuardCancel_Engineer::Create()
{
    CState_Combat_GuardCancel_Engineer* pInstance = new CState_Combat_GuardCancel_Engineer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_GuardCancel_Engineer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_GuardCancel_Engineer::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
	m_iAnimIndex = 1;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_GUARDCANCEL_ENGINEER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 3.f;

	m_iStateChangeKeyFrame = 100;

    return S_OK;
}

void CState_Combat_GuardCancel_Engineer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_GuardCancel_Engineer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// 가드 상태에서도 움직일 수 있도록 설정한다.
    if (pAnimator->Get_CurAnimFrame() > m_iStateChangeKeyFrame)
        return m_ePreStateType;

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_GuardCancel_Engineer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_GuardCancel_Engineer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{  
    return STATE_END;
}
