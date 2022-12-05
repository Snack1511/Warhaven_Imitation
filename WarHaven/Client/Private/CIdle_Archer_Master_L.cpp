#include "stdafx.h"
#include "CIdle_Archer_Master_L.h"

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

CIdle_Archer_Master_L::CIdle_Archer_Master_L()
{
}

CIdle_Archer_Master_L::~CIdle_Archer_Master_L()
{
}

CIdle_Archer_Master_L* CIdle_Archer_Master_L::Create()
{
    CIdle_Archer_Master_L* pInstance = new CIdle_Archer_Master_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CIdle_Archer_Master_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CIdle_Archer_Master_L::Initialize()
{
 //   __super::Initialize();

    // ����ü �и�.
    // 


    m_eAnimType = ANIM_BASE_L;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 3;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_IDLE_PLAYER_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    
    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 1.f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
    //m_vecAdjState.push_back(STATE_IDLE_PLAYER_R);

    m_vecAdjState.push_back(STATE_SWITCH_L_TO_R);

    m_vecAdjState.push_back(STATE_WALK_PLAYER_L);
    m_vecAdjState.push_back(STATE_RUN_BEGIN_PLAYER_L);

	m_vecAdjState.push_back(STATE_JUMP_PLAYER_L);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_L);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_L);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_L);

    m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_L);

    m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);

	m_vecAdjState.push_back(STATE_GUARD_BEGIN_PLAYER);


    m_vecAdjState.push_back(STATE_WARRIOR_OXEN_BEGIN);
    m_vecAdjState.push_back(STATE_WARRIOR_GUARDBREAK);

    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PLAYER);

    m_vecAdjState.push_back(STATE_CHANGE_PLAYER);

    return S_OK;
}

void CIdle_Archer_Master_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    if (ePrevType == STATE_SWITCH_R_TO_L)
        m_fInterPolationTime = 0.f;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CIdle_Archer_Master_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);

}

void CIdle_Archer_Master_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CIdle_Archer_Master_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* Player�� Idle�� ���� ����
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
