#include "stdafx.h"
#include "CState_Combat_GuardCancel_Fiona.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_GuardCancel_Fiona::CState_Combat_GuardCancel_Fiona()
{
}

CState_Combat_GuardCancel_Fiona::~CState_Combat_GuardCancel_Fiona()
{
}

CState_Combat_GuardCancel_Fiona* CState_Combat_GuardCancel_Fiona::Create()
{
    CState_Combat_GuardCancel_Fiona* pInstance = new CState_Combat_GuardCancel_Fiona();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_GuardCancel_Fiona");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_GuardCancel_Fiona::Initialize()
{
	m_eAnimType = ANIM_BASE_L;          // 애니메이션의 메쉬타입
	m_iAnimIndex = 47;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_GUARDCANCEL_FIONA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 3.f;

	m_iStateChangeKeyFrame = 100;

    return S_OK;
}

void CState_Combat_GuardCancel_Fiona::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    if (ePrevType == AI_STATE_COMBAT_COUNTER_FIONA)
        m_bAttackTrigger = true;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_GuardCancel_Fiona::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_bAttackTrigger)
        return AI_STATE_COMBAT_SPINATTACK_FIONA;

	// 가드 상태에서도 움직일 수 있도록 설정한다.
    if (pAnimator->Get_CurAnimFrame() > m_iStateChangeKeyFrame)
    {
        if (m_ePreStateType != STATE_END)
            return m_ePreStateType;
        else
            return AI_STATE_COMBAT_DEFAULT_FIONA_R;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_GuardCancel_Fiona::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_GuardCancel_Fiona::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{  
    return STATE_END;
}
