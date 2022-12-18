#include "stdafx.h"
#include "CJump_Paladin.h"

#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CJump_Paladin::CJump_Paladin()
{
}

CJump_Paladin::~CJump_Paladin()
{
}

HRESULT CJump_Paladin::Initialize()
{
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 20.f;

	m_vecAdjState.push_back(STATE_CHANGE_PLAYER);

	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT_PALADIN);

    return S_OK;
}

void CJump_Paladin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;


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

		DoMove(iDirection, pOwner);

		pOwner->Get_PhysicsCom()->Set_MaxSpeed(m_fMaxSpeed);
		pOwner->Get_PhysicsCom()->Set_SpeedasMax();

	}


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_Paladin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	Follow_MouseLook(pOwner);
   
    return __super::Tick(pOwner, pAnimator);
}

void CJump_Paladin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fAirFriction = 1.f;
    /* 할거없음 */
}

STATE_TYPE CJump_Paladin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
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
