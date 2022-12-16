#include "stdafx.h"
#include "CState_Combat_GuardLoop_Paladin.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_GuardLoop_Paladin::CState_Combat_GuardLoop_Paladin()
{
}

CState_Combat_GuardLoop_Paladin::~CState_Combat_GuardLoop_Paladin()
{
}

CState_Combat_GuardLoop_Paladin* CState_Combat_GuardLoop_Paladin::Create()
{
    CState_Combat_GuardLoop_Paladin* pInstance = new CState_Combat_GuardLoop_Paladin();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_GuardLoop_Paladin");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_GuardLoop_Paladin::Initialize()
{
	m_eAnimType = ANIM_BASE_L;          // 애니메이션의 메쉬타입
	m_iAnimIndex = 38;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_GUARDLOOP_PALADIN;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	// 선형 보간 시간
	m_fInterPolationTime = 0.f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 2.f;

    m_iStateChangeKeyFrame = 30;

    return S_OK;
}

void CState_Combat_GuardLoop_Paladin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    m_iRand = random(0, 2);

	pOwner->Enable_GuardCollider(true);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_GuardLoop_Paladin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (Get_TargetLook_Length(pOwner) > 3.5f)
        return AI_STATE_COMBAT_DEAFULT_PALADIN_L;

    pOwner->Set_LookToTarget();

   
   

    switch (m_iRand)
    {
    case 0:
            
        if (pAnimator->Get_CurAnimFrame() < m_iStateChangeKeyFrame)
            return AI_STATE_COMBAT_HORIZONTALMIDDLE_PALADIN_L;

    case 1:

        if (pAnimator->Get_CurAnimFrame() < m_iStateChangeKeyFrame + 20)
            return AI_STATE_COMBAT_HORIZONTALMIDDLE_PALADIN_R;


    case 2:

        if(pAnimator->Is_CurAnimFinished())
            return AI_STATE_COMBAT_DEAFULT_PALADIN_L;

        break;

    default:
        break;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_GuardLoop_Paladin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_GuardCollider(false);

}

STATE_TYPE CState_Combat_GuardLoop_Paladin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
