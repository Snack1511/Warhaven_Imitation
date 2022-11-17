#include "stdafx.h"
#include "CStop_WarHammer_L.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CStop_WarHammer_L::CStop_WarHammer_L()
{
}

CStop_WarHammer_L::~CStop_WarHammer_L()
{
}

CStop_WarHammer_L* CStop_WarHammer_L::Create()
{
	CStop_WarHammer_L* pInstance = new CStop_WarHammer_L();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CStop_WarHammer_L");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CStop_WarHammer_L::Initialize()
{

	__super::Initialize();

	m_eAnimType = ANIM_BASE_L;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 25;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_STOP_WARHAMMER_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_vecAdjState.push_back(STATE_IDLE_WARHAMMER_L);
	m_vecAdjState.push_back(STATE_WALK_WARHAMMER_L);
	m_vecAdjState.push_back(STATE_RUNBEGIN_WARHAMMER_L);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_L);
	m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_L);
	m_vecAdjState.push_back(STATE_ATTACK_STING_WARHAMMER_L);


	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 24;

	m_iDirectionAnimIndex[STATE_DIRECTION_N] = 25;
	m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 25;
	m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 25;

	m_iDirectionAnimIndex[STATE_DIRECTION_S] = 26;
	m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 26;
	m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 26;

	m_iDirectionAnimIndex[STATE_DIRECTION_W] = 27;

	return S_OK;
}

void CStop_WarHammer_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{


	/* Owner의 Animator Set Idle로 */
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CStop_WarHammer_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	//if (KEY(W, HOLD) || KEY(A, HOLD) || KEY(S, HOLD) || KEY(D, HOLD))
		//return STATE_RUN_BEGIN_WARHAMMER_L;
	return __super::Tick(pOwner, pAnimator);
}

void CStop_WarHammer_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);


}

STATE_TYPE CStop_WarHammer_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}

void CStop_WarHammer_L::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
