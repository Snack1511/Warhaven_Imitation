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

	//m_vecAdjState.push_back(STATE_IDLE_VALKYRIE_R);
	//m_vecAdjState.push_back(STATE_WALK_VALKYRIE_R);
	//m_vecAdjState.push_back(STATE_RUNBEGIN_VALKYRIE_R);
	//m_vecAdjState.push_back(STATE_JUMP_VALKYRIE_R);

	//m_vecAdjState.push_back(STATE_SHIELDATTACK_VALKYRIE);
	//m_vecAdjState.push_back(STATE_SPINATTACK_VALKYRIE);

	//m_vecAdjState.push_back(STATE_SPRINT_BEGIN_VALKYRIE);

	m_iStateChangeKeyFrame = 9999;

	m_fInterPolationTime = 0.1f;
	m_fAnimSpeed = 2.f;


	m_fMyAccel = 10.f;
	m_fMyMaxLerp = 0.5f;



	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f * 0.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f * 0.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f * 0.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f * 0.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.5f * 0.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f  * 0.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f * 0.5f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f * 0.5f;


	/*���߿� �߰�*/
	//m_vecAdjState.push_back(STATE_COUNTER_VALKYRIE);
	//m_vecAdjState.push_back(STATE_SHIELDATTACK_VALKYRIE);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_R);
	//m_vecAdjState.push_back(STATE_VERTICALATTACK_VALKYRIE_R);
	//m_vecAdjState.push_back(STATE_ATTACK_STING_VALKYRIE_R);


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

