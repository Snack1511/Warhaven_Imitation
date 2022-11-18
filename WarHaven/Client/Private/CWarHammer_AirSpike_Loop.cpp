#include "stdafx.h"
#include "CWarHammer_AirSpike_Loop.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CWarHammer_AirSpike_Loop::CWarHammer_AirSpike_Loop()
{
}

CWarHammer_AirSpike_Loop::~CWarHammer_AirSpike_Loop()
{
}

CWarHammer_AirSpike_Loop* CWarHammer_AirSpike_Loop::Create()
{
	CWarHammer_AirSpike_Loop* pInstance = new CWarHammer_AirSpike_Loop();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWarHammer_AirSpike_Loop");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CWarHammer_AirSpike_Loop::Initialize()
{
	m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 1;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_AIRSPIKE_LOOP_WARHAMMER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	m_fInterPolationTime = 0.f;
	m_fAnimSpeed = 2.5f;

	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_L);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_L);

	//m_vecAdjState.push_back(STATE_ATTACK_STING_WARHAMMER_L);
	//m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);


	m_vecAdjState.push_back(STATE_AIRSPIKE_END_WARHAMMER);




	 // return __super::Initialize();

	return S_OK;
}

void CWarHammer_AirSpike_Loop::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 10.f;

	m_fMaxSpeed = pOwner->Get_Status().fSprintJumpSpeed;
	Physics_Setting(m_fMaxSpeed, pOwner);

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarHammer_AirSpike_Loop::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (!pOwner->Is_Air())
		return STATE_AIRSPIKE_END_WARHAMMER;
	Follow_MouseLook(pOwner);
	pOwner->Set_DirAsLook();

	return __super::Tick(pOwner, pAnimator);
}

void CWarHammer_AirSpike_Loop::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CWarHammer_AirSpike_Loop::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
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
