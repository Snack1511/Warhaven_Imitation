#include "stdafx.h"
#include "CSprint_End_Paladin.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CSprint_End_Paladin::CSprint_End_Paladin()
{
}

CSprint_End_Paladin::~CSprint_End_Paladin()
{
}

CSprint_End_Paladin* CSprint_End_Paladin::Create()
{
    CSprint_End_Paladin* pInstance = new CSprint_End_Paladin();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSprint_End_Paladin");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSprint_End_Paladin::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 46;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SPRINT_END_PALADIN;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 30;

    // 선형 보간 시간
    m_fInterPolationTime = 0.05f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복


    m_vecAdjState.push_back(STATE_GUARD_BEGIN_PALADIN);
    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PALADIN);

    m_vecAdjState.push_back(STATE_IDLE_PALADIN_R);
    m_vecAdjState.push_back(STATE_WALK_PALADIN_R);
    m_vecAdjState.push_back(STATE_RUN_PALADIN_R);

    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_R);
    m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT_PALADIN);

    m_vecAdjState.push_back(STATE_SHIELDWALL_BEGIN_PALADIN);
    m_vecAdjState.push_back(STATE_RUSH_BEGIN_PALADIN);
    m_vecAdjState.push_back(STATE_SHIELDSLAM_PALADIN);

    Init_CommonState_Player();


	m_fMyMaxLerp = 0.4f;
	m_fMaxSpeed = 10.f;
	m_fMyAccel = 10.f;

    Add_KeyFrame(15, 111);

    return S_OK;
}

void CSprint_End_Paladin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    if (ePrevType == STATE_SPRINT_LOOP_PALADIN)
        m_fInterPolationTime = 0.f;
    else
        m_fInterPolationTime = 0.05f;

    if (ePrevType == STATE_GLIDING)
        m_fInterPolationTime = 0.2f;

	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();


	//최대속도 설정
	pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fSprintSpeed);
	pMyPhysicsCom->Set_SpeedasMax();
    pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 0.5f;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSprint_End_Paladin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pOwner->Is_Air())
        return STATE_SPRINT_JUMPFALL_PALADIN;

    if (pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_PALADIN_R;

	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom(); 


    return __super::Tick(pOwner, pAnimator);

}

void CSprint_End_Paladin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
    pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 1.f;
    /* 할거없음 */
}

STATE_TYPE CSprint_End_Paladin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* PALADIN가 Sprint로 오는 조건
    1. 쉬프트를 누른 상태에서 움직인다.
    */
    // 만약 쉬프트키 나 W 를 해제했을 시
    if (KEY(LSHIFT, NONE) ||
        (KEY(W, NONE) && KEY(A, NONE) && KEY(S, NONE) && KEY(D, NONE)))
        return STATE_SPRINT_END_PALADIN;

    return STATE_END;
}
void	CSprint_End_Paladin::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    switch (iSequence)
    {
    case 111:
        Play_Sound(L"Env_FootStepGround", CHANNEL_ENVIRONMENT, 0.4f);
        CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SoilParticle_L_Foot", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
        break;
    default:
        break;
    }

}