#include "stdafx.h"
#include "CJump_Player.h"

#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CJump_Player::CJump_Player()
{
}

CJump_Player::~CJump_Player()
{
}

HRESULT CJump_Player::Initialize()
{
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 20.f;


    return S_OK;
}

void CJump_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 20.f;


    /* Owner의 Animator Set Idle로 */

    pOwner->Get_PhysicsCom()->Set_Jump(pOwner->Get_Status().fJumpPower);

	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	_uint iDirection = Get_Direction_Four();

	if (iDirection == STATE_DIRECTION_END)
	{
		m_iAnimIndex = iPlaceJumpAnimIndex;
		m_fAnimSpeed = 1.5f;
	}
	else
	{
		m_iAnimIndex = m_iDirectionAnimIndex[iDirection];
		m_fAnimSpeed = 1.f;
	}


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
        
   
    return __super::Tick(pOwner, pAnimator);
}

void CJump_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fAirFriction = 1.f;
    /* 할거없음 */
}

STATE_TYPE CJump_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Jump로 오는 조건
    1. SpaceBar 를 누른다.
    */



    // 만약 WASD 를 눌렀다면
    if (CUser::Get_Instance()->Get_LastKey() == KEY::SPACE)
    {
		
	
		return m_eStateType;
    }
  


    return STATE_END;
}
