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

	fOxenJumpPower = 1.3f;

	m_vecAdjState.push_back(STATE_CHANGE_PLAYER);
	m_vecAdjState.push_back(STATE_GLIDING);


    return S_OK;
}

void CJump_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	m_ePreStateType = ePrevType;

	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;


    /* Owner의 Animator Set Idle로 */

    

	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	_uint iDirection = Get_Direction();

	if (ePrevType == STATE_WARRIOR_OXEN_LOOPATTACK)
	{
		m_iAnimIndex = iPlaceJumpAnimIndex;
		pOwner->Get_PhysicsCom()->Set_Jump(pOwner->Get_Status().fJumpPower * fOxenJumpPower);
		pOwner->Get_PhysicsCom()->Set_MaxSpeed(0.f);
		pOwner->Get_PhysicsCom()->Set_SpeedasMax();
	}
	else
	{
		if (iDirection == STATE_DIRECTION_END)
		{
			m_iAnimIndex = iPlaceJumpAnimIndex;
			m_fAnimSpeed = 1.5f;
		}
		else
		{
			m_iAnimIndex = m_iDirectionAnimIndex[iDirection];
			m_fAnimSpeed = 1.5f;

			DoMove(iDirection, pOwner);

			if (pOwner->Get_PhysicsCom()->Get_Physics().fSpeed <= 1.f)
			{
				pOwner->Get_PhysicsCom()->Set_MaxSpeed(m_fMaxSpeed);
				pOwner->Get_PhysicsCom()->Set_SpeedasMax();

			}


		}

		pOwner->Get_PhysicsCom()->Set_Jump(pOwner->Get_Status().fJumpPower);
	}




    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	Follow_MouseLook(pOwner);
   
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
	if (KEY(SPACE, TAP))
	{
		return m_eStateType;
    }
  


    return STATE_END;
}
