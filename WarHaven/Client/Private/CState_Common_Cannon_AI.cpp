#include "stdafx.h"
#include "CState_Common_Cannon_AI.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"

#include "CColorController.h"

#include "UsefulHeaders.h"

#include "CCannon.h"

CState_Common_Cannon_AI::CState_Common_Cannon_AI()
{
}

CState_Common_Cannon_AI::~CState_Common_Cannon_AI()
{
}

CState_Common_Cannon_AI* CState_Common_Cannon_AI::Create()
{
    CState_Common_Cannon_AI* pInstance = new CState_Common_Cannon_AI();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_Cannon_AI");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Common_Cannon_AI::Initialize()
{
    // 상하체 분리.
    // 
    // __super::Initialize();

    m_eAnimType = ANIM_ETC;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 0;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_CANNON_AI;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_tHitInfo.bFly = true;
    m_tHitInfo.iLandKeyFrame = 100;
    m_tHitInfo.fJumpPower = 6.f;
    m_tHitInfo.bGuardBreak = true;

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 1.5f;
    m_fDamagePumping = 7.f;

    return S_OK;
}

void CState_Common_Cannon_AI::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    CCannon* pCannon = pOwner->Get_AdjCannon();
    CGameObject* pTargetObj = pOwner->Get_OwnerPlayer()->Get_TargetObject();

    // 현재 인접한 타겟오브젝트가 대포가 아닐 시
    if (pCannon != pTargetObj)
    {
        m_eCannonState = CANNON_NOENTER;
        return;
    }
    else
        m_eCannonState = CANNON_FOLLOW;




    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Cannon_AI::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    CCannon* pCannon = pOwner->Get_AdjCannon();
    CGameObject* pTargetObject = pOwner->Get_TargetObject();


    switch (m_eCannonState)
    {
    case Client::CState_Common_Cannon_AI::CANNON_FOLLOW:

        if (pCannon)
        {
            m_eCannonState = CANNON_ENTER;
            if (!pCannon->Can_ControlCannon(pOwner->Get_OwnerPlayer()))
            {
                return pOwner->Get_DefaultState();
            }
        }

        else
            Follow_Move(pOwner, pTargetObject);


        break;

    case Client::CState_Common_Cannon_AI::CANNON_ENTER:

        m_fTimeAcc += fDT(0);

        if (pCannon)
        {
            if (m_fTimeAcc > 0.2f)
            {
                pOwner->Get_PhysicsCom()->Set_Speed(0.f);
                pCannon->Control_Cannon(pOwner->Get_OwnerPlayer());
                pOwner->Get_Transform()->Set_LerpLook(pCannon->Get_Transform()->Get_World(WORLD_LOOK), 0.4f);
                pOwner->Teleport_Unit(pCannon->Get_ControlPos());
                m_fTimeAcc = 4.5f;
                m_eCannonState = CANNON_CONTROL;
            }
        }
        else
            m_eCannonState = CANNON_FOLLOW;

        break;

    case Client::CState_Common_Cannon_AI::CANNON_CONTROL:

        m_fTimeAcc += fDT(0);

        if (m_fTimeAcc > 5.f)
            pCannon->Shoot_Cannon();

        break;

    case Client::CState_Common_Cannon_AI::CANNON_STOP:

        break;

    case Client::CState_Common_Cannon_AI::CANNON_NOENTER:

        if (m_ePreStateType != STATE_END)
            return m_ePreStateType;
        else
        {
            STATE_TYPE eDefaultState = pOwner->Get_DefaultState();
            return eDefaultState;
        }

    default:
        break;
    }


    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Cannon_AI::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

    if(m_eCannonState != CANNON_NOENTER)
        pOwner->Get_AdjCannon()->Exit_Cannon();

}

STATE_TYPE CState_Common_Cannon_AI::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
