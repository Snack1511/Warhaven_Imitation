#include "stdafx.h"
#include "CJump_Player_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CJump_Player_L::CJump_Player_L()
{
}

CJump_Player_L::~CJump_Player_L()
{
}

CJump_Player_L* CJump_Player_L::Create()
{
    CJump_Player_L* pInstance = new CJump_Player_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Player_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CJump_Player_L::Initialize()
{
	__super::Initialize();


    m_eAnimType = ANIM_BASE_L;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 4;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_PLAYER_L; // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;



    m_vecAdjState.push_back(STATE_JUMPFALL_PLAYER_L);


    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 6;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 7;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 8;
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 5;

	iPlaceJumpAnimIndex = 4;


    return S_OK;
}

void CJump_Player_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{

    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CJump_Player_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);

}

void CJump_Player_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CJump_Player_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Check_Condition(pOwner, pAnimator);

}
