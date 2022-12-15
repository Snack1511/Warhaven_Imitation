#include "stdafx.h"
#include "CIdle_Paladin_L.h"

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

CIdle_Paladin_L::CIdle_Paladin_L()
{
}

CIdle_Paladin_L::~CIdle_Paladin_L()
{
}

CIdle_Paladin_L* CIdle_Paladin_L::Create()
{
    CIdle_Paladin_L* pInstance = new CIdle_Paladin_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CIdle_Paladin_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CIdle_Paladin_L::Initialize()
{
 //   __super::Initialize();

    // ����ü �и�.
    // 


    m_eAnimType = ANIM_BASE_L;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 3;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_IDLE_PALADIN_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    
    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 1.f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.

    m_vecAdjState.push_back(STATE_GUARD_BEGIN_PALADIN);
    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PALADIN);

    m_vecAdjState.push_back(STATE_SWITCH_L_TO_R_PALADIN);

    m_vecAdjState.push_back(STATE_WALK_PALADIN_L);
    m_vecAdjState.push_back(STATE_JUMP_PALADIN_L);
    m_vecAdjState.push_back(STATE_RUNBEGIN_PALADIN_L);

    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_L);
    m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT_PALADIN);

    m_vecAdjState.push_back(STATE_SHIELDWALL_BEGIN_PALADIN);
    m_vecAdjState.push_back(STATE_RUSH_BEGIN_PALADIN);
    m_vecAdjState.push_back(STATE_SHIELDSLAM_PALADIN);




    return S_OK;
}

void CIdle_Paladin_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    if (ePrevType == STATE_SWITCH_R_TO_L)
        m_fInterPolationTime = 0.f;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CIdle_Paladin_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);

}

void CIdle_Paladin_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CIdle_Paladin_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* PALADIN�� Idle�� ���� ����
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
