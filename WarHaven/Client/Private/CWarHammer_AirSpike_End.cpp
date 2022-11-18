#include "stdafx.h"
#include "CWarHammer_AirSpike_End.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CWarHammer_AirSpike_End::CWarHammer_AirSpike_End()
{
}

CWarHammer_AirSpike_End::~CWarHammer_AirSpike_End()
{
}

CWarHammer_AirSpike_End* CWarHammer_AirSpike_End::Create()
{
	CWarHammer_AirSpike_End* pInstance = new CWarHammer_AirSpike_End();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWarHammer_AirSpike_End");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CWarHammer_AirSpike_End::Initialize()
{
	m_eAnimType = ANIM_ATTACK;            // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 2;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_AIRSPIKE_END_WARHAMMER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	m_iStateChangeKeyFrame = 66;

	m_fInterPolationTime = 0.f;
	m_fAnimSpeed = 2.5f;

	m_vecAdjState.push_back(STATE_IDLE_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_WALK_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_RUNBEGIN_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_JUMP_WARHAMMER_R);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_ATTACK_STING_WARHAMMER_R);
	m_vecAdjState.push_back(STATE_INSTALL_BEIGN_WARHAMMER);


	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_WARHAMMER);

	Add_KeyFrame(12, 0);
	Add_KeyFrame(30, 1);

	// return __super::Initialize();
	return S_OK;
}

void CWarHammer_AirSpike_End::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.1f;

	Physics_Setting(0.1, pOwner);

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarHammer_AirSpike_End::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Tick(pOwner, pAnimator);
}

void CWarHammer_AirSpike_End::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CWarHammer_AirSpike_End::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

	/* WARHAMMER�� AirSpike_End �� ���� ����
	1.  ���鿡 ��������
	*/
	if (!pOwner->Is_Air())
	{
		return m_eStateType;
	}

	return STATE_END;
}

void CWarHammer_AirSpike_End::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:
		m_bAttackTrigger = true;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
		break;

	case 1:
		m_bAttackTrigger = false;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
		break;

	default:
		break;
	}

}
