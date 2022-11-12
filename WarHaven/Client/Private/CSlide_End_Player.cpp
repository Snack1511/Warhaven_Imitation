#include "stdafx.h"
#include "CSlide_End_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CSlide_End_Player::CSlide_End_Player()
{
}

CSlide_End_Player::~CSlide_End_Player()
{
}

CSlide_End_Player* CSlide_End_Player::Create()
{
    CSlide_End_Player* pInstance = new CSlide_End_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSlide_End_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSlide_End_Player::Initialize()
{
    m_eAnimType = ANIM_ETC;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 34;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SLIDE_END_PLAYER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 19;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_vecAdjState.push_back(STATE_SPRINT_JUMPFALL_PLAYER);


    return S_OK;
}

void CSlide_End_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
    if (ePrevType == STATE_SLIDE_LOOP_PLAYER)
        m_fInterPolationTime = 0.f;

    else
        m_fInterPolationTime = 0.1f;


    __super::Enter(pOwner, pAnimator, ePrevType);

}

STATE_TYPE CSlide_End_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CSlide_End_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
  
}

STATE_TYPE CSlide_End_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;
   

    return STATE_END;
}

