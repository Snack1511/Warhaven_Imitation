#include "stdafx.h"
#include "CIdle_Valkyrie_L.h"

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

CIdle_Valkyrie_L::CIdle_Valkyrie_L()
{
}

CIdle_Valkyrie_L::~CIdle_Valkyrie_L()
{
}

CIdle_Valkyrie_L* CIdle_Valkyrie_L::Create()
{
    CIdle_Valkyrie_L* pInstance = new CIdle_Valkyrie_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CIdle_Valkyrie_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CIdle_Valkyrie_L::Initialize()
{
 //   __super::Initialize();

    // ����ü �и�.
    // 


    m_eAnimType = ANIM_BASE_L;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 3;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_IDLE_VALKYRIE_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    
    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 1.f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.

    m_vecAdjState.push_back(STATE_SWITCH_L_TO_R_VALKYRIE);

    m_vecAdjState.push_back(STATE_WALK_VALKYRIE_L);
    m_vecAdjState.push_back(STATE_JUMP_VALKYRIE_L);
    m_vecAdjState.push_back(STATE_RUNBEGIN_VALKYRIE_L);

    /*���߿� �߰�*/
    m_vecAdjState.push_back(STATE_COUNTER_VALKYRIE);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_VALKYRIE_L);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_L);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_VALKYRIE_L);
    m_vecAdjState.push_back(STATE_ATTACK_STING_VALKYRIE_L);
    m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT_VALKYRIE);


    m_vecAdjState.push_back(STATE_SPINATTACK_VALKYRIE);

    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_VALKYRIE);



    return S_OK;
}

void CIdle_Valkyrie_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    if (ePrevType == STATE_SWITCH_R_TO_L)
        m_fInterPolationTime = 0.f;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CIdle_Valkyrie_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);

}

void CIdle_Valkyrie_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CIdle_Valkyrie_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* VALKYRIE�� Idle�� ���� ����
	1. ���� �������� �ִϸ��̼��� ������ ��
	*/

	if (
		KEY(W, NONE) &&
		KEY(A, NONE) &&
		KEY(S, NONE) &&
		KEY(D, NONE))
	{		
	
		if (pAnimator->Is_CurAnimFinished())
			return m_eStateType;
	}

    return STATE_END;
}
