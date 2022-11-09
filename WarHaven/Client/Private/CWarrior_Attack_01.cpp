#include "stdafx.h"
#include "CWarrior_Attack_01.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

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
    m_iStateChangeKeyFrame = 50;
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
    
    MOUSEMOVE eMouseMove = MMS_X;
    _long iX = MOUSE_MOVE(eMouseMove);

    eMouseMove = MMS_Y;
    _long iY = MOUSE_MOVE(eMouseMove);

    // R_Idle L_Idle Setting
    if (m_eAnimType == ANIM_BASE_R)
    {
        if (KEY(CTRL, HOLD))
            m_iAnimIndex = 13;
        else
            m_iAnimIndex = 9;
    }
        else if (m_eAnimType == ANIM_BASE_L)
    {
        if (KEY(CTRL, HOLD))
            m_iAnimIndex = 12;
        else
            m_iAnimIndex = 8;
    }


    //MouseMoe �׽�Ʈ��
   

    // ���� ��� -> ������ �ϴ�
    if (iX > 0 && iY > 0)
    {
        m_iAnimIndex = 0;
    }

    // ������ �ϴ� -> ���� ���
    if (iX < 0 && iY < 0)
    {
        m_iAnimIndex = 5;
    }

    // ������ ��� -> ���� �ϴ�
    if (iX < 0 && iY > 0)
    {
        m_iAnimIndex = 3;
    }

    // ���� �ϴ� - > ������ ���
    if (iX > 0 && iY < 0)
    {
        m_iAnimIndex = 6;
    }

    //

    // ���¸� �ٲ������.. �ƴϸ� Base_R or Base_L �� ��
    m_eAnimType = ANIM_ATTACK;

    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CWarrior_Attack_01::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    // 9 : ��� ����(R Attack)
    // 23 : Ȳ�� ����(SKill)
    Create_SwordEffect(pOwner);


    if (m_iAnimIndex == 22 && pAnimator->Is_CurAnimFinished())
        return STATE_WALK_PLAYER;
    

    if (m_iAnimIndex == 24)
    {
        if (KEY(Q, NONE))
        {
            m_iAnimIndex = 23;

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.f);
        }
        else if(pAnimator->Is_CurAnimFinished())
        {
            m_iAnimIndex = 25;

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.f);
        }
    }

    if (m_iAnimIndex == 25)
    {
        if (KEY(Q, NONE))
        {
            m_iAnimIndex = 23;

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.f);
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
        }

        
    }

    // Ȳ�� ����� ������ ���� ���� �͵��� �־������.
    // �⺻������ ������ ������ ������ �ϱ� ������ Base_L �� �ٲ��ָ��.

    if (m_iAnimIndex == 9 ||
        m_iAnimIndex == 23 ||
        m_iAnimIndex == 3 ||
        m_iAnimIndex == 5 ||
        m_iAnimIndex == 12)
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
        CColorController::COLORDESC tColorDesc;
        ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

        tColorDesc.eFadeStyle = CColorController::KEYFRAME;
        tColorDesc.fFadeInStartTime = 1.f;
        tColorDesc.fFadeInTime = 1.f;
        tColorDesc.fFadeOutStartTime = 1.f;
        tColorDesc.fFadeOutTime = 1.f;
        tColorDesc.vTargetColor = _float4(1.f, 0.f, 0.f, 1.f);
        tColorDesc.iMeshPartType = MODEL_PART_WEAPON;
        tColorDesc.iStartKeyFrame = 10;
        tColorDesc.iEndKeyFrame = 30;
        GET_COMPONENT_FROM(pOwner, CColorController)->Set_ColorControll(tColorDesc);

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


    return STATE_END;
}

void CWarrior_Attack_01::Create_SwordEffect(CUnit* pOwner)
{
    m_fCreateTimeAcc += fDT(0);

    if (m_fCreateTime <= m_fCreateTimeAcc)
    {
        CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CSword_Effect), pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
        m_fCreateTimeAcc = 0.f;
    }
}
