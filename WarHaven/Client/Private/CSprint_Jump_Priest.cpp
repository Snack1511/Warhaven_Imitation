#include "stdafx.h"
#include "CSprint_Jump_Priest.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CSprint_Jump_Priest::CSprint_Jump_Priest()
{
}

CSprint_Jump_Priest::~CSprint_Jump_Priest()
{
}

CSprint_Jump_Priest* CSprint_Jump_Priest::Create()
{
    CSprint_Jump_Priest* pInstance = new CSprint_Jump_Priest();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSprint_Jump_Priest");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSprint_Jump_Priest::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 46;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SPRINT_JUMP_PRIEST;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;

    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    m_vecAdjState.push_back(STATE_GLIDING);
    m_vecAdjState.push_back(STATE_SPRINT_JUMPFALL_PRIEST);
    m_vecAdjState.push_back(STATE_JUMP_LAND_PRIEST);
    m_vecAdjState.push_back(STATE_PROJECTILECATCH_BEGIN_PRIEST);
    m_vecAdjState.push_back(STATE_AIRDASH_PRIEST);
    m_vecAdjState.push_back(STATE_WINDATTACK_PRIEST);
    m_vecAdjState.push_back(STATE_CURE_BEGIN_PRIEST);
    m_vecAdjState.push_back(STATE_ATTACK_STING_PRIEST);

    Init_CommonState_Player();


    return S_OK;
}

void CSprint_Jump_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Get_PhysicsCom()->Set_Jump(pOwner->Get_Status().fJumpPower + 0.5f);

    CTransform* pMyTransform = pOwner->Get_Transform();
    CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

    _float4 vCamLook = pOwner->Get_FollowCamLook();
    vCamLook.y = 0.f;
    _float4 vCamRight = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_RIGHT);
    vCamRight.y = 0.f;

    _float4 vDir = pOwner->Get_FollowCamLook();



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
    vDir.y = 0.f;

    pMyTransform->Set_LerpLook(vFinalDir, m_fMyMaxLerp);
    pMyPhysicsCom->Set_Dir(vFinalDir);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);

}

STATE_TYPE CSprint_Jump_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	//pMyPhysicsCom->Set_Accel(m_fMyAccel);

    //Follow_MouseLook(pOwner);

    _float4 vCamLook = pOwner->Get_FollowCamLook();
    vCamLook.y = 0.f;
    vCamLook.Normalize();

    pOwner->Get_Transform()->Set_LerpLook(vCamLook, 0.4f);


    return __super::Tick(pOwner, pAnimator);

}

void CSprint_Jump_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CSprint_Jump_Priest::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* PRIEST가 Sprint로 오는 조건
    1. 쉬프트를 누른 상태에서 점프한다.
    */


    if (KEY(SPACE, TAP))
    {
        return m_eStateType;
    }


   
    return STATE_END;
}
