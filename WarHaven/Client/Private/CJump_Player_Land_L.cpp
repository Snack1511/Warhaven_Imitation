#include "CJump_Player_Land_L.h"
#include "CState.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CJump_Player_Land_L::CJump_Player_Land_L()
{
}

CJump_Player_Land_L::~CJump_Player_Land_L()
{
}

CJump_Player_Land_L* CJump_Player_Land_L::Create()
{
    CJump_Player_Land_L* pInstance = new CJump_Player_Land_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Player_Land_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CJump_Player_Land_L::Initialize()
{

    m_eAnimType = ANIM_BASE_L;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 9;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_LAND_PLAYER_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 40;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    
    m_vecAdjState.push_back(STATE_IDLE_PLAYER_L);
    m_vecAdjState.push_back(STATE_WALK_PLAYER_L);
    m_vecAdjState.push_back(STATE_RUN_PLAYER_L);

    return S_OK;
}

void CJump_Player_Land_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CJump_Player_Land_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CJump_Player_Land_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CJump_Player_Land_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Walk�� ���� ����
    1. ���� ����� �� . 
    */

    // �׽�Ʈ��. ���� ���� �� ����ؾ���.
    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;

    return STATE_END;
}
