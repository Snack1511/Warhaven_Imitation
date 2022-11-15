#include "stdafx.h"
#include "CJump_SpearMan_Fall_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CJump_SpearMan_Fall_L::CJump_SpearMan_Fall_L()
{
}

CJump_SpearMan_Fall_L::~CJump_SpearMan_Fall_L()
{
}

CJump_SpearMan_Fall_L* CJump_SpearMan_Fall_L::Create()
{
    CJump_SpearMan_Fall_L* pInstance = new CJump_SpearMan_Fall_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_SpearMan_Fall_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CJump_SpearMan_Fall_L::Initialize()
{



    m_eAnimType = ANIM_BASE_L;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 0;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMPFALL_SPEARMAN_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;


    m_vecAdjState.push_back(STATE_JUMP_LAND_SPEARMAN_L);




    return S_OK;
}

void CJump_SpearMan_Fall_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{

    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CJump_SpearMan_Fall_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (!pOwner->Is_Air())
        return STATE_JUMP_LAND_SPEARMAN_L;

   return __super::Tick(pOwner, pAnimator);
}

void CJump_SpearMan_Fall_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CJump_SpearMan_Fall_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* SPEARMAN�� Walk�� ���� ����
    1. Jump �� ������.
    */

    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;


    return STATE_END;
}
