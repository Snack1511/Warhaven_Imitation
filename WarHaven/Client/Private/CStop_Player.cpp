#include "stdafx.h"
#include "CStop_Player.h"

#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CStop_Player::CStop_Player()
{
}

CStop_Player::~CStop_Player()
{
}

HRESULT CStop_Player::Initialize()
{
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 20.f;
	m_fInterPolationTime = 0.05f;
	m_fAnimSpeed = 4.f;

	m_vecAdjState.push_back(STATE_WARRIOR_GUARDBREAK);
	m_vecAdjState.push_back(STATE_WARRIOR_OXEN_BEGIN);

	m_vecAdjState.push_back(STATE_SLIDE_BEGIN_PLAYER);
	m_vecAdjState.push_back(STATE_GUARD_BEGIN_PLAYER);

	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);


	m_iStateChangeKeyFrame = 28;
	
	m_fMaxSpeed = 4.f;


    return S_OK;
}

void CStop_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fAirFriction = 3.f;

	switch (CUser::Get_Instance()->Get_LastKey())
	{
	case KEY::W:

		m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_N];

		break;

	case KEY::A:


		m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_W];

		break;

	case KEY::S:


		m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_S];

		break;

	case KEY::D:


		m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_E];

		break;

	default:

		break;
	}


	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 20.f;



    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CStop_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
        
   
    return __super::Tick(pOwner, pAnimator);
}

void CStop_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fAirFriction = 1.f;
    /* �Ұž��� */
}

STATE_TYPE CStop_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* Player�� Walk�� ���� ����
	1.
	*/

	// m_eStateType �� End �� ���� ������ Enter �� ȣ���Ѵ�.

	// W �� A �� ������ ���� ������ �̵��Ѵ�.
	// W �� D �� ������ ���� ������ �̵��Ѵ�.

	// ���� WASD �� �����ٸ�
	if (
		KEY(W, NONE) &&
		KEY(A, NONE) &&
		KEY(S, NONE) &&
		KEY(D, NONE)
		)
	{
		return m_eStateType;
	}




    return STATE_END;
}
