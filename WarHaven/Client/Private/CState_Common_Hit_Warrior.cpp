#include "stdafx.h"
#include "CState_Common_Hit_Warrior.h"

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

CState_Common_Hit_Warrior::CState_Common_Hit_Warrior()
{
}

CState_Common_Hit_Warrior::~CState_Common_Hit_Warrior()
{
}

CState_Common_Hit_Warrior* CState_Common_Hit_Warrior::Create()
{
    CState_Common_Hit_Warrior* pInstance = new CState_Common_Hit_Warrior();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_Hit_Warrior");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CState_Common_Hit_Warrior::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_HIT;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = m_iHitIndex[0];                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_HIT_WARRIOR;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.

    m_iStateChangeKeyFrame = 20;

    m_fAnimSpeed = 2.f;
    
    return S_OK;
}

void CState_Common_Hit_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    /* �� �������� hit info�� �޾Ҵ�. */
    m_tHitInfo = *((HIT_INFO*)(pData));
    __super::Hit_State(pOwner);


    m_iRand = random(0, 2);


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Hit_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
    {
        STATE_TYPE eDefaultState = pOwner->Get_DefaultState();
        return eDefaultState;
    }

    if (pAnimator->Get_CurAnimFrame() > m_iStateChangeKeyFrame)
    {
        switch (m_iRand)
        {
        case 0:

            return AI_STATE_COMBAT_GUARDDASH_WARRIOR;

        case 1:

            return AI_STATE_COMBAT_HORIZONTALMIDDLE_WARRIOR_R;


        case 2:

            return AI_STATE_COMBAT_GUARDBEGIN_WARRIOR;


        default:
            break;
        }
        
    }


    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Hit_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Common_Hit_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
