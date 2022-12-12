#include "stdafx.h"
#include "CState_Combat_GuardBegin_Fiona.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat_GuardBegin_Fiona::CState_Combat_GuardBegin_Fiona()
{
}

CState_Combat_GuardBegin_Fiona::~CState_Combat_GuardBegin_Fiona()
{
}

CState_Combat_GuardBegin_Fiona* CState_Combat_GuardBegin_Fiona::Create()
{
    CState_Combat_GuardBegin_Fiona* pInstance = new CState_Combat_GuardBegin_Fiona();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_GuardBegin_Fiona");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_GuardBegin_Fiona::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
	m_iAnimIndex = 0;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = AI_STATE_COMBAT_GUARDBEGIN_FIONA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	m_iStateChangeKeyFrame = 20;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 3.f;


    return S_OK;
}

void CState_Combat_GuardBegin_Fiona::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    m_iRand = random(0, 2);


	pOwner->Enable_GuardCollider(true);
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_GuardBegin_Fiona::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    pOwner->Set_LookToTarget();

    if (m_iRand == 0)
    {
        if (pAnimator->Get_CurAnimFrame() > m_iStateChangeKeyFrame)
        {
            if (Get_TargetLook_Length(pOwner) > 3.f)
               return AI_STATE_COMBAT_DEAFULT_FIONA_R;

            else
                return AI_STATE_COMBAT_HORIZONTALMIDDLE_FIONA_R;
        }
    }
    else
    {
        if (pAnimator->Is_CurAnimFinished())
        {
            return AI_STATE_COMBAT_GUARDLOOP_FIONA;
        }
    }

    return __super::Tick(pOwner, pAnimator);

}

void CState_Combat_GuardBegin_Fiona::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
	pOwner->Enable_GuardCollider(false);

}

STATE_TYPE CState_Combat_GuardBegin_Fiona::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
   
    return STATE_END;
}
