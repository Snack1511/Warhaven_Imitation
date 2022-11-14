#include "stdafx.h"
#include "CWarrior_Attack_HorizontalUp_R.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CWarrior_Attack_HorizontalUp_R::CWarrior_Attack_HorizontalUp_R()
{
}

CWarrior_Attack_HorizontalUp_R::~CWarrior_Attack_HorizontalUp_R()
{
}

CWarrior_Attack_HorizontalUp_R* CWarrior_Attack_HorizontalUp_R::Create()
{
	CWarrior_Attack_HorizontalUp_R* pInstance = new CWarrior_Attack_HorizontalUp_R();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWarrior_Attack_HorizontalUp_R");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CWarrior_Attack_HorizontalUp_R::Initialize()
{
	__super::Initialize();

	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 5;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_ATTACK_HORIZONTALUP_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


	m_vecAdjState.push_back(STATE_IDLE_PLAYER_L);
	m_vecAdjState.push_back(STATE_WALK_PLAYER_L);
	m_vecAdjState.push_back(STATE_RUN_PLAYER_L);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_L);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_L);

	m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_L);

	m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_L);
	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);
	m_vecAdjState.push_back(STATE_BOUNCE_PLAYER_R);


	return S_OK;
}

void CWarrior_Attack_HorizontalUp_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	if (ePrevType == STATE_SWITCH_L_TO_R)
	{
		m_fAnimSpeed = 2.3f;
	}
	else
		m_fAnimSpeed = 2.5f;


	/* Owner의 Animator Set Idle로 */
	__super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarrior_Attack_HorizontalUp_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Attack_HorizontalUp_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Exit(pOwner, pAnimator);

}

STATE_TYPE CWarrior_Attack_HorizontalUp_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* Player가 Attack 으로 오는 조건
	1.  LBuutton 을 이용해 공격한다.
	*/
	// 	


	return __super::Check_Condition(pOwner, pAnimator);
}

void CWarrior_Attack_HorizontalUp_R::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
