#include "stdafx.h"
#include "CAirDash_Priest.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CUnit_Priest.h"

CAirDash_Priest::CAirDash_Priest()
{
}

CAirDash_Priest::~CAirDash_Priest()
{
}

CAirDash_Priest* CAirDash_Priest::Create()
{
    CAirDash_Priest* pInstance = new CAirDash_Priest();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CAirDash_Priest");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CAirDash_Priest::Initialize()
{

    m_eAnimType = ANIM_ATTACK;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 1;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_AIRDASH_PRIEST;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 50;

    // 선형 보간 시간
    m_fInterPolationTime = 0.05f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 1.8f;

    m_vecAdjState.push_back(STATE_GLIDING);

	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 0;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 1;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = -1;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 2;

    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 1;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 1;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = -1;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = -1;

    m_fMyAccel = 100.f;
    m_fMyMaxLerp = 0.05f;

	iPlaceJumpAnimIndex = 1;

    Add_KeyFrame(7, 0);

    return S_OK;
}

void CAirDash_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    pOwner->On_Use(CUnit::SKILL1);

    m_fMaxSpeed = pOwner->Get_Status().fDashAttackSpeed * 1.2f;

    pOwner->Get_PhysicsCom()->Set_Jump(2.f);
    pOwner->Get_PhysicsCom()->Get_Physics().fPlusAcc = 0.92f;

    /* Owner의 Animator Set Idle로 */

    CTransform* pMyTransform = pOwner->Get_Transform();
    CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

    //_uint iDirection = Get_Direction();

    //if (iDirection == STATE_DIRECTION_END)
    //    m_iAnimIndex = iPlaceJumpAnimIndex;
    //else
    //    m_iAnimIndex = m_iDirectionAnimIndex[iDirection];


    Physics_Setting(m_fMaxSpeed, pOwner);

    _float4 vCamLook = pOwner->Get_FollowCamLook();
    vCamLook.y = 0.f;
    _float4 vCamRight = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_RIGHT);
    vCamRight.y = 0.f;

    _float4 vDir = pOwner->Get_FollowCamLook();


    _uint iFrame = pAnimator->Get_CurAnimFrame();

    _float4 vFinalDir = ZERO_VECTOR;

    if (KEY(W, HOLD))
    {
        vFinalDir += vCamLook;
    }
    if (KEY(A, HOLD))
    {
        vFinalDir -= vCamRight;
    }
    if (KEY(S, HOLD))
    {
        vFinalDir -= vCamLook;
    }
    if (KEY(D, HOLD))
    {
        vFinalDir += vCamRight;
    }

    if (vFinalDir.Is_Zero())
    {
        vFinalDir = pMyTransform->Get_World(WORLD_LOOK);
    }



    vFinalDir.Normalize();
    vFinalDir.y = 0.f;

    m_vDir = vFinalDir;

    pMyTransform->Set_LerpLook(vFinalDir, m_fMyMaxLerp);
    _float4 vUnitLook = pMyTransform->Get_MyWorld(WORLD_LOOK);
    pMyPhysicsCom->Set_Dir(vUnitLook);
    pMyPhysicsCom->Set_Accel(m_fMyAccel);

    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fAirFriction = 0.f;


    //DoMove(iDirection, pOwner);

    //pOwner->Get_PhysicsCom()->Set_MaxSpeed(m_fMaxSpeed);
    //pOwner->Get_PhysicsCom()->Set_SpeedasMax();

    static_cast<CUnit_Priest*>(pOwner)->TurnOn_Trail(true);
    CUser::Get_Instance()->SetActive_InteractUI(false); // test

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CAirDash_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (!pOwner->Is_Air())
        return STATE_SPRINT_END_PRIEST;
    
    if(pAnimator->Is_CurAnimFinished())
        return STATE_JUMPFALL_PRIEST;


    CTransform* pMyTransform = pOwner->Get_Transform();
    CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

    pMyTransform->Set_LerpLook(m_vDir, m_fMyMaxLerp);
    _float4 vUnitLook = pMyTransform->Get_MyWorld(WORLD_LOOK);
    pMyPhysicsCom->Set_Dir(m_vDir);
    pMyPhysicsCom->Set_Accel(m_fMyAccel);


    return __super::Tick(pOwner, pAnimator);
    
}

void CAirDash_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Get_Physics().fPlusAcc = 1.f;
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fAirFriction = 1.f;

    static_cast<CUnit_Priest*>(pOwner)->TurnOn_Trail(false);

}

STATE_TYPE CAirDash_Priest::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    if (!pOwner->Can_Use(CUnit::SKILL1))
        return STATE_END;
   
    if (KEY(LSHIFT, TAP))
    {
        return m_eStateType;
    }
    

    return STATE_END;

}

void CAirDash_Priest::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    switch (iSequence)
    {
    case 0:

        Play_Sound(L"Effect_AirDash_Priest");
        m_fMaxSpeed = pOwner->Get_Status().fDashAttackSpeed;
        Physics_Setting(m_fMaxSpeed, pOwner);

        break;

    default:
        break;
    }
}
