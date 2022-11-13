#include "stdafx.h"
#include "CSprint_Loop.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CSprint_Loop::CSprint_Loop()
{
}

CSprint_Loop::~CSprint_Loop()
{
}

CSprint_Loop* CSprint_Loop::Create()
{
    CSprint_Loop* pInstance = new CSprint_Loop();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSprint_Loop");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSprint_Loop::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 60;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SPRINT_LOOP_PLAYER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 200.f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_vecAdjState.push_back(STATE_SPRINT_END_PLAYER);
    m_vecAdjState.push_back(STATE_SPRINT_JUMP_PLAYER);
	m_vecAdjState.push_back(STATE_SPRINTATTACK_BEGIN_PLAYER);
    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);


    return S_OK;
}

void CSprint_Loop::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
    if (ePrevType == STATE_SPRINT_BEGIN_PLAYER || ePrevType == STATE_SPRINT_LOOP_PLAYER)
        m_fInterPolationTime = 0.f;

    else
        m_fInterPolationTime = 0.1f;



	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	//임시
	pMyPhysicsCom->Get_Physics().bAir = false;

	_float4 vCamLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
	vCamLook.y = 0.f;

	//1인자 룩 (안에서 Normalize 함), 2인자 러프에 걸리는 최대시간
	pMyTransform->Set_LerpLook(vCamLook, m_fMyMaxLerp);

	//실제 움직이는 방향
	pMyPhysicsCom->Set_Dir(vCamLook);

	//최대속도 설정
	pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fSprintSpeed);
	pMyPhysicsCom->Set_SpeedasMax();

	pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 0.3f;

    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CSprint_Loop::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	_float4 vCamLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);

	_float4 vDir = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);


	_uint iFrame = pAnimator->Get_CurAnimFrame();

		

	vDir.y = 0.f;

	pMyTransform->Set_LerpLook(vCamLook, m_fMyMaxLerp);
	pMyPhysicsCom->Set_Dir(vDir);
	pMyPhysicsCom->Set_Accel(m_fMyAccel);



    return __super::Tick(pOwner, pAnimator);

}

void CSprint_Loop::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
	pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 1.f;
    /* 할거없음 */
}

STATE_TYPE CSprint_Loop::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Sprint로 오는 조건
    1. 쉬프트를 누른 상태에서 움직인다.
    */
   

    if (pAnimator->Is_CurAnimFinished() && KEY(LSHIFT, HOLD))
        return m_eStateType;

   
    return STATE_END;
}
