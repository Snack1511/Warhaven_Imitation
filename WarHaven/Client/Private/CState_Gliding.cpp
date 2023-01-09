#include "stdafx.h"
#include "CState_Gliding.h"

#include "UsefulHeaders.h"


#include "Functor.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "Transform.h"
#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"
#include "Model.h"
#include "CColorController.h"
#include "CGlider.h"

CState_Gliding::CState_Gliding()
{
}

CState_Gliding::~CState_Gliding()
{
}

CState_Gliding* CState_Gliding::Create()
{
    CState_Gliding* pInstance = new CState_Gliding();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Gliding");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Gliding::Initialize()
{
    m_eAnimType = ANIM_ETC;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 18;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_GLIDING;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_fMyAccel = 10.f;
    m_fMyMaxLerp = 10.f;
    m_fAnimSpeed = 2.f;

    return S_OK;
}

void CState_Gliding::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    CUser::Get_Instance()->SetActive_InteractUI(false);

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

    GAMEINSTANCE->Start_RadialBlur(0.011f);

    m_fMaxSpeed = pOwner->Get_Status().fSprintSpeed;

    Physics_Setting(m_fMaxSpeed, pOwner);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Gliding::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (!pOwner->Is_Air())
    {
        //SprintEnd
        STATE_TYPE eSprintEndState = pOwner->Get_SprintEndState();
        m_bReturn = true;
        return eSprintEndState;
    }

    if (KEY(SPACE, TAP))
    {
        STATE_TYPE eSprintFallState = pOwner->Get_SprintFallState();
        pOwner->Reset_GlidingTime();
        pOwner->Get_PhysicsCom()->Set_Jump(0.f);
        m_bReturn = true;
        return  eSprintFallState;
    }

    _float4 vLook = pOwner->Get_FollowCamLook();
    
    if (vLook.y > -0.5f)
        vLook.y = 0.f;
    else
        vLook.y += 0.5f - FLT_MIN;

    pOwner->Get_Transform()->Set_LerpLook(vLook, m_fMyMaxLerp);
    pOwner->Get_PhysicsCom()->Set_Dir(vLook);


    return __super::Tick(pOwner, pAnimator);
}

void CState_Gliding::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fAirFriction = 1.f;
    pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;
    pOwner->Get_PhysicsCom()->Get_Physics().fPlusAcc = 1.f;

    if(!m_bReturn)
        pOwner->Enable_Glider(false);

    GAMEINSTANCE->Stop_RadialBlur();

    pOwner->Get_Glider()->Set_GliderState(CGlider::eGliderState::eClose);

}

STATE_TYPE CState_Gliding::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    if (CUser::Get_Instance()->Get_CurLevel() != LEVEL_HWARA && CUser::Get_Instance()->Get_CurLevel() != LEVEL_TEST)
        return STATE_END;

    if (pOwner->Get_GlidingTime() > 0.f)
        return STATE_END;

    if (pOwner->Is_Air())
    {
        CUser::Get_Instance()->Set_InteractKey(50);
        CUser::Get_Instance()->Set_InteractText(TEXT("활공"));
        CUser::Get_Instance()->SetActive_InteractUI(true);

        if (KEY(SPACE, TAP))
        {
            return m_eStateType;
        }
    }
    else
    {
        CUser::Get_Instance()->SetActive_InteractUI(false);

    }
        

    return STATE_END;
}
