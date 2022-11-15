#include "stdafx.h"
#include "CStop_SpearMan_R.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CStop_SpearMan_R::CStop_SpearMan_R()
{
}

CStop_SpearMan_R::~CStop_SpearMan_R()
{
}

CStop_SpearMan_R* CStop_SpearMan_R::Create()
{
	CStop_SpearMan_R* pInstance = new CStop_SpearMan_R();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CStop_SpearMan_R");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CStop_SpearMan_R::Initialize()
{
	__super::Initialize();

	m_eAnimType = ANIM_BASE_R;            // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 31;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_STOP_SPEARMAN_R;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


	m_vecAdjState.push_back(STATE_IDLE_SPEARMAN_R);
	m_vecAdjState.push_back(STATE_WALK_SPEARMAN_R);
	m_vecAdjState.push_back(STATE_RUN_SPEARMAN_R);

	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_R);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_R);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_R);
	//m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_R);

	//m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);

	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 31;
	m_iDirectionAnimIndex[STATE_DIRECTION_N] = 32;
	m_iDirectionAnimIndex[STATE_DIRECTION_S] = 33;
	m_iDirectionAnimIndex[STATE_DIRECTION_W] = 34;




	return S_OK;
}

void CStop_SpearMan_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{


	/* Owner�� Animator Set Idle�� */
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CStop_SpearMan_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{


	//if (m_bMoveTrigger)
	//	Move(Get_Direction(), pOwner);

	//if (m_bAttackTrigger)
	//{
	//	// ���� ����
	//	if (pOwner->Is_Weapon_R_Collision())
	//		return STATE_BOUNCE_PLAYER_R;

	//}

	return __super::Tick(pOwner, pAnimator);
}

void CStop_SpearMan_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);

	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);

}

STATE_TYPE CStop_SpearMan_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);

	/* Player�� Attack ���� ���� ����
	1.  Run ���� WASD Ű�� ������ �ʾҴٸ�
	*/


	//return STATE_END;
}

void CStop_SpearMan_R::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
