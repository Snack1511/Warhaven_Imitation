#include "stdafx.h"
#include "CQanda_Aiming_Sniping.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CQanda_Aiming_Sniping::CQanda_Aiming_Sniping()
{
}

CQanda_Aiming_Sniping::~CQanda_Aiming_Sniping()
{
}

CQanda_Aiming_Sniping* CQanda_Aiming_Sniping::Create()
{
	CQanda_Aiming_Sniping* pInstance = new CQanda_Aiming_Sniping();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CQanda_Aiming_Sniping");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CQanda_Aiming_Sniping::Initialize()
{
	m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 2;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_ATTACK_AIMING_SNIPING_QANDA;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	m_fInterPolationTime = 0.f;
	m_fAnimSpeed = 2.f;

	m_fMaxDistance = 50.f;

	m_vecAdjState.push_back(STATE_ATTACK_SHOOT_SNIPING_QANDA);

	return S_OK;
}

void CQanda_Aiming_Sniping::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 10.f;

	m_fMaxSpeed = pOwner->Get_Status().fRunBeginSpeed;

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CQanda_Aiming_Sniping::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	//// ����ü �˻� 
	//// �ִ�Ÿ� < �����ǰŸ�
	//// ��ġ�� �ʰ� ����.
	//if (m_fMaxDistance < 100.f)
	//{
	//	m_SnipingTarget.push_back(pOwner);
	//}

	return __super::Tick(pOwner, pAnimator);
}

void CQanda_Aiming_Sniping::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	// ���
	m_SnipingTarget;
}

STATE_TYPE CQanda_Aiming_Sniping::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* WARHAMMER�� Loop�� ���� ����
	1.  Begin �� ������ ���.
	*/
	if (pAnimator->Is_CurAnimFinished())
	{
		return m_eStateType;
	}

	return STATE_END;
}
