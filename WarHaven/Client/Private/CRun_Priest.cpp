#include "stdafx.h"
#include "CRun_Priest.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CColorController.h"

CRun_Priest::CRun_Priest()
{
}

CRun_Priest::~CRun_Priest()
{
}

CRun_Priest* CRun_Priest::Create()
{
	CRun_Priest* pInstance = new CRun_Priest();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CRun_Priest");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CRun_Priest::Initialize()
{
	m_eAnimType = ANIM_BASE_R;            // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 9;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_RUN_PRIEST;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


	m_iStateChangeKeyFrame = 0;

	// ���� ���� �ð�
	m_fInterPolationTime = 0.1f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	m_fAnimSpeed = 2.5f;
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 10.f;

	m_vecAdjState.push_back(STATE_WALK_PRIEST);
	m_vecAdjState.push_back(STATE_JUMP_PRIEST);

	Init_AttackState_Priest();
	Init_CommonState_Player();


	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 17;
	m_iDirectionAnimIndex[STATE_DIRECTION_N] = 18;
	m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 19;
	m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 20;
	m_iDirectionAnimIndex[STATE_DIRECTION_S] = 34;
	m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 35;
	m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 36;
	m_iDirectionAnimIndex[STATE_DIRECTION_W] = 24;

	m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
	m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;

	Add_KeyFrame(20, 0, true);
	Add_KeyFrame(44, 0, true);

	return S_OK;
}

void CRun_Priest::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	m_fMaxSpeed = pOwner->Get_Status().fRunSpeed;

	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRun_Priest::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	_uint iDirection = Move_Direction_Loop(pOwner, pAnimator, 0.05f);

	if (iDirection < 8)
		m_iCurDirection = iDirection;


	if (pOwner->Is_Air())
		return STATE_JUMPFALL_PRIEST;

	if (
		KEY(W, NONE) &&
		KEY(A, NONE) &&
		KEY(S, NONE) &&
		KEY(D, NONE)
		)
	{
		_uint* pInt = new _uint;
		*pInt = m_iCurDirection;
		pOwner->Enter_State(STATE_STOP_PRIEST, (void*)pInt);
		return STATE_END;

	}



	return __super::Tick(pOwner, pAnimator);
}

void CRun_Priest::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	/* �Ұž��� */
}

STATE_TYPE CRun_Priest::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	if (KEY(C, NONE))
	{
		// �ɾ��.
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

void CRun_Priest::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

	switch (iSequence)
	{
	case 0:
		Play_Sound(L"Env_FootStepGround", CHANNEL_ENVIRONMENT, 0.4f);
		break;
	}
}