#include "stdafx.h"
#include "CWarrior_Attack_01.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWarrior_Attack_01::CWarrior_Attack_01()
{
}

CWarrior_Attack_01::~CWarrior_Attack_01()
{
}

CWarrior_Attack_01* CWarrior_Attack_01::Create()
{
    CWarrior_Attack_01* pInstance = new CWarrior_Attack_01();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Attack_01");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarrior_Attack_01::Initialize()
{
    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 0;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_WARRIOR;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 84;
    m_vecAdjState.push_back(STATE_IDLE_PLAYER);
    m_vecAdjState.push_back(STATE_WALK_PLAYER);
    m_vecAdjState.push_back(STATE_RUN_PLAYER);
    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);


    return S_OK;
}

void CWarrior_Attack_01::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    // Ȳ�� ����
    if (m_iAnimIndex == 24)
    {
        m_eAnimType = ANIM_ATTACK;
        __super::Enter(pOwner, pAnimator);
        return;
    }

    // ���� �극��ũ
    if(m_iAnimIndex == 15)
    {
        m_eAnimType = ANIM_ATTACK;
        __super::Enter(pOwner, pAnimator);
        return;
    }

  
    if (m_eAnimType == ANIM_BASE_R)
    {
        // ���
        if (m_iAnimIndex == 13)
        {
            m_eAnimType = ANIM_ATTACK;
            __super::Enter(pOwner, pAnimator);
            return;
        }
    }

    
    if (m_eAnimType == ANIM_BASE_L)
    {
        // ���
        if (m_iAnimIndex == 12)
        {
            m_eAnimType = ANIM_ATTACK;
            __super::Enter(pOwner, pAnimator);
            return;
        }
    }
   
    


    MOUSEMOVE eMouseMove = MMS_X;
    _long iX = MOUSE_MOVE(eMouseMove);

    eMouseMove = MMS_Y;
    _long iY = MOUSE_MOVE(eMouseMove);

    eMouseMove = MMS_WHEEL;
    _long iWheel = MOUSE_MOVE(eMouseMove);

    if (0 < iWheel)
    {
        if (m_eAnimType == ANIM_BASE_R)
            m_iAnimIndex = 13;

        if(m_eAnimType == ANIM_BASE_L)
            m_iAnimIndex = 12;

        m_iStateChangeKeyFrame = 87;

    }
    else if (0 > iWheel)
    {
        m_iAnimIndex = 14;
        m_iStateChangeKeyFrame = 70;
    }
    else
    {
        if (m_eAnimType == ANIM_BASE_R)
            m_iAnimIndex = 9;

        else if (m_eAnimType == ANIM_BASE_L)
            m_iAnimIndex = 8;

        m_iStateChangeKeyFrame = 85;
    }

    //MouseMoe �׽�Ʈ��
   

    // ���� ��� -> ������ �ϴ�
    if (iX > 0 && iY > 0)
    {
        m_iStateChangeKeyFrame = 84;
        m_iAnimIndex = 0;
    }

    // ������ �ϴ� -> ���� ���
    if (iX < 0 && iY < 0)
    {
        m_iStateChangeKeyFrame = 84;
        m_iAnimIndex = 5;
    }

    // ������ ��� -> ���� �ϴ�
    if (iX < 0 && iY > 0)
    {
        m_iStateChangeKeyFrame = 84;
        m_iAnimIndex = 3;
    }

    // ���� �ϴ� - > ������ ���
    if (iX > 0 && iY < 0)
    {
        m_iStateChangeKeyFrame = 84;
        m_iAnimIndex = 6;
    }

    // ���¸� �ٲ������.. �ƴϸ� Base_R or Base_L �� ��
    m_eAnimType = ANIM_ATTACK;

    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CWarrior_Attack_01::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    // 9 : ��� ����(R Attack)
    // 23 : Ȳ�� ����(SKill)

    // Ȳ�� ���� ĵ��
    if (m_iAnimIndex == 22 && pAnimator->Is_CurAnimFinished())
        return STATE_WALK_PLAYER;
    
    // ������� �ִϸ��̼� ����
    if (m_iAnimIndex == 14)
        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.f);


    if (m_iAnimIndex == 15 && pAnimator->Is_CurAnimFinished())
    {
        return STATE_WALK_PLAYER;
    }

    // Ȳ�� ����
    if (m_iAnimIndex == 24)
    {
        if (KEY(Q, NONE))
        {
            m_iAnimIndex = 23;

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.f);
            pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.1f);
        }
        else if(pAnimator->Is_CurAnimFinished())
        {
            m_iAnimIndex = 25;

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.f);
            pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.f);
        }
    }

    // Ȳ�� ���� Loop
    if (m_iAnimIndex == 25)
    {
        if (KEY(Q, NONE))
        {
            m_iAnimIndex = 23;

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.f);
            pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.f);
        }
        

        if (pAnimator->Is_CurAnimFinished())
        {
            ++m_iFinishedFrame;

            if (m_iFinishedFrame == 3)
            {
                m_iAnimIndex = 22;
                m_iFinishedFrame = 0;
            }


            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.f);
            pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, 0.f);
        }

        
    }

    // Ȳ�� ����� ������ ���� ���� �͵��� �־������.
    // �⺻������ ������ ������ ������ �ϱ� ������ Base_L �� �ٲ��ָ��.

    if (m_iAnimIndex == 9 ||
        m_iAnimIndex == 23 ||
        m_iAnimIndex == 3 ||
        m_iAnimIndex == 5 ||
        m_iAnimIndex == 12 ||
        m_iAnimIndex == 14)
    {
        m_eAnimType = ANIM_BASE_L;
        m_iAnimIndex = 8;
    }



    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Attack_01::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CWarrior_Attack_01::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Attack ���� ���� ����
    1.  LBuutton �� �̿��� �����Ѵ�.
    2.  CTRL LButton �� �̿��� ������´�.
    */
    if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
    {
        m_iAnimIndex = 0;

        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);

        return m_eStateType;
    }

    if (KEY(Q, TAP))
    {
        m_iAnimIndex = 24;

        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);

        return m_eStateType;
    }

    if (KEY(T, TAP))
    {
        m_iAnimIndex = 15;

        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);

        return m_eStateType;
    }

    MOUSEMOVE eMouseMove = MMS_WHEEL;
    _long iMouseMove = MOUSE_MOVE(eMouseMove);



    if (0 > iMouseMove || 0 < iMouseMove)
    {
        m_iAnimIndex = 0;

        return m_eStateType;
    }

    // ���
    if (0 < iMouseMove)
    {
       

        return m_eStateType;
    }


    return STATE_END;
}