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

    return S_OK;
}

void CJump_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
    /* Owner�� Animator Set Idle�� */


    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CJump_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (!pOwner->Get_PhysicsCom()->Get_Physics().bAir)
        return STATE_JUMP_LAND_PLAYER_R;

    
        
   
    return __super::Tick(pOwner, pAnimator);


    
}

void CJump_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
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
        if (Jump_W())
            m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_N];

        else if (Jump_A())
            m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_W];

        else if (Jump_S())
            m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_S];

        else if (Jump_D())
            m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_E];


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


