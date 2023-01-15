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
#include "Physics.h"
#include "CGameSystem.h"

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

    CLASS_TYPE eMyPlayerClassType = pOwner->Get_OwnerPlayer()->Get_CurClass();

    m_eAnimType = ANIM_BASE_R;
    m_eGlideState = GLIDE_JUMP;

    switch (eMyPlayerClassType)
    {
    case Client::WARRIOR:

        m_iAnimIndex = 12;
        break;

    case Client::SPEAR:
        m_iAnimIndex = 444;
        break;

    case Client::ARCHER:
        m_iAnimIndex = 12;
        break;

    case Client::PALADIN:
        m_iAnimIndex = 4;
        break;

    case Client::PRIEST:
        m_iAnimIndex = 18;
        break;

    case Client::ENGINEER:
        m_iAnimIndex = 15;
        break;

    case Client::FIONA:
        m_iAnimIndex = 4;
        break;

    case Client::QANDA:
        m_iAnimIndex = 3;
        break;

    case Client::HOEDT:
        m_iAnimIndex = 444;
        break;

    case Client::LANCER:
        m_iAnimIndex = 7;
        m_eGlideState = GLIDE_NOENTER;
        break;

    default:
        break;
    }

    if (m_eGlideState != GLIDE_NOENTER)
    {
        pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fAirFriction = 0.01f;
        //pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 5.f;
        pOwner->Get_PhysicsCom()->Get_Physics().fPlusAcc = 0.3f;
        pOwner->Enable_Glider(true);
        pOwner->Set_GliderAnimIndex(0, 0.1f, 2.f);
        pOwner->Get_Glider()->Set_GliderState(CGlider::eGliderState::eOpen);

        if (pOwner->Get_OwnerPlayer()->Get_CurClass() == ARCHER)
            static_cast<CUnit_Archer*>(pOwner)->Enable_Arrow(false);
    }

    
    m_fMaxSpeed = pOwner->Get_Status().fSprintSpeed;
    Physics_Setting(m_fMaxSpeed, pOwner);
    pOwner->Get_PhysicsCom()->Set_Jump(pOwner->Get_Status().fJumpPower + 0.5f);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Gliding_AI::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    
    switch (m_eGlideState)
    {
    case Client::CState_Common_Gliding_AI::GLIDE_JUMP:

        Move_Gliding(pOwner, "Hwara_Center", false);

        if (pAnimator->Is_CurAnimFinished())
        {
            m_eAnimType = ANIM_ETC;
            m_iAnimIndex = 18;

            __super::Re_Enter(pOwner, pAnimator, m_fInterPolationTime, m_fAnimSpeed);
            m_eGlideState = GLIDE_LOOP;
        }

        break;

    case Client::CState_Common_Gliding_AI::GLIDE_LOOP:

        if (!pOwner->Is_Air())
            m_eGlideState = GLIDE_LAND;

        Move_Gliding(pOwner, "Hwara_Center", true);
        break;

    
    case Client::CState_Common_Gliding_AI::GLIDE_STOP:
        
        m_bReturn = true;
        return Glide_Cancel(pOwner);

    case Client::CState_Common_Gliding_AI::GLIDE_LAND:

        return Glide_Land(pOwner);


    case Client::CState_Common_Gliding_AI::GLIDE_NOENTER:
    default:

        return Glide_Cancel(pOwner);

        break;
    }





    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Gliding_AI::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fAirFriction = 1.f;
    pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;
    pOwner->Get_PhysicsCom()->Get_Physics().fPlusAcc = 1.f;

    if (m_eGlideState != GLIDE_NOENTER)
    {
        if (!m_bReturn)
            pOwner->Enable_Glider(false);

        if (pOwner->Get_OwnerPlayer()->Get_CurClass() == ARCHER)
            static_cast<CUnit_Archer*>(pOwner)->Enable_Arrow(true);

        pOwner->Get_Glider()->Set_GliderState(CGlider::eGliderState::eClose);
    }

}

STATE_TYPE CState_Common_Gliding_AI::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

STATE_TYPE CState_Common_Gliding_AI::Glide_Land(CUnit* pOwner)
{
    /* PathNavi SprintEnd 글라이더를 착용한 상태에서 착지 */
    STATE_TYPE eSprintEndState = pOwner->Get_SprintEndState();
    m_bReturn = true;
    return eSprintEndState;
}

STATE_TYPE CState_Common_Gliding_AI::Glide_Cancel(CUnit* pOwner)
{
    /* PathNavi SprintEnd 글라이더를 착용한 상태에서 해제 */
    STATE_TYPE eSprintFallState = pOwner->Get_SprintFallState();
    pOwner->Reset_GlidingTime();
    pOwner->Get_PhysicsCom()->Set_Jump(0.f);
    return  eSprintFallState;
}

void CState_Common_Gliding_AI::Move_Gliding(CGameObject* pSourObject, string strTargetPos, _bool bGilde)
{
    //타겟의 위치를 받아서 Look 방향을 구해 이동 시킬 수 있도록 설정.
    _float4 vLook = CGameSystem::Get_Instance()->Find_Position(strTargetPos) - pSourObject->Get_Transform()->Get_World(WORLD_POS);
    vLook.Normalize();

    if (vLook.y > -0.5f)
        vLook.y = 0.f;
    else
        vLook.y += 0.5f - FLT_MIN;

    CUnit* pMyUnit = dynamic_cast<CUnit*>(pSourObject);
   
    if (!pMyUnit)
    {
        Call_MsgBox(L"Move_Gliding a SourObject is not unit : AI_Gliding");
        return;
    }
        
    pMyUnit->Get_Transform()->Set_LerpLook(vLook, m_fMyMaxLerp);
    pMyUnit->Get_PhysicsCom()->Set_Dir(vLook);
}
