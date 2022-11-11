#include "stdafx.h"
#include "CRun_Player_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CColorController.h"

CRun_Player_L::CRun_Player_L()
{
}

CRun_Player_L::~CRun_Player_L()
{
}

CRun_Player_L* CRun_Player_L::Create()
{
    CRun_Player_L* pInstance = new CRun_Player_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_Player_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CRun_Player_L::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_BASE_L;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 0;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_RUN_PLAYER_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 9999;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
//    m_vecAdjState.push_back(STATE_ATTACK_WARRIOR);
    m_vecAdjState.push_back(STATE_WALK_PLAYER_L);
//    m_vecAdjState.push_back(STATE_SLIDE_PLAYER);
 //   m_vecAdjState.push_back(STATE_JUMP_PLAYER);
 //   m_vecAdjState.push_back(STATE_SPRINT_PLAYER);
    //m_vecAdjState.push_back(STATE_IDLE_PLAYER);

    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);

    // 15
    m_VecDirectionAnimIndex[STATE_DIRECTION_E] = 18;

    m_VecDirectionAnimIndex[STATE_DIRECTION_N] = 19;
    m_VecDirectionAnimIndex[STATE_DIRECTION_NE] = 20;
    m_VecDirectionAnimIndex[STATE_DIRECTION_NW] = 21;

    m_VecDirectionAnimIndex[STATE_DIRECTION_S] = 34;
    m_VecDirectionAnimIndex[STATE_DIRECTION_SE] = 35;
    m_VecDirectionAnimIndex[STATE_DIRECTION_SW] = 36;



    m_VecDirectionAnimIndex[STATE_DIRECTION_W] = 22;
    

    // ------------------------------------------------------

    // 234
    m_VecDirectionBeginAnimIndex[STATE_DIRECTION_E] = 10;
    
    m_VecDirectionBeginAnimIndex[STATE_DIRECTION_N] = 11;
    m_VecDirectionBeginAnimIndex[STATE_DIRECTION_NE] = 12;
    m_VecDirectionBeginAnimIndex[STATE_DIRECTION_NW] = 13;
   
    m_VecDirectionBeginAnimIndex[STATE_DIRECTION_S] = 14;
    m_VecDirectionBeginAnimIndex[STATE_DIRECTION_SE] = 15;
    m_VecDirectionBeginAnimIndex[STATE_DIRECTION_SW] = 16;
   
    m_VecDirectionBeginAnimIndex[STATE_DIRECTION_W] = 17;
   


    


    return S_OK;
}

void CRun_Player_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
    /* Owner�� Animator Set Idle�� */
    CColorController::COLORDESC m_tColorDesc;
    ZeroMemory(&m_tColorDesc, sizeof(CColorController::COLORDESC));

    m_tColorDesc.eFadeStyle = CColorController::TIME;
    m_tColorDesc.fFadeInStartTime = 1.f;
    m_tColorDesc.fFadeInTime = 1.f;
    m_tColorDesc.fFadeOutStartTime = 1.f;
    m_tColorDesc.fFadeOutTime = 1.f;

    m_tColorDesc.vTargetColor = _float4(1.f, 0.f, 0.f, 0.f);

    m_tColorDesc.iMeshPartType = MODEL_PART_WEAPON;
    m_tColorDesc.iStartKeyFrame = 3;
    m_tColorDesc.iEndKeyFrame = 6;


    GET_COMPONENT_FROM(pOwner, CColorController)->Set_ColorControll(m_tColorDesc);



    __super::Enter(pOwner, pAnimator, iPreAnimIndex);
}

STATE_TYPE CRun_Player_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
    { 
        return STATE_IDLE_PLAYER_L;

    }

    return __super::Tick(pOwner, pAnimator);
}

void CRun_Player_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CRun_Player_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Walk�� ���� ����
    1. CTRL �� ������ ���� ����
    2. WASD �� ���� ����
    */

    // m_eStateType �� End �� ���� ������ Enter �� ȣ���Ѵ�.

    // W �� A �� ������ ���� ������ �̵��Ѵ�.
    // W �� D �� ������ ���� ������ �̵��Ѵ�.

    // ���� WASD �� �����ٸ�

    // �ȱ⸦ ���� �ʰ�
    if (KEY(CTRL, NONE))
    {
        // Ȳ�� ���⸦ ���� �ʰ�
        if (KEY(Q, NONE))
        {
            // ������ ���� �ʰ�
            if (KEY(SPACE, NONE))
            {
                // �����̵��� ���� �ʴ´ٸ�? (���߿� ��߿ø���, �һ� ������ ����)
                if (KEY(J, NONE))
                {
                    if (KEY(W, HOLD) ||
                        KEY(A, HOLD) ||
                        KEY(S, HOLD) ||
                        KEY(D, HOLD))
                    {
                        return m_eStateType;
                    }
                }
            }
        }

    }
    

   


    return STATE_END;
}

//_bool CRun_Player_L::Change_Animation_Run(_uint iBeginAttackAnim, _uint iAttackAnim, CAnimator* pAnimator)
//{
//    // RunBegin 
//    if (m_iAnimIndex != iBeginAttackAnim)
//    {
//        if (m_iAnimIndex == 21)
//        {
//            if (m_iAnimIndex == iAttackAnim && pAnimator->Is_CurAnimFinished())
//            {
//                m_eAnimType = ANIM_BASE_R;
//                m_iAnimIndex = iBeginAttackAnim;
//
//                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
//                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 3.f);
//            }
//        }
//        else
//        {
//            m_eAnimType = ANIM_BASE_R;
//            m_iAnimIndex = iAttackAnim;
//
//            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
//            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.7f);
//
//        }
//
//    return false;
//}
