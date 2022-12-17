#include "stdafx.h"
#include "CSlide_Loop_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "Physics.h"

CSlide_Loop_Player::CSlide_Loop_Player()
{
}

CSlide_Loop_Player::~CSlide_Loop_Player()
{
}

CSlide_Loop_Player* CSlide_Loop_Player::Create()
{
    CSlide_Loop_Player* pInstance = new CSlide_Loop_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSlide_Loop_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSlide_Loop_Player::Initialize()
{
    m_eAnimType = ANIM_ETC;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 33;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SLIDE_LOOP_PLAYER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    //m_vecAdjState.push_back(STATE_SLIDE_END_PLAYER);


    return S_OK;
}

void CSlide_Loop_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    if (ePrevType == STATE_SLIDE_BEGIN_PLAYER)
        m_fInterPolationTime = 0.f;

    else
        m_fInterPolationTime = 0.1f;

    pOwner->Get_PhysicsCom()->Set_MaxSpeed(10.f);


    __super::Enter(pOwner, pAnimator, ePrevType, pData);

}

STATE_TYPE CSlide_Loop_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    // ��ȣ�ۿ� �ؾ���.

    pOwner->Get_PhysicsCom()->Set_Accel(30.f);


    return __super::Tick(pOwner, pAnimator);
}

void CSlide_Loop_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CSlide_Loop_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Walk�� ���� ����
    1. CTRL �� ������ ���� ����
    2. WASD �� ���� ����
    */
    
    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;

   


    return STATE_END;
}

