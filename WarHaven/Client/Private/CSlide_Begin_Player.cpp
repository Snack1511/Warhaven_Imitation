#include "stdafx.h"
#include "CSlide_Begin_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CSlide_Begin_Player::CSlide_Begin_Player()
{
}

CSlide_Begin_Player::~CSlide_Begin_Player()
{
}

CSlide_Begin_Player* CSlide_Begin_Player::Create()
{
    CSlide_Begin_Player* pInstance = new CSlide_Begin_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSlide_Begin_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSlide_Begin_Player::Initialize()
{
    m_eAnimType = ANIM_ETC;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 32;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SLIDE_BEGIN_PLAYER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;


    m_vecAdjState.push_back(STATE_SLIDE_LOOP_PLAYER);


    return S_OK;
}

void CSlide_Begin_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{

    __super::Enter(pOwner, pAnimator, ePrevType);

}

STATE_TYPE CSlide_Begin_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CSlide_Begin_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CSlide_Begin_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    if (KEY(J, TAP))
    {
        return m_eStateType;
    }

    return STATE_END;
}

