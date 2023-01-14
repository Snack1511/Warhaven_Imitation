#include "stdafx.h"
#include "CState_Common_Gliding_AI.h"

#include "UsefulHeaders.h"


#include "Functor.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "CUnit_Archer.h"
#include "Transform.h"
#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"
#include "Model.h"
#include "CColorController.h"
#include "CGlider.h"

CState_Common_Gliding_AI::CState_Common_Gliding_AI()
{
}

CState_Common_Gliding_AI::~CState_Common_Gliding_AI()
{
}

CState_Common_Gliding_AI* CState_Common_Gliding_AI::Create()
{
    CState_Common_Gliding_AI* pInstance = new CState_Common_Gliding_AI();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_Gliding_AI");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Common_Gliding_AI::Initialize()
{
    m_eAnimType = ANIM_ETC;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 18;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_GLIDING_AI;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_fMyAccel = 10.f;
    m_fMyMaxLerp = 0.4f;
    m_fAnimSpeed = 2.f;

    return S_OK;
}

void CState_Common_Gliding_AI::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{


    if (ePrevType != m_ePreStateType && pOwner->Get_OwnerPlayer()->Get_CurClass() >= FIONA)
        m_iAnimIndex = 11;

    else
        m_iAnimIndex = 18;

    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fAirFriction = 0.01f;
    //pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 5.f;
    pOwner->Get_PhysicsCom()->Get_Physics().fPlusAcc = 0.3f;
    pOwner->Enable_Glider(true);
    pOwner->Set_GliderAnimIndex(0, 0.1f, 2.f);
    pOwner->Get_Glider()->Set_GliderState(CGlider::eGliderState::eOpen);

    if (pOwner->Get_OwnerPlayer()->Get_CurClass() == ARCHER)
        static_cast<CUnit_Archer*>(pOwner)->Enable_Arrow(false);

    m_fMaxSpeed = pOwner->Get_Status().fSprintSpeed;

    Physics_Setting(m_fMaxSpeed, pOwner);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Gliding_AI::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (!pOwner->Is_Air())
    {
        //SprintEnd
        STATE_TYPE eSprintEndState = pOwner->Get_SprintEndState();
        m_bReturn = true;
        return eSprintEndState;
    }


    /* 날개 접는 코드 */
    //{
    //    STATE_TYPE eSprintFallState = pOwner->Get_SprintFallState();
    //    pOwner->Reset_GlidingTime();
    //    pOwner->Get_PhysicsCom()->Set_Jump(0.f);
    //    m_bReturn = true;
    //    return  eSprintFallState;
    //}


    // 이전 : 카메라 Look 방향으로 이동 
    // 변경 예정 : TargetObj? 아니면 Trigger? 의 위치를 받아서 Look 방향을 구해 이동 시킬 수 있도록 설정.
    _float4 vLook = pOwner->Get_FollowCamLook();
    
    if (vLook.y > -0.5f)
        vLook.y = 0.f;
    else
        vLook.y += 0.5f - FLT_MIN;

    pOwner->Get_Transform()->Set_LerpLook(vLook, m_fMyMaxLerp);
    pOwner->Get_PhysicsCom()->Set_Dir(vLook);


    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Gliding_AI::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fAirFriction = 1.f;
    pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;
    pOwner->Get_PhysicsCom()->Get_Physics().fPlusAcc = 1.f;

    if(!m_bReturn)
        pOwner->Enable_Glider(false);

    if (pOwner->Get_OwnerPlayer()->Get_CurClass() == ARCHER)
        static_cast<CUnit_Archer*>(pOwner)->Enable_Arrow(true);

    pOwner->Get_Glider()->Set_GliderState(CGlider::eGliderState::eClose);

}

STATE_TYPE CState_Common_Gliding_AI::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
