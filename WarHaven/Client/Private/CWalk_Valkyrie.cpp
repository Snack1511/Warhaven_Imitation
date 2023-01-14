#include "stdafx.h"
#include "CWalk_Valkyrie.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWalk_Valkyrie::CWalk_Valkyrie()
{
}

CWalk_Valkyrie::~CWalk_Valkyrie()
{
}

HRESULT CWalk_Valkyrie::Initialize()
{
	m_fInterPolationTime = 0.2f;


	////m_eAnimDivide = ANIM_DIVIDE::eBODYLOWER;
	m_vecAdjState.push_back(STATE_GUARD_BEGIN_VALKYRIE);
	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_VALKYRIE);
	/*나중에 추가*/
	m_vecAdjState.push_back(STATE_COUNTER_VALKYRIE);
	m_vecAdjState.push_back(STATE_SHIELDATTACK_VALKYRIE);
	m_vecAdjState.push_back(STATE_SPINATTACK_VALKYRIE);
	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT_VALKYRIE);

	Init_CommonState_Hero_Player();

	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;

	Add_KeyFrame(0, 0, true);
	Add_KeyFrame(40, 0, true);

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 100.f;


	return S_OK;
}

void CWalk_Valkyrie::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	/* Owner의 Animator Set Idle로 */
	m_fMaxSpeed = pOwner->Get_Status().fWalkSpeed;

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWalk_Valkyrie::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	Move_Direction_Loop(pOwner, pAnimator, 0.05f);



	return __super::Tick(pOwner, pAnimator);
}

void CWalk_Valkyrie::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	/* 할거없음 */
}

STATE_TYPE CWalk_Valkyrie::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

	if (KEY(C, HOLD))
	{
		// 걸어간다.
		if (KEY(W, HOLD) ||
			KEY(A, HOLD) ||
			KEY(S, HOLD) ||
			KEY(D, HOLD))
		{

			return m_eStateType;
		}

	}

	return STATE_END;
}

void CWalk_Valkyrie::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

	switch (iSequence)
	{
	case 0:
		Play_Sound(L"Env_FootStepGround", CHANNEL_ENVIRONMENT, 0.4f);
		break;
	}
}