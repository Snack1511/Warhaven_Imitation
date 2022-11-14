#include "stdafx.h"
#include "CWarrior_Attack_HorizontalMiddle_R.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "CColorController.h"


CWarrior_Attack_HorizontalMiddle_R::CWarrior_Attack_HorizontalMiddle_R()
{
}

CWarrior_Attack_HorizontalMiddle_R::~CWarrior_Attack_HorizontalMiddle_R()
{
}

CWarrior_Attack_HorizontalMiddle_R* CWarrior_Attack_HorizontalMiddle_R::Create()
{
	CWarrior_Attack_HorizontalMiddle_R* pInstance = new CWarrior_Attack_HorizontalMiddle_R();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWarrior_Attack_HorizontalMiddle_R");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CWarrior_Attack_HorizontalMiddle_R::Initialize()
{

	__super::Initialize();

	m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
	m_iAnimIndex = 9;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_ATTACK_HORIZONTALMIDDLE_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 2.5f;

	// Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

	//enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
	m_iStateChangeKeyFrame = 50;

	m_vecAdjState.push_back(STATE_IDLE_PLAYER_L);
	m_vecAdjState.push_back(STATE_WALK_PLAYER_L);
	m_vecAdjState.push_back(STATE_RUN_PLAYER_L);

	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_L);
	m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_L);
	m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_L);

	m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_L);
	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);
	m_vecAdjState.push_back(STATE_BOUNCE_PLAYER_R);


	return S_OK;
}

void CWarrior_Attack_HorizontalMiddle_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
	if (ePrevType == STATE_SWITCH_L_TO_R)
	{
		m_fAnimSpeed = 2.3f;
	}
	else
		m_fAnimSpeed = 2.5f;


	/* Owner의 Animator Set Idle로 */
	__super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CWarrior_Attack_HorizontalMiddle_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (m_bMoveTrigger)
		Move(Get_Direction(), pOwner);


	//if (m_bMoveTrigger)
	//	Move(Get_Direction(), pOwner);

	//if (m_bAttackTrigger)
	//{
	//	// 공격 진입
	//	if (pOwner->Is_Weapon_R_Collision())
	//		return STATE_BOUNCE_PLAYER_R;

	//}

	return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Attack_HorizontalMiddle_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);

	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);

}

STATE_TYPE CWarrior_Attack_HorizontalMiddle_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);

	/* Player가 Attack 으로 오는 조건
	1.  LBuutton 을 이용해 공격한다.
	*/
	//if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
	//{
	//	return m_eStateType;
	//}

	//return STATE_END;
}

void CWarrior_Attack_HorizontalMiddle_R::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	__super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

}
