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

void CJump_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 20.f;


    /* Owner�� Animator Set Idle�� */

    pOwner->Get_PhysicsCom()->Set_Jump(pOwner->Get_Status().fJumpPower);

	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();


	//_float4 vCamLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
	//vCamLook.y = 0.f;

	////1���� �� (�ȿ��� Normalize ��), 2���� ������ �ɸ��� �ִ�ð�
	//pMyTransform->Set_LerpLook(vCamLook, m_fMyMaxLerp);

	////���� �����̴� ����
	//pMyPhysicsCom->Set_Dir(vCamLook);

	////�ִ�ӵ� ����

	//pMyPhysicsCom->Set_MaxSpeed(fSpeed);

	//if (bSpeedasMax)
	//	pMyPhysicsCom->Set_SpeedasMax();


    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CJump_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
        
   
    return __super::Tick(pOwner, pAnimator);
}

void CJump_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fAirFriction = 1.f;
    /* �Ұž��� */
}

STATE_TYPE CJump_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Walk�� ���� ����
    1. 
    */

    // m_eStateType �� End �� ���� ������ Enter �� ȣ���Ѵ�.

    // W �� A �� ������ ���� ������ �̵��Ѵ�.
    // W �� D �� ������ ���� ������ �̵��Ѵ�.

    // ���� WASD �� �����ٸ�
    if (CUser::Get_Instance()->Get_LastKey() == KEY::SPACE)
    {

		switch (Get_Direction_Four())
		{
		case STATE_DIRECTION_N:

			m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_N];

			break;

		case STATE_DIRECTION_W:

			m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_W];

			break;

		case STATE_DIRECTION_S:

			m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_S];

			break;

		case STATE_DIRECTION_E:

			m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_E];

			break;

		default:
			m_iAnimIndex = iPlaceJumpAnimIndex;
			break;
		}

		return m_eStateType;
    }

  
      
      

   


    return STATE_END;
}

_bool	CJump_Player::Jump_W()
{
    // Key(W)
    if (KEY(W, HOLD))
    {
        return true;
    }


    return false;

}

_bool	CJump_Player::Jump_A()
{
    if (KEY(A, HOLD))
    {
        return true;
    }

    return false;

}

_bool	CJump_Player::Jump_S()
{
    // Key(S)
    if (KEY(S, HOLD))
    {
        return true;
    }

    return false;

}

_bool	CJump_Player::Jump_D()
{
    // Key(D)
    if (KEY(D, HOLD))
    {
        return true;
    }

    return false;

}


