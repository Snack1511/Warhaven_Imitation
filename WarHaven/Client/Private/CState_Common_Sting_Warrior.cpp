#include "stdafx.h"
#include "CState_Common_Sting_Warrior.h"

#include "GameInstance.h"
#include "Physics.h"
#include "Functor.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "Transform.h"
#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"
#include "Model.h"
#include "CColorController.h"

#include "CUtility_Transform.h"

CState_Common_Sting_Warrior::CState_Common_Sting_Warrior()
{
}

CState_Common_Sting_Warrior::~CState_Common_Sting_Warrior()
{
}

CState_Common_Sting_Warrior* CState_Common_Sting_Warrior::Create()
{
    CState_Common_Sting_Warrior* pInstance = new CState_Common_Sting_Warrior();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_Sting_Warrior");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CState_Common_Sting_Warrior::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_HIT;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = m_iHitStingIndex[0];                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_STINGHIT_WARRIOR;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;
    m_bSting = true;
    
    return S_OK;
}

void CState_Common_Sting_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{

    /* �� �������� hit info�� �޾Ҵ�. */
    m_tHitInfo = *((HIT_INFO*)(pData));
    __super::Sting_State(pOwner);


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Sting_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    //if(eStingHitState


    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Sting_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Common_Sting_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    return STATE_END;
}
