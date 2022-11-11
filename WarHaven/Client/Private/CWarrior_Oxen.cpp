#include "stdafx.h"
#include "CWarrior_Oxen.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

CWarrior_Oxen::CWarrior_Oxen()
{
}

CWarrior_Oxen::~CWarrior_Oxen()
{
}

CWarrior_Oxen* CWarrior_Oxen::Create()
{
    CWarrior_Oxen* pInstance = new CWarrior_Oxen();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Oxen");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarrior_Oxen::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 24;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_PLAYER_SKILL1;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 84;
    //m_vecAdjState.push_back(STATE_WALK_PLAYER);
    //m_vecAdjState.push_back(STATE_RUN_PLAYER);
    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);


    m_iCancelAnimIndex = 22;

    m_iPlayAnimIndex = 23;

    m_iLoopAnimIndex = 25;
    m_iBeginAnimIndex = 24;

    m_iEndFinishedFrame = 3;

    return S_OK;
}

void CWarrior_Oxen::Enter(CUnit* pOwner, CAnimator* pAnimator, _uint iPreAnimIndex)
{
    pOwner->TurnOn_TrailEffect(true);

    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, iPreAnimIndex);

    // CUser::Get_Instance()->Turn_BloodOverLay();
}

STATE_TYPE CWarrior_Oxen::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    // 9 : ��� ����(R Attack)
    // 23 : Ȳ�� ����(SKill)
    //Create_SwordEffect(pOwner);
    

    // Ȳ�Һ��� ������
    if (m_iAnimIndex == m_iPlayAnimIndex)
    {
        m_iStateChangeKeyFrame = 87;

        if (pAnimator->Is_CurAnimFinished())
            return STATE_IDLE_PLAYER_L;
    }

    // Ȳ�� ���� ĵ��
    if (m_iAnimIndex == m_iCancelAnimIndex)
    {
        m_iStateChangeKeyFrame = 20;

        if (pAnimator->Is_CurAnimFinished())
            return STATE_IDLE_PLAYER_R;
    }

    // Ȳ�� ���� ��
    if (m_iAnimIndex == m_iBeginAnimIndex)
    {
        if (KEY(Q, NONE))
        {
            Play_Skill(m_iPlayAnimIndex, 0.1f, pOwner, pAnimator);
        }

        else if(pAnimator->Is_CurAnimFinished())
        {
            m_fInterPolationTime = 0.f;

            Change_Animation(m_iAnimIndex, m_iLoopAnimIndex, pOwner, pAnimator);
        }
    }

    // Ȳ�� ���� Loop
    if (m_iAnimIndex == m_iLoopAnimIndex)
    {
        if (KEY(Q, NONE))
        {
            Play_Skill(m_iPlayAnimIndex, 0.1f, pOwner, pAnimator);
        }
        

        if (pAnimator->Is_CurAnimFinished())
        {
            ++m_iCurFinishedFrame;

            if (m_iCurFinishedFrame == m_iEndFinishedFrame)
            {
                m_iCurFinishedFrame = 0;
                Change_Animation(m_iAnimIndex, m_iCancelAnimIndex, pOwner, pAnimator);
                return __super::Tick(pOwner, pAnimator);
                
            }

            m_fInterPolationTime = 0.f;

            Change_Animation(m_iAnimIndex, m_iLoopAnimIndex, pOwner, pAnimator);
        }

        
    }

    // Ȳ�� ����� ������ ���� ���� �͵��� �־������.
    // �⺻������ ������ ������ ������ �ϱ� ������ Base_L �� �ٲ��ָ��.


    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Oxen::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
    pOwner->TurnOn_TrailEffect(false);

}

STATE_TYPE CWarrior_Oxen::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Oxen ���� ���� ����
    1.  Q �� �̿��� �����Ѵ�.
        */

    if (KEY(Q, TAP))
        return m_eStateType;

    return STATE_END;
}

void CWarrior_Oxen::Play_Skill(_uint iChangeIndex, _float fInterPolationTime, CUnit* pOwner, CAnimator* pAnimator)
{
    m_fInterPolationTime = fInterPolationTime;

    Change_Animation(m_iAnimIndex, m_iPlayAnimIndex, pOwner, pAnimator);
}


