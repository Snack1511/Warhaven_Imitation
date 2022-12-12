#include "stdafx.h"
#include "CState_Combat_GuardEnd_Engineer.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_GuardEnd_Engineer::CState_Combat_GuardEnd_Engineer()
{
}

CState_Combat_GuardEnd_Engineer::~CState_Combat_GuardEnd_Engineer()
{
}

CState_Combat_GuardEnd_Engineer* CState_Combat_GuardEnd_Engineer::Create()
{
    CState_Combat_GuardEnd_Engineer* pInstance = new CState_Combat_GuardEnd_Engineer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_GuardEnd_Engineer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_GuardEnd_Engineer::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
	m_iAnimIndex = 3;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_GUARDEND_ENGINEER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	// 선형 보간 시간
	m_fInterPolationTime = 0.f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 2.5f;

    m_iStateChangeKeyFrame = 10;

    return S_OK;
}

void CState_Combat_GuardEnd_Engineer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_GuardEnd_Engineer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Get_CurAnimFrame() < m_iStateChangeKeyFrame)
        return AI_STATE_COMBAT_DEAFULT_ENGINEER_R;

	// 가드 상태에서도 움직일 수 있도록 설정한다.


    return __super::Tick(pOwner, pAnimator);

}

void CState_Combat_GuardEnd_Engineer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_GuardCollider(false);
}

STATE_TYPE CState_Combat_GuardEnd_Engineer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
