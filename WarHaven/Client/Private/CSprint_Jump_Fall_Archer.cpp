#include "stdafx.h"
#include "CSprint_Jump_Fall_Archer.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CSprint_Jump_Fall_Archer::CSprint_Jump_Fall_Archer()
{
}

CSprint_Jump_Fall_Archer::~CSprint_Jump_Fall_Archer()
{
}

CSprint_Jump_Fall_Archer* CSprint_Jump_Fall_Archer::Create()
{
    CSprint_Jump_Fall_Archer* pInstance = new CSprint_Jump_Fall_Archer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSprint_Jump_Fall_Archer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSprint_Jump_Fall_Archer::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 49;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SPRINT_JUMPFALL_ARCHER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;

    m_vecAdjState.push_back(STATE_GLIDING);
    m_vecAdjState.push_back(STATE_JUMP_LAND_ARCHER_R);
    m_vecAdjState.push_back(STATE_SPRINT_END_ARCHER);


    return S_OK;
}

void CSprint_Jump_Fall_Archer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{

    if (ePrevType == STATE_GLIDING)
        m_fInterPolationTime = 0.2f;


    __super::Enter(pOwner, pAnimator, ePrevType, pData);

}

STATE_TYPE CSprint_Jump_Fall_Archer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (!pOwner->Is_Air())
    {
        STATE_TYPE eSprintEndState = pOwner->Get_SprintEndState();
        return eSprintEndState;
    }


    Follow_MouseLook(pOwner);

    return __super::Tick(pOwner, pAnimator);

}

void CSprint_Jump_Fall_Archer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CSprint_Jump_Fall_Archer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* ARCHER�� Sprint�� ���� ����
    1. ����Ʈ�� ���� ���¿��� �����Ѵ�.
    */
    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;


   
    return STATE_END;
}
