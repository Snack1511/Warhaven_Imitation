#include "stdafx.h"
#include "CSprint_Jump_Fall_Paladin.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CSprint_Jump_Fall_Paladin::CSprint_Jump_Fall_Paladin()
{
}

CSprint_Jump_Fall_Paladin::~CSprint_Jump_Fall_Paladin()
{
}

CSprint_Jump_Fall_Paladin* CSprint_Jump_Fall_Paladin::Create()
{
    CSprint_Jump_Fall_Paladin* pInstance = new CSprint_Jump_Fall_Paladin();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSprint_Jump_Fall_Paladin");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSprint_Jump_Fall_Paladin::Initialize()
{
    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 47;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SPRINT_JUMPFALL_PALADIN;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;


    m_vecAdjState.push_back(STATE_JUMP_LAND_PALADIN_R);
    m_vecAdjState.push_back(STATE_SPRINT_END_PALADIN);


    return S_OK;
}

void CSprint_Jump_Fall_Paladin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{

    __super::Enter(pOwner, pAnimator, ePrevType, pData);

}

STATE_TYPE CSprint_Jump_Fall_Paladin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    Follow_MouseLook(pOwner);

    return __super::Tick(pOwner, pAnimator);

}

void CSprint_Jump_Fall_Paladin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CSprint_Jump_Fall_Paladin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* PALADIN�� Sprint�� ���� ����
    1. ����Ʈ�� ���� ���¿��� �����Ѵ�.
    */
    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;


   
    return STATE_END;
}
