#include "stdafx.h"
#include "CRun_AI_TG_Warrior_Begin_L.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CColorController.h"

CRun_AI_TG_Warrior_Begin_L::CRun_AI_TG_Warrior_Begin_L()
{
}

CRun_AI_TG_Warrior_Begin_L::~CRun_AI_TG_Warrior_Begin_L()
{
}

CRun_AI_TG_Warrior_Begin_L* CRun_AI_TG_Warrior_Begin_L::Create()
{
    CRun_AI_TG_Warrior_Begin_L* pInstance = new CRun_AI_TG_Warrior_Begin_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_AI_TG_Warrior_Begin_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CRun_AI_TG_Warrior_Begin_L::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_BASE_L;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 17;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_RUNBEGIN_WARRIOR_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_vecAdjState.push_back(AI_STATE_ATTACK_HORIZONTALMIDDLE_L);


    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 10;

    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 11;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 12;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 13;

    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 14;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 15;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 16;

    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 17;
    
    m_iAINextState = AI_STATE_ATTACK_HORIZONTALMIDDLE_L;

    return S_OK;
}

void CRun_AI_TG_Warrior_Begin_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRun_AI_TG_Warrior_Begin_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{



    

    if (pAnimator->Is_CurAnimFinished())
    {
        if (m_bAIMove)
            return AI_STATE_RUN_WARRIOR_L;
        else
            return AI_STATE_RUNBEGIN_WARRIOR_R;
    }


    return __super::Tick(pOwner, pAnimator);
}

void CRun_AI_TG_Warrior_Begin_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CRun_AI_TG_Warrior_Begin_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    //if (KEY(LSHIFT, NONE))
    //{
    //    if (
    //        KEY(W, HOLD) ||
    //        KEY(A, HOLD) ||
    //        KEY(S, HOLD) ||
    //        KEY(D, HOLD)
    //        )
    //    {
    //        return m_eStateType;
    //    }
    //}
   
    return STATE_END;
}