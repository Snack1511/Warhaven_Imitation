#include "stdafx.h"
#include "CStop_Player_L.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CStop_Player_L::CStop_Player_L()
{
}

CStop_Player_L::~CStop_Player_L()
{
}

CStop_Player_L* CStop_Player_L::Create()
{
	CStop_Player_L* pInstance = new CStop_Player_L();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CStop_Player_L");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CStop_Player_L::Initialize()
{

	__super::Initialize();

	m_eAnimType = ANIM_BASE_L;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 24;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_STOP_PLAYER_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	m_vecAdjState.push_back(STATE_IDLE_PLAYER_L);
	m_vecAdjState.push_back(STATE_WALK_PLAYER_L);
	m_vecAdjState.push_back(STATE_RUN_PLAYER_L);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_L);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_L);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_L);

	m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_L);

	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);

	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 23;
	m_iDirectionAnimIndex[STATE_DIRECTION_N] = 24;
	m_iDirectionAnimIndex[STATE_DIRECTION_S] = 25;
	m_iDirectionAnimIndex[STATE_DIRECTION_W] = 26;



	return S_OK;
}

void CStop_Player_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{


	/* Owner의 Animator Set Idle로 */
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CStop_Player_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	return __super::Tick(pOwner, pAnimator);
}

void CStop_Player_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);


}

STATE_TYPE CStop_Player_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}

void CStop_Player_L::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
