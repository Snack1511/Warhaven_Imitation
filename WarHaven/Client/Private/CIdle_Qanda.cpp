#include "stdafx.h"
#include "CIdle_Qanda.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"

#include "CColorController.h"

#include "UsefulHeaders.h"

CIdle_Qanda::CIdle_Qanda()
{
}

CIdle_Qanda::~CIdle_Qanda()
{
}

CIdle_Qanda* CIdle_Qanda::Create()
{
    CIdle_Qanda* pInstance = new CIdle_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CIdle_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CIdle_Qanda::Initialize()
{
    // ����ü �и�.
    // 

  //  __super::Initialize();

    m_eAnimType = ANIM_BASE_R;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 11;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = STATE_IDLE_ARCHER_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    
    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 1.f;

    m_vecAdjState.push_back(STATE_WALK_ARCHER_R);
    m_vecAdjState.push_back(STATE_RUNBEGIN_ARCHER_R);

    m_vecAdjState.push_back(STATE_JUMP_ARCHER_R);

    m_vecAdjState.push_back(STATE_ATTACK_BEGIN_ARCHER);

    m_vecAdjState.push_back(STATE_ATTACK_BEGIN_ARCHER);
    m_vecAdjState.push_back(STATE_ATTACK_BEGIN_SNIPING_ARCHER);
    m_vecAdjState.push_back(STATE_ATTACK_BEGIN_POISION_ARCHER);
    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_ARCHER);

    m_vecAdjState.push_back(STATE_GUARD_ARCHER);
    m_vecAdjState.push_back(STATE_ATTACK_SWING_ARCHER);

    Init_CommonState_Hero_Player();

    return S_OK;
}

void CIdle_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner�� Animator Set Idle�� */
    pOwner->Set_AnimWeaponIndex(0, m_fInterPolationTime, m_fAnimSpeed);
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CIdle_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CIdle_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CIdle_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

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
