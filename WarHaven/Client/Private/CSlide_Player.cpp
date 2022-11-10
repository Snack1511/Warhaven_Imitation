#include "stdafx.h"
#include "CSlide_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CSlide_Player::CSlide_Player()
{
}

CSlide_Player::~CSlide_Player()
{
}

CSlide_Player* CSlide_Player::Create()
{
    CSlide_Player* pInstance = new CSlide_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSlide_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSlide_Player::Initialize()
{
    m_eAnimType = ANIM_ETC;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 32;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SLIDE_PLAYER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_vecAdjState.push_back(STATE_WALK_PLAYER);
    m_vecAdjState.push_back(STATE_RUN_PLAYER);
    //m_vecAdjState.push_back(STATE_IDLE_PLAYER);

    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);


    return S_OK;
}

void CSlide_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    // Enter ���� ���� ���ϰ� ���� State �߰�.


    m_eStateType = STATE_SLIDE_PLAYER;
    m_eAnimType = ANIM_ETC;

    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CSlide_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (KEY(J, NONE))
    {
        if (m_iAnimIndex != 34)
        {
            m_iAnimIndex = 34;

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.f);
        }
        
    }
        

    if (m_iAnimIndex == 34 && pAnimator->Is_CurAnimFinished())
    {
        return STATE_WALK_PLAYER;
    }
        

    if (m_iAnimIndex == 32 && pAnimator->Is_CurAnimFinished())
    {

        if (m_iAnimIndex != 33)
        {
            m_iAnimIndex = 33;

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.f);
        }
    }




    return __super::Tick(pOwner, pAnimator);
}

void CSlide_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CSlide_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Walk�� ���� ����
    1. CTRL �� ������ ���� ����
    2. WASD �� ���� ����
    */

    // m_eStateType �� End �� ���� ������ Enter �� ȣ���Ѵ�.

    // W �� A �� ������ ���� ������ �̵��Ѵ�.
    // W �� D �� ������ ���� ������ �̵��Ѵ�.

    // ���� WASD �� �����ٸ�

    if (KEY(CTRL, NONE))
    {
        if (KEY(Q, NONE))
        {
            if (KEY(J, HOLD) || 
                m_iAnimIndex == 34 )
            {
                return m_eStateType;
            }
        }

    }
    

   


    return STATE_END;
}

