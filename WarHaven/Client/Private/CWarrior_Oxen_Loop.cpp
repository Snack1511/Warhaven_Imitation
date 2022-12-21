#include "stdafx.h"
#include "CWarrior_Oxen_Loop.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

CWarrior_Oxen_Loop::CWarrior_Oxen_Loop()
{
}

CWarrior_Oxen_Loop::~CWarrior_Oxen_Loop()
{
}

CWarrior_Oxen_Loop* CWarrior_Oxen_Loop::Create()
{
    CWarrior_Oxen_Loop* pInstance = new CWarrior_Oxen_Loop();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Oxen_Loop");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarrior_Oxen_Loop::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 25;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WARRIOR_OXEN_LOOP;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 3.f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 2;
    //m_vecAdjState.push_back(STATE_WALK_PLAYER);
    //m_vecAdjState.push_back(STATE_RUN_PLAYER);
    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);

    m_vecAdjState.push_back(STATE_WARRIOR_OXEN_LOOPATTACK);

    m_eBounceState = STATE_BOUNCE_PLAYER_R;

    m_iEndFinishedFrame = 3;

    return S_OK;
}

void CWarrior_Oxen_Loop::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    pOwner->TurnOn_TrailEffect(true);

    _float4 vPos = pOwner->Get_Transform()->Get_World(WORLD_POS);

    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Crow_Charge", pOwner, vPos);

    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarrior_Oxen_Loop::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    if (pAnimator->Is_CurAnimFinished())
    {
        ++m_iCurFinishedFrame;

        if (m_iCurFinishedFrame == m_iEndFinishedFrame)
        {
            m_iCurFinishedFrame = 0;
            return STATE_WARRIOR_OXEN_END;
        }
    }

    // Ȳ�� ����� ������ ���� ���� �͵��� �־������.
    // �⺻������ ������ ������ ������ �ϱ� ������ Base_L �� �ٲ��ָ��.


    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Oxen_Loop::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
    pOwner->TurnOn_TrailEffect(false);

}

STATE_TYPE CWarrior_Oxen_Loop::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Oxen ���� ���� ����
    1.  Q �� �̿��� �����Ѵ�.
        */

    return STATE_END;
}