#include "stdafx.h"
#include "CWarrior_Attack_UpperMiddle_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"


CWarrior_Attack_UpperMiddle_R::CWarrior_Attack_UpperMiddle_R()
{
}

CWarrior_Attack_UpperMiddle_R::~CWarrior_Attack_UpperMiddle_R()
{
}

CWarrior_Attack_UpperMiddle_R* CWarrior_Attack_UpperMiddle_R::Create()
{
    CWarrior_Attack_UpperMiddle_R* pInstance = new CWarrior_Attack_UpperMiddle_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Attack_UpperMiddle_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarrior_Attack_UpperMiddle_R::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 9;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_UPPER_MIDDLE_PLAYER_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    m_iStateChangeKeyFrame = 80;
    
    m_vecAdjState.push_back(STATE_IDLE_PLAYER_L);
    m_vecAdjState.push_back(STATE_WALK_PLAYER_L);
    m_vecAdjState.push_back(STATE_RUN_PLAYER_L);
    //m_vecAdjState.push_back(STATE_IDLE_PLAYER);
    //m_vecAdjState.push_back(STATE_WALK_PLAYER);
    //m_vecAdjState.push_back(STATE_RUN_PLAYER);
    //m_vecAdjState.push_back(STATE_SILDING);
    //m_vecAdjState.push_back(STATE_RUN);
    //m_vecAdjState.push_back(STATE_DASH);
    //m_vecAdjState.push_back(STATE_WALK);

    return S_OK;
}

void CWarrior_Attack_UpperMiddle_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
	if (ePrevType == STATE_SWITCH_L_TO_R)
	{
		m_fAnimSpeed = 2.3f;
	}
	else
		m_fAnimSpeed = 2.5f;



    /* Owner�� Animator Set Idle�� */
    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CWarrior_Attack_UpperMiddle_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Attack_UpperMiddle_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CWarrior_Attack_UpperMiddle_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Attack ���� ���� ����
    1.  LBuutton �� �̿��� �����Ѵ�.
    */
    if (KEY(CTRL, NONE))
    {
        if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
        {
            return m_eStateType;
        }
    }
   
    return STATE_END;
}
