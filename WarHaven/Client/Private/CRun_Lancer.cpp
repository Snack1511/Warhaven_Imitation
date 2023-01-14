#include "stdafx.h"
#include "CRun_Lancer.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CColorController.h"

CRun_Lancer::CRun_Lancer()
{
}

CRun_Lancer::~CRun_Lancer()
{
}

CRun_Lancer* CRun_Lancer::Create()
{
    CRun_Lancer* pInstance = new CRun_Lancer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_Lancer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CRun_Lancer::Initialize()
{
    m_eAnimType = ANIM_BASE_R;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 5;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_RUN_LANCER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;


    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;
    m_fMyMaxLerp = 0.4f;
    m_fMyAccel = 30.f;

    m_vecAdjState.push_back(STATE_STOP_LANCER);
    m_vecAdjState.push_back(STATE_JUMP_LANCER);

    m_vecAdjState.push_back(STATE_ATTACK_STING_LANCER);
    m_vecAdjState.push_back(STATE_ATTACK_DEAFULT_LANCER);
    m_vecAdjState.push_back(STATE_ATTACK_BREEZE_BEGIN_LANCER);



   //m_iDirectionAnimIndex[STATE_DIRECTION_E] = 17;
   //m_iDirectionAnimIndex[STATE_DIRECTION_N] = 18;
   //m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 19;
   //m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 20;
   //m_iDirectionAnimIndex[STATE_DIRECTION_S] = 34;
   //m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 35;
   //m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 36;
   //m_iDirectionAnimIndex[STATE_DIRECTION_W] = 24;

   //m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
   //m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
   //m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
   //m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
   //m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.f;
   //m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
   //m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
   //m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;

    //Add_KeyFrame(5, 0, true);
    //Add_KeyFrame(5, 0, true);

    return S_OK;
}

void CRun_Lancer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    m_fMaxSpeed = pOwner->Get_Status().fSprintSpeed;

    Physics_Setting(m_fMaxSpeed, pOwner, false);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRun_Lancer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pOwner->Is_Air())
        return STATE_JUMPFALL_LANCER;

    DoMove(STATE_DIRECTION_N, pOwner);
    Follow_MouseLook(pOwner);
    pOwner->Set_DirAsLook();

    //_uint iDirection = Move_Direction_Loop(pOwner, pAnimator, 0.05f);

    //if (iDirection < 8)
    //    m_iCurDirection = iDirection;


    //if (pOwner->Is_Air())
    //    return STATE_JUMPFALL_LANCER;


    return __super::Tick(pOwner, pAnimator);
}

void CRun_Lancer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PreAnimIndex() = pAnimator->Get_CurAnimFrame();

    /* �Ұž��� */
}

STATE_TYPE CRun_Lancer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    if (KEY(C, NONE))
    {
        // �ɾ��.
        if (KEY(W, HOLD) ||
            KEY(A, HOLD) ||
            KEY(S, HOLD) ||
            KEY(D, HOLD))
        {

            return m_eStateType;
        }

    }

    return STATE_END;
}
