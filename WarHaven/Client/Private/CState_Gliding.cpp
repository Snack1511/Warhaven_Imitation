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
    if (ePrevType != m_ePreStateType && pOwner->Get_OwnerPlayer()->Get_CurClass() >= FIONA)
        m_iAnimIndex = 11;

    else
        m_iAnimIndex = 18;

    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fAirFriction = 0.01f;
    pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 5.f;
    pOwner->Enable_Glider(true);
    pOwner->Set_GliderAnimIndex(0, 0.1f, 2.f);

    m_fMaxSpeed = pOwner->Get_Status().fSprintSpeed;

    Physics_Setting(m_fMaxSpeed, pOwner);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Gliding::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (!pOwner->Is_Air())
    {
        STATE_TYPE eLandEndState = pOwner->Get_LandState();
        pOwner->Enable_Glider(false);
        m_bReturn = true;
        return eLandEndState;
    }

    _float4 vLook = pOwner->Get_FollowCamLook();
    vLook.y = 0.f;

    pOwner->Get_Transform()->Set_LerpLook(vLook, m_fMyMaxLerp);
    pOwner->Get_PhysicsCom()->Set_Dir(vLook);


    return __super::Tick(pOwner, pAnimator);
}

void CState_Gliding::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fAirFriction = 1.f;
    pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;
    
    if(!m_bReturn)
        pOwner->Enable_Glider(false);

}

STATE_TYPE CState_Gliding::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    if (CUser::Get_Instance()->Get_CurLevel() != LEVEL_HWARA && CUser::Get_Instance()->Get_CurLevel() != LEVEL_TEST)
        return STATE_END;

    if (pOwner->Is_Air())
    {
        if(KEY(SPACE, TAP))
            return m_eStateType;
    }
        


    return STATE_END;
}
