#include "stdafx.h"
#include "CRun_Player.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CRun_Player::CRun_Player()
{
}

CRun_Player::~CRun_Player()
{
}

HRESULT CRun_Player::Initialize()
{
	//m_eAnimDivide = ANIM_DIVIDE::eBODYLOWER;

    m_vecAdjState.push_back(STATE_WARRIOR_GUARDBREAK);
    m_vecAdjState.push_back(STATE_WARRIOR_OXEN_BEGIN);
	m_vecAdjState.push_back(STATE_GUARD_BEGIN_PLAYER);

	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);
	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PLAYER);

	m_vecAdjState.push_back(STATE_CANNON_PLAYER);
	m_vecAdjState.push_back(STATE_REVIVE_PLAYER);

	m_vecAdjState.push_back(STATE_CHANGE_PLAYER);

	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;

    m_iStateChangeKeyFrame = 0;

	m_fInterPolationTime = 0.1f;
    return S_OK;
}

void CRun_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 10.f;



	//CTransform* pMyTransform = pOwner->Get_Transform();
	//CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
	//

	//_float4 vCamLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
	//vCamLook.y = 0.f;

	////1인자 룩 (안에서 Normalize 함), 2인자 러프에 걸리는 최대시간
	//pMyTransform->Set_LerpLook(vCamLook, m_fMyMaxLerp);

	////실제 움직이는 방향
	//pMyPhysicsCom->Set_Dir(vCamLook);

	////최대속도 설정
	//pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fRunSpeed);
	//pMyPhysicsCom->Set_SpeedasMax();

    
	if (ePrevType == STATE_RUN_PLAYER_R || ePrevType == STATE_RUN_PLAYER_L)
	{

		m_fInterPolationTime = 0.f;
		//pAnimator->Set_CurFrame(22);
	}


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRun_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	_uint iDirection = Move_Direction_Loop(pOwner, pAnimator, 0.1f);

	if (iDirection < 8)
		m_iCurDirection = iDirection;

    return __super::Tick(pOwner, pAnimator);

}

void CRun_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CRun_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Run로 오는 조건
    1. 뛰어간다.
    */

	if (KEY(C, NONE))
	{
		// 걸어간다.
		if (KEY(W, HOLD) ||
			KEY(A, HOLD) ||
			KEY(S, HOLD) ||
			KEY(D, HOLD))
		{

			return m_eStateType;
		}
		
	}


    return STATE_END;
}
