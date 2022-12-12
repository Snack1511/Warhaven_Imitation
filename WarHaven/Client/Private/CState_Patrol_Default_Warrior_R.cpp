#include "stdafx.h"
#include "CState_Patrol_Default_Warrior_R.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"

#include "CColorController.h"

#include "UsefulHeaders.h"

CState_Patrol_Default_Warrior_R::CState_Patrol_Default_Warrior_R()
{
}

CState_Patrol_Default_Warrior_R::~CState_Patrol_Default_Warrior_R()
{
}

CState_Patrol_Default_Warrior_R* CState_Patrol_Default_Warrior_R::Create()
{
    CState_Patrol_Default_Warrior_R* pInstance = new CState_Patrol_Default_Warrior_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Patrol_Default_Warrior_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Patrol_Default_Warrior_R::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 11;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATROL_DEAFULT_WARRIOR_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iIdleDelay = 10;

    return S_OK;
}

void CState_Patrol_Default_Warrior_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    if (ePrevType == AI_STATE_PATROL_WARRIOR_SWITCH_L_TO_R)
    {
        m_iIdleDelay = 2;
        m_fInterPolationTime = 0.f;
    }
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Patrol_Default_Warrior_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_iRand == 0)
    {
        if (pAnimator->Get_CurAnimFrame() > m_iIdleDelay)
        {
            if (m_iRand == 1)
                return AI_STATE_PATROL_WARRIOR_SWITCH_R_TO_L;
            else
                return AI_STATE_PATROL_WALK_WARRIOR_R;
        }
    }
    else
    {
        if (m_iRand == 1)
            return AI_STATE_PATROL_WARRIOR_SWITCH_R_TO_L;
        else
            return AI_STATE_PATROL_WALK_WARRIOR_R;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CState_Patrol_Default_Warrior_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
    /* �Ұž��� */
}

STATE_TYPE CState_Patrol_Default_Warrior_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
