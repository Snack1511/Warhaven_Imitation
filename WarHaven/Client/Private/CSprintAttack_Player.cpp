#include "stdafx.h"
#include "CSprintAttack_Player.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CSprintAttack_Player::CSprintAttack_Player()
{
}

CSprintAttack_Player::~CSprintAttack_Player()
{
}

CSprintAttack_Player* CSprintAttack_Player::Create()
{
    CSprintAttack_Player* pInstance = new CSprintAttack_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSprintAttack_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSprintAttack_Player::Initialize()
{
	m_eAnimType = ANIM_ATTACK;          // 애니메이션의 메쉬타입
	m_iAnimIndex = 21;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_SPRINTATTACK_PLAYER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 17.f;

	// 선형 보간 시간
	m_fInterPolationTime = 0.f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 2.5f;

	m_iStateChangeKeyFrame = 80;

	m_vecAdjState.push_back(STATE_IDLE_PLAYER_L);
	m_vecAdjState.push_back(STATE_RUN_PLAYER_L);
	m_vecAdjState.push_back(STATE_WALK_PLAYER_L);


    return S_OK;
}

void CSprintAttack_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	//임시
	pMyPhysicsCom->Get_Physics().bAir = false;

	_float4 vCamLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
	vCamLook.y = 0.f;

	//1인자 룩 (안에서 Normalize 함), 2인자 러프에 걸리는 최대시간
	pMyTransform->Set_LerpLook(vCamLook, 0.4f);

	//실제 움직이는 방향
	pMyPhysicsCom->Set_Dir(vCamLook);

	//최대속도 설정
	pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fSprintAttackSpeed);
	pMyPhysicsCom->Set_SpeedasMax();


	//마찰 조절하기
	//*주의 : 사용하고나면 Exit에서 반드시 1로 되돌려주기
	pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 0.65f;
	pMyPhysicsCom->Set_Accel(m_fMyAccel);

	Add_KeyFrame(33, 1);

	//마찰 조절하기
	//*주의 : 사용하고나면 Exit에서 반드시 1로 되돌려주기


    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CSprintAttack_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (33 > pAnimator->Get_CurAnimFrame())
	{
		CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
		pMyPhysicsCom->Set_Accel(m_fMyAccel);
	}
	

    return __super::Tick(pOwner, pAnimator);

}

void CSprintAttack_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
	pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 1.f;
    /* 할거없음 */
}

STATE_TYPE CSprintAttack_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Sprint로 오는 조건
	1. 스프린트 공격 준비가 끝나면
    */

	if (pAnimator->Is_CurAnimFinished())
		return m_eStateType;
   
    return STATE_END;
}

void	CSprintAttack_Player::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 1:
	{
		CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
		pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 0.7f;

	}


	default:
		break;
	}
}
