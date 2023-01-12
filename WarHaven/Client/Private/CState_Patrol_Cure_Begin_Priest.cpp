#include "stdafx.h"
#include "CState_Patrol_Cure_Begin_Priest.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"


CState_Patrol_Cure_Begin_Priest::CState_Patrol_Cure_Begin_Priest()
{
}

CState_Patrol_Cure_Begin_Priest::~CState_Patrol_Cure_Begin_Priest()
{
}

CState_Patrol_Cure_Begin_Priest* CState_Patrol_Cure_Begin_Priest::Create()
{
    CState_Patrol_Cure_Begin_Priest* pInstance = new CState_Patrol_Cure_Begin_Priest();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Patrol_Cure_Begin_Priest");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Patrol_Cure_Begin_Priest::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 5;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATROL_CURE_BEGIN_PRIEST;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.4f;

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_iStateChangeKeyFrame = 99;

	return __super::Initialize();
}

void CState_Patrol_Cure_Begin_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Patrol_Cure_Begin_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    CGameObject* pCureObject = pOwner->Get_CureObject();

    if (!pCureObject)
        return AI_STATE_PATROL_CURE_END_PRIEST;

    _float4 vTargetLook = pCureObject->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);
    vTargetLook.y = 0.f;

    pOwner->Get_Transform()->Set_LerpLook(vTargetLook, 0.4f);

	m_fTimeAcc += fDT(0);

	if (pAnimator->Is_CurAnimFinished() || m_fTimeAcc > 0.5f)
	{
		if (pCureObject)
			return AI_STATE_PATROL_CURE_LOOP_PRIEST;
		else
			return AI_STATE_PATROL_CURE_END_PRIEST;
	}


    return __super::Tick(pOwner, pAnimator);
}

void CState_Patrol_Cure_Begin_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Patrol_Cure_Begin_Priest::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	CUnit* pOtherUnit = static_cast<CUnit*>(pOwner->Get_CureObject());

	if (!pOtherUnit)
		return STATE_END;

    //if (KEY(RBUTTON, TAP))
    //    return m_eStateType;

    return STATE_END;
}


void CState_Patrol_Cure_Begin_Priest::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{

}
