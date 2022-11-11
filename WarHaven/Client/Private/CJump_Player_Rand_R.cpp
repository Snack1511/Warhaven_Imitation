#include "stdafx.h"
#include "CJump_Player_Rand_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CJump_Player_Rand_R::CJump_Player_Rand_R()
{
}

CJump_Player_Rand_R::~CJump_Player_Rand_R()
{
}

CJump_Player_Rand_R* CJump_Player_Rand_R::Create()
{
    CJump_Player_Rand_R* pInstance = new CJump_Player_Rand_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Player_Rand_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CJump_Player_Rand_R::Initialize()
{

    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 17;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_LAND_PLAYER_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 40;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    
    m_vecAdjState.push_back(STATE_IDLE_PLAYER_R);
    m_vecAdjState.push_back(STATE_WALK_PLAYER_R);
    m_vecAdjState.push_back(STATE_RUN_PLAYER_R);

    return S_OK;
}

void CJump_Player_Rand_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CJump_Player_Rand_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CJump_Player_Rand_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CJump_Player_Rand_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Walk�� ���� ����
    1. ���� ����� �� . 
    */

    // �׽�Ʈ��. ���� ���� �� ����ؾ���.
    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;

    return STATE_END;
}
