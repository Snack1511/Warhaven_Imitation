#include "stdafx.h"
#include "CSprint_Loop_Qanda.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"
#include "CUnit_Qanda.h"

#include "CAnimWeapon_Crow.h"

#include "CUser.h"


CSprint_Loop_Qanda::CSprint_Loop_Qanda()
{
}

CSprint_Loop_Qanda::~CSprint_Loop_Qanda()
{
}

CSprint_Loop_Qanda* CSprint_Loop_Qanda::Create()
{
	CSprint_Loop_Qanda* pInstance = new CSprint_Loop_Qanda();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CSprint_Loop_Qanda");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CSprint_Loop_Qanda::Initialize()
{
	m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
	m_iAnimIndex = 36;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_SPRINT_LOOP_QANDA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_iStateChangeKeyFrame = 0;

	// 선형 보간 시간
	m_fInterPolationTime = 0.f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 2.15f;

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 200.f;

	// Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

	Add_KeyFrame(10, 0, true);
	Add_KeyFrame(27, 0, true);
	Add_KeyFrame(43, 0, true);
	Add_KeyFrame(60, 0, true);

	//enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
	m_vecAdjState.push_back(STATE_SPRINT_END_QANDA);
	m_vecAdjState.push_back(STATE_SPRINT_JUMP_QANDA);

	return S_OK;
}

void CSprint_Loop_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	if (ePrevType == STATE_SPRINT_BEGIN_QANDA || ePrevType == STATE_SPRINT_LOOP_QANDA)
		m_fInterPolationTime = 0.f;

	else
		m_fInterPolationTime = 0.1f;


	m_fMaxSpeed = pOwner->Get_Status().fSprintSpeed;

	CAnimWeapon_Crow* pAnimCrow = static_cast<CUnit_Qanda*>(pOwner)->Get_Crow();

	if (pAnimCrow->Get_Phase() == CAnimWeapon_Crow::ePhyxState::eIDLE)
	{
		static_cast<CUnit_Qanda*>(pOwner)->Set_CrowAnimIndex(5, 0.f, m_fAnimSpeed);
		static_cast<CUnit_Qanda*>(pOwner)->Get_Crow()->Set_PhiysicsSpeed(m_fMaxSpeed);

	}

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

STATE_TYPE CSprint_Loop_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pOwner->Is_Air())
		return STATE_SPRINT_JUMPFALL_QANDA;

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

void CSprint_Loop_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
	pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 1.f;

	CAnimWeapon_Crow* pAnimCrow = static_cast<CUnit_Qanda*>(pOwner)->Get_Crow();

	if (pAnimCrow->Get_Phase() == CAnimWeapon_Crow::ePhyxState::eIDLE)
	{
		static_cast<CUnit_Qanda*>(pOwner)->Set_CrowAnimIndex(10, 0.1f, m_fAnimSpeed);
		static_cast<CUnit_Qanda*>(pOwner)->Get_Crow()->Set_PhiysicsSpeed(4.f);

	}
}

STATE_TYPE CSprint_Loop_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* QANDA가 Sprint로 오는 조건
	1. 쉬프트를 누른 상태에서 움직인다.
	*/


	if (pAnimator->Is_CurAnimFinished())
		return m_eStateType;


	return STATE_END;
}

void CSprint_Loop_Qanda::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

	switch (iSequence)
	{
	case 0:
		Play_Sound(L"Env_FootStepGround", CHANNEL_ENVIRONMENT, 0.4f);
		break;
	}
}