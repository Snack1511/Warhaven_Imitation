#include "stdafx.h"
#include "CIdle_Player_R.h"

#include "GameInstance.h"
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

CIdle_Player_R::CIdle_Player_R()
{
}

CIdle_Player_R::~CIdle_Player_R()
{
}

CIdle_Player_R* CIdle_Player_R::Create()
{
    CIdle_Player_R* pInstance = new CIdle_Player_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CIdle_Player_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CIdle_Player_R::Initialize()
{
    // ����ü �и�.
    // 

  //  __super::Initialize();

    m_eAnimType = ANIM_BASE_R;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 11;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_IDLE_PLAYER_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    
    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�
    m_vecAdjState.push_back(STATE_WALK_PLAYER_R);
    m_vecAdjState.push_back(STATE_RUN_PLAYER_R);

    m_vecAdjState.push_back(STATE_PLAYER_SKILL2);
    m_vecAdjState.push_back(STATE_PLAYER_SKILL1);

    m_vecAdjState.push_back(STATE_SWITCH_R_TO_L);


    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    //m_vecAdjState.push_back(STATE_JUMP);
    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);

    //m_vecAdjState.push_back(STATE_IDLE_PLAYER_L);


    //m_vecAdjState.push_back(STATE_WALK_PLAYER);
    //m_vecAdjState.push_back(STATE_RUN_PLAYER);
    //m_vecAdjState.push_back(STATE_ATTACK_WARRIOR);
    //m_vecAdjState.push_back(STATE_JUMP_PLAYER);
    //m_vecAdjState.push_back(STATE_SPRINT_PLAYER);


    m_iChangeHandIndex = 53;

    return S_OK;
}

void CIdle_Player_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
    /* Owner�� Animator Set Idle�� */
    //GET_COMPONENT_FROM(pOwner, CModel)->Set_ShaderColor(MODEL_PART_WEAPON, _float4(1, 0.3, 0, 0));


    


    __super::Enter(pOwner, pAnimator, iPreAnimIndex);
}

STATE_TYPE CIdle_Player_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    if (m_iChangeHandIndex == m_iAnimIndex)
    {
        STATE_TYPE eStateType = STATE_END;

        eStateType = End_Animation(53, ANIM_BASE_L, STATE_IDLE_PLAYER_L, false, pOwner, pAnimator);

        if (STATE_END != eStateType)
            return eStateType;
    }

    // Į ��ġ ����
    if (KEY(R, TAP))
    {
        Change_Animation(m_iAnimIndex, m_iChangeHandIndex, pOwner, pAnimator);
    }

    //// Į ��ġ ����
    //if (KEY(R, TAP))
    //{
    //    Change_Animation(11, 53, pOwner, pAnimator);
    //}

    return __super::Tick(pOwner, pAnimator);
}

void CIdle_Player_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CIdle_Player_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Idle�� ���� ���� 
    1. ���� �������� �ִϸ��̼��� ������ ��
    */

    if (KEY(R, NONE))
    {
        if (pAnimator->Is_CurAnimFinished())
            return m_eStateType;
    }
  


    return STATE_END;
}

void CIdle_Player_R::Switch_Right_And_Left(_uint iAnimType, _uint iAnimIndex, _uint iChangeAnimIndex, CAnimator* pAnimator)
{
    if (m_eAnimType == iAnimType == m_iAnimIndex == iAnimIndex)
    {
        m_iAnimIndex = iChangeAnimIndex;

        pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
        pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
    }
}
