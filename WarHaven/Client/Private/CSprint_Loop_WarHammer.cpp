#include "stdafx.h"
#include "CSprint_Loop_WarHammer.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CSprint_Loop_WarHammer::CSprint_Loop_WarHammer()
{
}

CSprint_Loop_WarHammer::~CSprint_Loop_WarHammer()
{
}

CSprint_Loop_WarHammer* CSprint_Loop_WarHammer::Create()
{
	CSprint_Loop_WarHammer* pInstance = new CSprint_Loop_WarHammer();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CSprint_Loop_WarHammer");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CSprint_Loop_WarHammer::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
	m_iAnimIndex = 58;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_SPRINT_LOOP_WARHAMMER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_iStateChangeKeyFrame = 0;

	// 선형 보간 시간
	m_fInterPolationTime = 0.f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 1.8f;

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 200.f;

	// Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

	//enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
	m_vecAdjState.push_back(STATE_SPRINT_END_WARHAMMER);
	m_vecAdjState.push_back(STATE_SPRINT_JUMP_WARHAMMER);
	m_vecAdjState.push_back(STATE_SPRINTATTACK_BEGIN_WARHAMMER);


	m_vecAdjState.push_back(STATE_GROGGYATTACK_WARHAMMER);
	m_vecAdjState.push_back(STATE_AIRSPIKE_BEGIN_WARHAMMER);

	Add_KeyFrame(12, 0, true);
	Add_KeyFrame(28, 0, true);
	Add_KeyFrame(45, 0, true);
	Add_KeyFrame(60, 0, true);

	return S_OK;
}

void CSprint_Loop_WarHammer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	if (ePrevType == STATE_SPRINT_BEGIN_WARHAMMER || ePrevType == STATE_SPRINT_LOOP_WARHAMMER)
		m_fInterPolationTime = 0.f;

	else
		m_fInterPolationTime = 0.1f;



	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	_float4 vCamLook = pOwner->Get_FollowCamLook();
	vCamLook.y = 0.f;

	//1인자 룩 (안에서 Normalize 함), 2인자 러프에 걸리는 최대시간
	pMyTransform->Set_LerpLook(vCamLook, m_fMyMaxLerp);

	//실제 움직이는 방향
	pMyPhysicsCom->Set_Dir(vCamLook);

	//최대속도 설정
	pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fSprintSpeed);
	pMyPhysicsCom->Set_SpeedasMax();

	pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 0.3f;

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSprint_Loop_WarHammer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pOwner->Is_Air())
		return STATE_SPRINT_JUMPFALL_WARHAMMER;

	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

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
	vDir.y = 0.f;

	pMyTransform->Set_LerpLook(vFinalDir, m_fMyMaxLerp);
	_float4 vUnitLook = pMyTransform->Get_MyWorld(WORLD_LOOK);
	pMyPhysicsCom->Set_Dir(vUnitLook);
	pMyPhysicsCom->Set_Accel(m_fMyAccel);



	return __super::Tick(pOwner, pAnimator);

}

void CSprint_Loop_WarHammer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
	pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 1.f;
	/* 할거없음 */
}

STATE_TYPE CSprint_Loop_WarHammer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* WARHAMMER가 Sprint로 오는 조건
	1. 쉬프트를 누른 상태에서 움직인다.
	*/


	if (pAnimator->Is_CurAnimFinished())
		return m_eStateType;


	return STATE_END;
}

void CSprint_Loop_WarHammer::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:
		Play_Sound(L"Env_FootStepGround", CHANNEL_EFFECTS);
		break;

	default:
		break;
	}


}