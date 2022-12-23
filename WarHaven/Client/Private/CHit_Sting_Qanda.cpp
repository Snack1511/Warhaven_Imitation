#include "stdafx.h"
#include "CHit_Sting_Qanda.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"

CHit_Sting_Qanda::CHit_Sting_Qanda()
{
}

CHit_Sting_Qanda::~CHit_Sting_Qanda()
{
}

CHit_Sting_Qanda* CHit_Sting_Qanda::Create()
{
    CHit_Sting_Qanda* pInstance = new CHit_Sting_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHit_Sting_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CHit_Sting_Qanda::Initialize()
{
    
    m_eAnimType = ANIM_HIT;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 9;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_STINGHIT_ARCHER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;

    m_iStateChangeKeyFrame = 50;

    m_fMyAccel = 10.f;
    m_fMyMaxLerp = 10.f;
    m_fMaxSpeed = 0.8f;

    return __super::Initialize();
}

void CHit_Sting_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_TYPE::CAMERA_LERP_DEFAULT);

    pOwner->Get_Status().fRunSpeed = pOwner->Get_Status().fStoreSpeed;
    pOwner->Get_Status().fWalkSpeed = pOwner->Get_Status().fBackStepSpeed;

    m_tHitInfo = *((HIT_INFO*)(pData));
    __super::Sting_State(pOwner);

    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CHit_Sting_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CHit_Sting_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	//pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);

}

STATE_TYPE CHit_Sting_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Attack 으로 오는 조건
    1.  LBuutton 을 이용해 공격한다.
    */
   
    return STATE_END;
}
