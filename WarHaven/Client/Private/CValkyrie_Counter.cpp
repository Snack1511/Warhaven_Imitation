#include "stdafx.h"
#include "CValkyrie_Counter.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CValkyrie_Counter::CValkyrie_Counter()
{
}

CValkyrie_Counter::~CValkyrie_Counter()
{
}

CValkyrie_Counter* CValkyrie_Counter::Create()
{
	CValkyrie_Counter* pInstance = new CValkyrie_Counter();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CValkyrie_Counter");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CValkyrie_Counter::Initialize()
{
	m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 15;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_COUNTER_VALKYRIE;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	 
	m_iStateChangeKeyFrame = 9999;

	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.7f;


	m_fMyAccel = 10.f;
	m_fMyMaxLerp = 0.5f;


	Add_KeyFrame(1, 333);


	return S_OK;
}

void CValkyrie_Counter::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Enable_GuardCollider(true);


	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CValkyrie_Counter::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// ���߿� Hit ���� �� Change �ϵ��� ��ȯ
	if (m_bAttackTrigger)
		return STATE_SPINATTACK_VALKYRIE;

	if (pAnimator->Is_CurAnimFinished())
		return STATE_IDLE_VALKYRIE_R;

	return __super::Tick(pOwner, pAnimator);
}

void CValkyrie_Counter::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;

	pOwner->On_Use(CUnit::SKILL3);
	pOwner->Enable_GuardCollider(false);

}

STATE_TYPE CValkyrie_Counter::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	if (!pOwner->Can_Use(CUnit::SKILL3))
		return STATE_END;

	/* VALKYRIE�� SpinAttack �� ���� ����
	1.  Q ������ ������ ��ų ���
	*/
	if (KEY(Q, HOLD))
	{
		return m_eStateType;
	}

	return STATE_END;
}

void CValkyrie_Counter::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 333:
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"FionaParring", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
		break;
	default:
		break;
	}
}

