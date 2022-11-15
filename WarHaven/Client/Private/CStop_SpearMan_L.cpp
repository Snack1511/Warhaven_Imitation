#include "stdafx.h"
#include "CStop_SpearMan_L.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CStop_SpearMan_L::CStop_SpearMan_L()
{
}

CStop_SpearMan_L::~CStop_SpearMan_L()
{
}

CStop_SpearMan_L* CStop_SpearMan_L::Create()
{
	CStop_SpearMan_L* pInstance = new CStop_SpearMan_L();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CStop_SpearMan_L");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CStop_SpearMan_L::Initialize()
{

	__super::Initialize();

	m_eAnimType = ANIM_BASE_L;            // �ִϸ��̼��� �޽�Ÿ��
	m_iAnimIndex = 24;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
	m_eStateType = STATE_STOP_SPEARMAN_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

	m_vecAdjState.push_back(STATE_WALK_SPEARMAN_L);
	m_vecAdjState.push_back(STATE_RUN_SPEARMAN_L);

	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 22;
	m_iDirectionAnimIndex[STATE_DIRECTION_N] = 23;
	m_iDirectionAnimIndex[STATE_DIRECTION_S] = 24;
	m_iDirectionAnimIndex[STATE_DIRECTION_W] = 25;

	return S_OK;
}

void CStop_SpearMan_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{


	/* Owner�� Animator Set Idle�� */
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CStop_SpearMan_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
		return STATE_IDLE_SPEARMAN_L;

	return __super::Tick(pOwner, pAnimator);
}

void CStop_SpearMan_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);


}

STATE_TYPE CStop_SpearMan_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}

void CStop_SpearMan_L::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
