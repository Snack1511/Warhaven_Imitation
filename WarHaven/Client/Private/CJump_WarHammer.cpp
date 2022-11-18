#include "stdafx.h"
#include "CJump_WarHammer.h"

#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CJump_WarHammer::CJump_WarHammer()
{
}

CJump_WarHammer::~CJump_WarHammer()
{
}

HRESULT CJump_WarHammer::Initialize()
{
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 20.f;

    return S_OK;
}

void CJump_WarHammer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed * 2.f;


    /* Owner의 Animator Set Idle로 */

    pOwner->Get_PhysicsCom()->Set_Jump(pOwner->Get_Status().fJumpPower);

	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

	_uint iDirection = Get_Direction();

	if (iDirection == STATE_DIRECTION_END)
	{
		m_iAnimIndex = iPlaceJumpAnimIndex;
		m_fAnimSpeed = 1.5f;
	}
	else
	{
		m_iAnimIndex = m_iDirectionAnimIndex[iDirection];
		m_fAnimSpeed = 1.5f;

		Move(iDirection, pOwner);

		if (pOwner->Get_PhysicsCom()->Get_Physics().fSpeed <= 1.f)
		{
			pOwner->Get_PhysicsCom()->Set_MaxSpeed(m_fMaxSpeed);
			pOwner->Get_PhysicsCom()->Set_SpeedasMax();

		}


	}


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_WarHammer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	Follow_MouseLook(pOwner);
   
    return __super::Tick(pOwner, pAnimator);
}

void CJump_WarHammer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fAirFriction = 1.f;
    /* 할거없음 */
}

STATE_TYPE CJump_WarHammer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
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
